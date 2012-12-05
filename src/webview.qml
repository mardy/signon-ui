import QtQuick 2.0
import QtWebKit 3.0

WebView {
    id: webView
    width: 400
    height: 300

    Component.onCompleted: url = request.startUrl

    onLoadingChanged: {
        console.log("Loading changed")
        if (loadRequest.status === WebView.LoadSucceededStatus) {
            request.onLoadFinished(true)
        }
    }

    onUrlChanged: request.currentUrl = url
}
