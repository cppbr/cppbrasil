import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material


Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    Material.theme: Material.System
    Material.accent: Material.Blue
    title: qsTr("Nota Fiscal Eletrônica - NFe/NFCe")

    property int tipoDocDF : 0
    property int numDocDF  : 0

    Connections {
        target: notaFiscal
        function onRetWSChange(webServices) {
            tSaida.text = tSaida.text + webServices
        }
        function onRetStatusServico(status) {
            tSaida.text = tSaida.text + status
            busyIndicator.running = false
        }
        function onRetLote(lote) {
            tSaida.text = tSaida.text + lote
            busyIndicator.running = false
        }
        function onRetXML(xml) {
            tXML.text = xml
            busyIndicator.running = false
        }
    }

    TabBar {
        id: bar
        width: parent.width
        anchors{
            top: mainWindow.top
            left: mainWindow.left
            right: mainWindow.right
        }

        TabButton{
            text: qsTr("Nota Fiscal")
        }
        TabButton{
            text: qsTr("XML")
        }
    }

    StackLayout {
        width: parent.width
        height: parent.height - bar.height
        currentIndex: bar.currentIndex
        anchors {
            top: bar.bottom
            left: mainWindow.left
            right: mainWindow.right
            bottom: mainWindow.bottom
        }

        Item {
            id: nota
            GridLayout {
                id: grid
                anchors.fill: parent
                columns: 2
                rows: 1

                Rectangle {
                    Layout.preferredWidth  : parent.width * 0.35
                    Layout.preferredHeight : parent.height
                    color: "transparent"

                    Column {
                       id: columnLayout
                       width: parent.width
                       height: parent.height
                       spacing: 5

                       BusyIndicator {
                           id: busyIndicator
                           anchors.horizontalCenter: parent.horizontalCenter
                           running: false
                       }

                       Label {
                           id: lbCombobox
                           anchors.horizontalCenter: parent.horizontalCenter
                           text: qsTr("Tipo de Documento")
                           font.pointSize: 10
                       }

                       ComboBox {
                           id: cmbModeloDF
                           anchors.horizontalCenter: parent.horizontalCenter
                           textRole: "text"
                           model: ListModel {
                               id: listModeloDF
                               ListElement {text: "NFe"; doc:"55"}
                               ListElement {text: "NFCe"; doc:"65"}
                           }
                           onCurrentIndexChanged: tipoDocDF = parseInt(model.get(currentIndex).doc)
                       }

                       Label {
                           id: lbFieldDocNumber
                           anchors.horizontalCenter: parent.horizontalCenter
                           text: qsTr("Número do Documento")
                           font.pointSize: 10
                       }

                       TextField {
                           id: txDocNumber
                           anchors.horizontalCenter: parent.horizontalCenter
                           width: parent.width * 0.70
                           placeholderText: qsTr("Número da NFe/NFCe")
                           font.bold: true
                           horizontalAlignment : TextInput.AlignHCenter
                           inputMethodHints: Qt.ImhDigitsOnly

                           onTextChanged: numDocDF = parseInt(text)
                       }


                       Button {
                           id: btStatus
                           anchors.horizontalCenter: parent.horizontalCenter
                           text: qsTr("Status do Serviço")
                           font.pointSize: 10
                           onClicked: {
                               busyIndicator.running = true
                               notaFiscal.onReqStatusServico()
                           }
                       }

                       Button {
                           id: btEnviar
                           anchors.horizontalCenter: parent.horizontalCenter
                           text: qsTr("Enviar NFe")
                           font.pointSize: 10
                           onClicked: {
                               busyIndicator.running = true
                               notaFiscal.onReqGerarEnviar(numDocDF, tipoDocDF)
                           }
                       }

                       Rectangle {
                           id: recIm
                           anchors.horizontalCenter: parent.horizontalCenter
                           width: 121
                           height: 122
                           Image {
                               id: image
                               anchors.fill: parent
                               source: "images/nfe.png"
                               anchors.rightMargin: 0
                               anchors.bottomMargin: 0
                               anchors.leftMargin: 0
                               anchors.topMargin: 0
                               fillMode: Image.PreserveAspectFit
                           }
                       }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredWidth  : parent.width * 0.60
                    Layout.preferredHeight : parent.height
                    color: "transparent"
                    ScrollView {
                       anchors.fill: parent
                       TextArea {
                            id: tSaida
                            x: 0
                            y: 0
                            width: 411
                            height: 421
                            Layout.fillWidth: true
                            Layout.preferredHeight : parent.height
                            focus: true
                            text: ""
                            selectByMouse:  true
                            persistentSelection: true //copy and paste need this!
                       }
                    }
                }

            }

        }
        Item {
            id:xml
            ScrollView {
                anchors.fill: parent
                 TextArea {
                     id: tXML
                     Layout.fillWidth: true
                     Layout.preferredHeight : parent.height
                     focus: true
                     wrapMode: TextEdit.WordWrap
                     text: ""
                     selectByMouse:  true
                     persistentSelection: true //copy and paste need this!
                 }
            }

        }

    }
}
