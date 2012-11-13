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

#ifndef SIGNON_UI_REQUEST_H
#define SIGNON_UI_REQUEST_H

#include <QDBusConnection>
#include <QDBusMessage>
#include <QObject>
#include <QVariantMap>
#include <QWidget>

namespace SignOnUi {

class RequestPrivate;

class Request: public QObject
{
    Q_OBJECT

public:
    static Request *newRequest(const QDBusConnection &connection,
                               const QDBusMessage &message,
                               const QVariantMap &parameters,
                               QObject *parent = 0);
    ~Request();

    static QString id(const QVariantMap &parameters);
    QString id() const;

    uint identity() const;
    QString method() const;
    QString mechanism() const;
    WId windowId() const;
    bool embeddedUi() const;

    bool isInProgress() const;

    const QVariantMap &parameters() const;
    const QVariantMap &clientData() const;

public Q_SLOTS:
    virtual void start();
    void cancel();

Q_SIGNALS:
    void completed();

protected:
    explicit Request(const QDBusConnection &connection,
                     const QDBusMessage &message,
                     const QVariantMap &parameters,
                     QObject *parent = 0);

    void setWidget(QWidget *widget);

    void fail(const QString &name, const QString &message);
    void setCanceled();
    void setResult(const QVariantMap &result);

private:
    RequestPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Request)
};

} // namespace

#endif // SIGNON_UI_REQUEST_H

