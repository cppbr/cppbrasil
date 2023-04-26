import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    id: mainForm
    width: 640
    height: 480
    visible: true
    title: qsTr("Client NF-e")

    property int tipoDoc : 0
    property int numDoc  : 0

    function substring(str, len) {
        return str.substring(0, len);
    }

    Column {
            anchors.fill: parent
            spacing: 10

            Rectangle {
                id: rectTop
                color: "#0096FF"
                width: parent.width
                height: 180
                Column {
                        anchors.fill: parent
                        spacing: 2

                        Row {
                            id: row1
                            anchors.horizontalCenter: parent.horizontalCenter

                            Label {
                                id: lbCombobox
                                anchors.verticalCenter: parent.verticalCenter
                                text: qsTr("Modelo:  ")
                                font.pointSize: 15
                            }

                            ComboBox {
                                id: cmbModeloDF
                                width: 120
                                textRole: "text"
                                font.pointSize: 15
                                model: ListModel {
                                    id: listModeloDF
                                    ListElement {text: "NFe"; doc:"55"}
                                    ListElement {text: "NFCe"; doc:"65"}
                                }
                                onCurrentIndexChanged: tipoDoc = parseInt(model.get(currentIndex).doc)
                            }
                        }

                        Row {
                            id: row2
                            anchors.horizontalCenter: parent.horizontalCenter

                            Label {
                                id: lbFieldDocNumber
                                anchors.verticalCenter: parent.verticalCenter
                                text: qsTr("Número: ")
                                font.pointSize: 15

                            }

                            TextField {
                                id: txDocNumber
                                width: 120
                                placeholderText: qsTr("N° da NFe/NFCe")
                                font.pointSize: 15
                                font.bold: true
                                horizontalAlignment : TextInput.AlignHCenter
                                inputMethodHints: Qt.ImhDigitsOnly

                                onTextChanged: numDoc = parseInt(text)
                            }
                        }

                        Rectangle {
                            id: rec2
                            anchors.horizontalCenter: parent.horizontalCenter
                            //color: "lightblue"
                            //gradient: Gradient {
                            //    GradientStop { position: 0.0; color: "lightblue" }
                            //    GradientStop { position: 1.0; color: "nave" }
                            //}
                            smooth: true
                            border.width: parent.border.width/2
                            border.color: "#22FFFFFF"

                            gradient: Gradient {
                                GradientStop { position: 0;    color: "#7DE5ED" }
                                GradientStop { position: .1;   color: "#81C6E8" }
                                GradientStop { position: .5;   color: "#5DA7DB" }
                                GradientStop { position: .501; color: "#5DA7DB" }
                                GradientStop { position: .8;   color: "#81C6E8" }
                                GradientStop { position: 1;    color: "#7DE5ED" }
                            }
                            radius: 20.0
                            width: 150
                            height: 35

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pointSize: 15
                                color: "white"
                                text: "Enviar"
                            }


                            BusyIndicator {
                                id: busyIndicator
                                height: parent.height
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                running: false

                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                      busyIndicator.running = true
                                      var server = txServer.text
                                      var xhr = new XMLHttpRequest();
                                      xhr.onreadystatechange = function() {
                                          if (xhr.readyState === XMLHttpRequest.DONE) {
                                              if (xhr.status === 200) {
                                                  txSaida.text = xhr.responseText
                                              } else {
                                                  console.error("Error creating/updating user: ", xhr.statusText)
                                              }
                                              busyIndicator.running = false
                                          }
                                      }
                                      xhr.open("POST", substring(txServer.text, 4) === "http" ? server + "/nota" : "http://" + server + "/nota");
                                      xhr.setRequestHeader("Content-Type", "application/json");
                                      xhr.send(JSON.stringify({
                                                                  nNFe: numDoc.toString(),
                                                                  tpDoc: tipoDoc.toString()
                                                              }));

                                  }

                            }

                        }

                        Row {
                            id: row4
                            anchors.horizontalCenter: parent.horizontalCenter

                            TextField {
                                id: txServer
                                width: parent.parent.width
                                placeholderText: qsTr("Endereço do servidor")
                                font.pointSize: 15
                                font.bold: true
                                horizontalAlignment : TextInput.AlignHCenter
                            }

                        }


                }
            }


            Rectangle {
                id: recText
                width: parent.width
                height: parent.height - rectTop.height - parent.spacing
                color: "transparent"

                ScrollView {
                    anchors.fill: parent
                     TextArea {
                         id: txSaida
                         Layout.fillWidth: true
                         focus: true
                         wrapMode: TextEdit.WordWrap
                         font.pointSize: 10
                         text: ""
                         selectByMouse:  true
                         persistentSelection: true //copy and paste need this!
                     }
                }
            }


    }


}
