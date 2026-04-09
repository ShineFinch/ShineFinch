import QtQuick
import QtQuick.Window
import QtQuick.Controls
import ShineFinch

Window {
    width: 640
    height: 480
    visible: false
    title: qsTr("ShineFinch")

    property bool showScreenshotOverlay: false

    Item {
        visible: showScreenshotOverlay
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: "#000000"
            opacity: 0.85
        }

    }
}
