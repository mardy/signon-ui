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

#include "browser-request.h"

#include "cookie-jar-manager.h"
#include "debug.h"
#include "dialog.h"
#include "i18n.h"

#include <QDBusArgument>
#include <QDesktopServices>
#include <QLabel>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QProgressBar>
#include <QPushButton>
#include <QQmlContext>
#include <QRegExp>
#include <QSettings>
#include <QWebElement>
#include <QWebFrame>
#include <QWebView>
#include <SignOn/uisessiondata_priv.h>

using namespace SignOnUi;

namespace SignOnUi {

static const QString keyPreferredWidth = QString("PreferredWidth");
static const QString keyTextSizeMultiplier = QString("TextSizeMultiplier");
static const QString keyUserAgent = QString("UserAgent");
static const QString keyViewportWidth = QString("ViewportWidth");
static const QString keyViewportHeight = QString("ViewportHeight");
static const QString keyZoomFactor = QString("ZoomFactor");
static const QString keyUsernameField = QString("UsernameField");
static const QString keyPasswordField = QString("PasswordField");
static const QString keyLoginButton = QString("LoginButton");
static const QString keyInternalLinksPattern = QString("InternalLinksPattern");
static const QString keyExternalLinksPattern = QString("ExternalLinksPattern");
static const QString keyAllowedUrls = QString("AllowedUrls");

/* Additional session-data keys we support. */
static const QString keyCookies = QString("Cookies");
static const QString keyAllowedSchemes = QString("AllowedSchemes");

#if 0
class WebPage: public QWebPage
{
    Q_OBJECT

public:
    WebPage(QObject *parent = 0): QWebPage(parent) {}
    ~WebPage() {}

    void setUserAgent(const QString &userAgent) { m_userAgent = userAgent; }

    void setExternalLinksPattern(const QString &pattern) {
        m_externalLinksPattern =
            QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp2);
    }

    void setInternalLinksPattern(const QString &pattern) {
        m_internalLinksPattern =
            QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp2);
    }

    void setAllowedSchemes(const QStringList &schemes) {
        m_allowedSchemes = schemes;
    }

    void setAllowedUrls(const QString &pattern) {
        m_allowedUrls =
            QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp2);
    }

    void setFinalUrl(const QUrl &url) { m_finalUrl = url; }

protected:
    // reimplemented virtual methods
    QString userAgentForUrl(const QUrl &url) const
    {
        return m_userAgent.isEmpty() ?
            QWebPage::userAgentForUrl(url) : m_userAgent;
    }

    bool acceptNavigationRequest(QWebFrame *frame,
                                 const QNetworkRequest &request,
                                 NavigationType type)
    {
        Q_UNUSED(type);

        QUrl url = request.url();
        TRACE() << url;

        /* We generally don't need to load the final URL, so skip loading it.
         * If this behaviour is not desired for some requests, then just avoid
         * calling setFinalUrl() */
        if (url.host() == m_finalUrl.host() &&
            url.path() == m_finalUrl.path()) {
            Q_EMIT finalUrlReached(url);
            return false;
        }

        /* open all new window requests (identified by "frame == 0") in the
         * external browser, as well as other links according to the
         * ExternalLinksPattern and InternalLinksPattern rules. */
        if (frame == 0 || urlIsBlocked(url)) {
            QDesktopServices::openUrl(url);
            return false;
        }
        /* Handle all other requests internally. */
        return true;
    }

Q_SIGNALS:
    void finalUrlReached(const QUrl &url);

private:
    bool urlIsBlocked(QUrl url) const;

private:
    QString m_userAgent;
    QRegExp m_externalLinksPattern;
    QRegExp m_internalLinksPattern;
    QStringList m_allowedSchemes;
    QRegExp m_allowedUrls;
    QUrl m_finalUrl;
};

bool WebPage::urlIsBlocked(QUrl url) const {
    if (!m_allowedSchemes.contains(url.scheme())) {
        TRACE() << "Scheme not allowed:" << url.scheme();
        return true;
    }

    if (!m_allowedUrls.isEmpty() &&
        !m_allowedUrls.exactMatch(url.toString())) {
        TRACE() << "URL not allowed:" << url;
        return true;
    }

    QString urlText = url.toString(QUrl::RemoveScheme |
                                   QUrl::RemoveUserInfo |
                                   QUrl::RemoveFragment |
                                   QUrl::StripTrailingSlash);
    if (urlText.startsWith("//")) {
        urlText = urlText.mid(2);
    }

    if (!m_internalLinksPattern.isEmpty()) {
        return !m_internalLinksPattern.exactMatch(urlText);
    }

    if (!m_externalLinksPattern.isEmpty()) {
        return m_externalLinksPattern.exactMatch(urlText);
    }

    return false;
}

class WebView: public QWebView
{
    Q_OBJECT

public:
    WebView(QWidget *parent = 0):
        QWebView(parent)
    {
        setSizePolicy(QSizePolicy::MinimumExpanding,
                      QSizePolicy::MinimumExpanding);
        setAttribute(Qt::WA_OpaquePaintEvent, true);
    }
    ~WebView() {};

    void setPreferredSize(const QSize &size) {
        m_preferredSize = size;
        updateGeometry();
    }

protected:
    QSize sizeHint() const {
        if (m_preferredSize.isValid()) {
            return m_preferredSize;
        } else {
            return QSize(400, 300);
        }
    }

    void paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.fillRect(rect(), palette().window());
        QWebView::paintEvent(event);
    }

private:
    QSize m_preferredSize;
};
#endif

class BrowserRequestPrivate: public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(BrowserRequest)
    Q_PROPERTY(QUrl currentUrl READ currentUrl WRITE setCurrentUrl)
    Q_PROPERTY(QUrl startUrl READ startUrl CONSTANT)
    Q_PROPERTY(QUrl finalUrl READ finalUrl CONSTANT)

public:
    BrowserRequestPrivate(BrowserRequest *request);
    ~BrowserRequestPrivate();

    QUrl currentUrl() const { return m_currentUrl; }
    QUrl startUrl() const { return m_startUrl; }
    QUrl finalUrl() const { return m_finalUrl; }
    QUrl responseUrl() const { return m_responseUrl; }

    void setCurrentUrl(const QUrl &url);

    QWidget *buildSuccessPage();
    QWidget *buildLoadFailurePage();
    void buildDialog(const QVariantMap &params);
    void start();

public Q_SLOTS:
    void onLoadFinished(bool ok);
    void onFinished();
    void onContentsChanged();

private:
    void showDialog();

private:
    mutable BrowserRequest *q_ptr;
    Dialog *m_dialog;
    QUrl m_currentUrl;
    QUrl m_startUrl;
    QUrl m_finalUrl;
    QUrl m_responseUrl;
    QString m_host;
    QSettings *m_settings;
    QWebElement m_usernameField;
    QWebElement m_passwordField;
    QWebElement m_loginButton;
    QString m_username;
    QString m_password;
    int m_loginCount;
};

} // namespace

BrowserRequestPrivate::BrowserRequestPrivate(BrowserRequest *request):
    QObject(request),
    q_ptr(request),
    m_dialog(0),
    m_settings(0),
    m_loginCount(0)
{
    static bool pathAdded = false;

    if (!pathAdded) {
        // Ensure that QWebProcess will be found
        qputenv("PATH", qgetenv("PATH") + ":/opt/qt5/bin");
        pathAdded = true;
    }
}

BrowserRequestPrivate::~BrowserRequestPrivate()
{
    delete m_dialog;
}

void BrowserRequestPrivate::setCurrentUrl(const QUrl &url)
{
    Q_Q(BrowserRequest);

    TRACE() << "Url changed:" << url;

    if (url.host() == m_finalUrl.host() &&
        url.path() == m_finalUrl.path()) {
        m_responseUrl = url;
        if (q->embeddedUi() || !m_dialog->isVisible()) {
            /* Do not show the notification page. */
            m_dialog->accept();
        } else {
            /* Replace the web page with an information screen */
            /* TODO */
            m_dialog->accept();
        }
    }
}

void BrowserRequestPrivate::onLoadFinished(bool ok)
{
    TRACE() << "Load finished" << ok;

    if (!m_dialog->isVisible()) {
        if (m_responseUrl.isEmpty()) {
            showDialog();
        } else {
            onFinished();
        }
    }
}

void BrowserRequestPrivate::buildDialog(const QVariantMap &params)
{
    m_dialog = new Dialog;

    QString title;
    if (params.contains(SSOUI_KEY_TITLE)) {
        title = params[SSOUI_KEY_TITLE].toString();
    } else if (params.contains(SSOUI_KEY_CAPTION)) {
        title = _("Web authentication for %1").
            arg(params[SSOUI_KEY_CAPTION].toString());
    } else {
        title = _("Web authentication");
    }

    m_dialog->setTitle(title);

    TRACE() << "Dialog was built";
}

void BrowserRequestPrivate::start()
{
    Q_Q(BrowserRequest);

    const QVariantMap &params = q->parameters();

    m_finalUrl = params.value(SSOUI_KEY_FINALURL).toString();
    m_startUrl = params.value(SSOUI_KEY_OPENURL).toString();
    buildDialog(params);

    QObject::connect(m_dialog, SIGNAL(finished(int)),
                     this, SLOT(onFinished()));

    m_dialog->rootContext()->setContextProperty("request", this);
    m_dialog->setSource(QUrl("qrc:/webview.qml"));
}

void BrowserRequestPrivate::onFinished()
{
    Q_Q(BrowserRequest);

    TRACE() << "Browser dialog closed";

    QVariantMap reply;
    QUrl url = m_responseUrl.isEmpty() ? m_currentUrl : m_responseUrl;
    reply[SSOUI_KEY_URLRESPONSE] = url.toString();

    if (!m_username.isEmpty())
        reply[SSOUI_KEY_USERNAME] = m_username;
    if (!m_password.isEmpty())
        reply[SSOUI_KEY_PASSWORD] = m_password;

    q->setResult(reply);
    m_dialog->close();
}

void BrowserRequestPrivate::onContentsChanged()
{
    /* See https://bugs.webkit.org/show_bug.cgi?id=32865 for the reason why
     * we are not simply calling m_usernameField.attribute("value")
     */
    if (!m_usernameField.isNull()) {
        m_username =
            m_usernameField.evaluateJavaScript("this.value").toString();
    }
    if (!m_passwordField.isNull()) {
        m_password =
            m_passwordField.evaluateJavaScript("this.value").toString();
    }
}

void BrowserRequestPrivate::showDialog()
{
    m_dialog->show();
}

BrowserRequest::BrowserRequest(const QDBusConnection &connection,
                             const QDBusMessage &message,
                             const QVariantMap &parameters,
                             QObject *parent):
    Request(connection, message, parameters, parent),
    d_ptr(new BrowserRequestPrivate(this))
{
}

BrowserRequest::~BrowserRequest()
{
}

void BrowserRequest::start()
{
    Q_D(BrowserRequest);

    Request::start();
    d->start();
}

#include "browser-request.moc"
