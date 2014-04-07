import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Extras.Browser 0.2
import com.canonical.Oxide 0.1

UbuntuWebView {
    Component.onCompleted: url = signonRequest.startUrl

    onLoadingChanged: {
        console.log("Loading changed")
        if (loading) {
            signonRequest.onLoadStarted()
        } else if (lastLoadSucceeded) {
            signonRequest.onLoadFinished(true)
        } else {
            signonRequest.onLoadFinished(false)
        }
    }
    onUrlChanged: signonRequest.currentUrl = url

    context: UbuntuWebContext {
        dataPath: rootDir
    }
}
