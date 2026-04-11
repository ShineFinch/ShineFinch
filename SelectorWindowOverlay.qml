import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    width: 420

    RowLayout {
        id: textContainer
        Layout.fillWidth: true
        Layout.preferredHeight: flickable.visible ? flickable.height : 0
        Layout.topMargin: 10
        Layout.leftMargin: 12
        Layout.rightMargin: 12

        Flickable {
            id: flickable
            visible: false
            width: parent.width
            height: Math.min(ocrResultArea.implicitHeight, 200)
            contentWidth: width
            contentHeight: ocrResultArea.implicitHeight

            ScrollBar.vertical: ScrollBar { }

            TextArea {
                id: ocrResultArea
                text: ""
                readOnly: true
                wrapMode: TextEdit.Wrap
                selectByMouse: true

                width: flickable.width
                padding: 0
                background: Rectangle { color: "transparent" }
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 12

        Item { Layout.fillWidth: true }

        Button {
            text: "OK"
            onClicked: root.tiggerConfirmed()
        }
        Button {
            text: "Cancel"
            onClicked: root.tiggerCanceled()
        }
        Button {
            text: "OCR"
            onClicked: root.triggerOCR()
        }
    }

    signal tiggerConfirmed
    signal tiggerCanceled
    signal triggerOCR

    function setOcrResult(text) {
        ocrResultArea.text = text
        flickable.visible = true
    }

    function resetOcrResult() {
        ocrResultArea.text = ""
        flickable.visible = false
    }
}