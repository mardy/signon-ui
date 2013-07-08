import QtQuick 2.0
import QtQuick.Window 2.0
import QtWebKit 3.0
import Ubuntu.Components.Extras.Browser 0.1

FocusScope {
    id: browser
    width: 400
    height: 300
    focus: true
    property string qtwebkitdpr: "1.0"

    UbuntuWebView {
        id: webView
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: osk.top
        }
        focus: true

        Component.onCompleted: url = request.startUrl

        onLoadingChanged: {
            console.log("Loading changed")
            if (loadRequest.status === WebView.LoadSucceededStatus) {
                request.onLoadFinished(true)
            }
        }
        onUrlChanged: request.currentUrl = url
    }

    KeyboardRectangle {
        id: osk
    }
}
