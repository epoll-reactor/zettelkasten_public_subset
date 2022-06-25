import QtQuick 2.0
import QtWebEngine 1.7
import QtQuick.Controls 2.4

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: qsTr("Site renderer")

    WebEngineView {
        id: webEngineView
        url: "file:///home/machen/code/C++/CEF_renderer/Work_page/examples/dashboard.html"
        anchors.fill: parent
    }
}
