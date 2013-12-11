import QtQuick 2.0
import QtWebKit 3.1
import Ubuntu.Components 0.1
import Ubuntu.Components.Extras.Browser 0.1

FocusScope {
    id: browser
    anchors.fill: parent
    focus: true

    UbuntuWebView {
        id: webView
        anchors.fill: parent
        focus: true

        Component.onCompleted: url = request.startUrl

        onLoadingChanged: {
            console.log("Loading changed")
            if (loadRequest.status === WebView.LoadSucceededStatus) {
                request.onLoadFinished(true)
            } else if (loadRequest.status === WebView.LoadFailedStatus) {
                request.onLoadFinished(false)
            } else if (loadRequest.status === WebView.LoadStarted) {
                request.onLoadStarted()
            }
        }
        onUrlChanged: request.currentUrl = url
    }
}
