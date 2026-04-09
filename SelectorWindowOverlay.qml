import QtQuick
import QtQuick.Controls

Row {
    width: 160
    height: 50
    spacing: 12
    anchors.centerIn: parent

    Button {
        text: "OK"
        // background: Rectangle { color: "#2EBD68"; radius: 8 }
        onClicked: {
               console.log("confirm() clicked!")
               tiggerConfirmed()
        }
    }
    Button {
        text: "Cancel"
        // background: Rectangle { color: "#FF5252"; radius: 8 }
        onClicked: tiggerCanceled()
    }

    signal tiggerConfirmed()
    signal tiggerCanceled()
}