import QtQuick 2.0
import QtQuick.Window 2.0
import QtWebKit 3.0
import QtWebKit.experimental 1.0
import Ubuntu.Components 0.1
import Ubuntu.Unity.Action 1.0 as UnityActions
import "ua-overrides.js" as Overrides

MainView {
    id: root
    width: units.gu(60)
    height: units.gu(90)

    Action {
        id: cancelAction
        text: i18n.tr("Cancel")
        onTriggered: Qt.quit()
    }

    actions: [cancelAction]

    FocusScope {
        id: browser
        anchors.fill: parent
        focus: true
        property string qtwebkitdpr: "1.0"

        QtObject {
            // clumsy way of defining an enum in QML
            id: formFactor
            readonly property int desktop: 0
            readonly property int phone: 1
            readonly property int tablet: 2
        }
        // FIXME: this is a quick hack that will become increasingly unreliable
        // as we support more devices, so we need a better solution for this
        // FIXME: only handling phone and tablet for now
        property int formFactor: (Screen.width >= units.gu(60)) ? formFactor.tablet : formFactor.phone

        onQtwebkitdprChanged: {
            // Do not make this patch to QtWebKit a hard requirement.
            if (webview.experimental.hasOwnProperty('devicePixelRatio')) {
                webview.experimental.devicePixelRatio = qtwebkitdpr
            }
        }

        WebView {
            id: webView
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: osk.top
            }
            focus: true
            UserAgent {
                id: userAgent
            }

            experimental.userAgent: userAgent.defaultUA

            experimental.preferences.developerExtrasEnabled: false
            experimental.preferences.navigatorQtObjectEnabled: true

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
}
