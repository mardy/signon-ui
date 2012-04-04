/*
 * This file is part of signon-ui
 *
 * Copyright (C) 2011 Canonical Ltd.
 *
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "service.h"

#include "debug.h"
#include "request.h"

#include <QTimer>
#include <QQueue>

using namespace SignOnUi;

namespace SignOnUi {

typedef QQueue<Request*> RequestQueue;

class ServicePrivate: public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(Service)

public:
    ServicePrivate(Service *service);
    ~ServicePrivate();

    void setTimeout(int timeout);

    RequestQueue &queueForWindowId(WId windowId);
    void enqueue(Request *request);
    void runQueue(RequestQueue &queue);
    void cancelUiRequest(const QString &requestId);

private:
    void resetTimer();

private Q_SLOTS:
    void onRequestCompleted();
    void onTimeout();

private:
    mutable Service *q_ptr;
    QTimer m_timer;
    int m_timeout;
    /* each window Id has a different queue */
    QMap<WId,RequestQueue> m_requests;
};

} // namespace

ServicePrivate::ServicePrivate(Service *service):
    QObject(service),
    q_ptr(service),
    m_timeout(0)
{
    m_timer.setSingleShot(true);
    QObject::connect(&m_timer, SIGNAL(timeout()),
                     this, SLOT(onTimeout()));
}

ServicePrivate::~ServicePrivate()
{
}

void ServicePrivate::setTimeout(int timeout)
{
    m_timeout = timeout * 1000;
    resetTimer();
}

void ServicePrivate::resetTimer()
{
    m_timer.stop();
    if (m_timeout > 0) {
        m_timer.start(m_timeout);
    }
}

void ServicePrivate::onTimeout()
{
    Q_Q(Service);

    if (m_requests.isEmpty()) {
        Q_EMIT q->idleTimeout();
    }
}

RequestQueue &ServicePrivate::queueForWindowId(WId windowId)
{
    if (!m_requests.contains(windowId)) {
        RequestQueue queue;
        m_requests.insert(windowId, queue);
    }
    return m_requests[windowId];
}

void ServicePrivate::enqueue(Request *request)
{
    resetTimer();

    WId windowId = request->windowId();

    RequestQueue &queue = queueForWindowId(windowId);
    queue.enqueue(request);

    runQueue(queue);
}

void ServicePrivate::runQueue(RequestQueue &queue)
{
    Request *request = queue.head();
    TRACE() << "Head:" << request;

    if (request->isInProgress()) {
        TRACE() << "Already in progress";
        return; // Nothing to do
    }

    QObject::connect(request, SIGNAL(completed()),
                     this, SLOT(onRequestCompleted()));
    request->start();
}

void ServicePrivate::onRequestCompleted()
{
    resetTimer();

    Request *request = qobject_cast<Request*>(sender());
    WId windowId = request->windowId();

    RequestQueue &queue = queueForWindowId(windowId);
    if (request != queue.head()) {
        BLAME() << "Completed request is not first in queue!";
        return;
    }

    queue.dequeue();
    request->deleteLater();

    if (queue.isEmpty()) {
        m_requests.remove(windowId);
    } else {
        /* start the next request */
        runQueue(queue);
    }
}

void ServicePrivate::cancelUiRequest(const QString &requestId)
{
    Request *request = 0;

    /* Find the request; we don't know in which queue it is, so we must search
     * all queues. */
    foreach (RequestQueue queue, m_requests) {
        foreach (Request *r, queue) {
            if (r->id() == requestId) {
                request = r;
                break;
            }
        }
    }

    TRACE() << "Cancelling request" << request;
    if (request != 0) {
        request->cancel();
    }
}

Service::Service(QObject *parent):
    QObject(parent),
    d_ptr(new ServicePrivate(this))
{
}

Service::~Service()
{
}

void Service::setTimeout(int timeout)
{
    Q_D(Service);
    d->setTimeout(timeout);
}

QVariantMap Service::queryDialog(const QVariantMap &parameters)
{
    Q_D(Service);

    TRACE() << "Got request:" << parameters;
    Request *request = Request::newRequest(connection(),
                                           message(),
                                           parameters,
                                           this);
    d->enqueue(request);

    /* The following line tells QtDBus not to generate a reply now */
    setDelayedReply(true);
    return QVariantMap();
}

QVariantMap Service::refreshDialog(const QVariantMap &newParameters)
{
    QString requestId = Request::id(newParameters);
    // TODO find the request and update it

    /* The following line tells QtDBus not to generate a reply now */
    setDelayedReply(true);
    return QVariantMap();
}

void Service::cancelUiRequest(const QString &requestId)
{
    Q_D(Service);
    d->cancelUiRequest(requestId);
}

#include "service.moc"
