import QtQuick 2.1
import Sailfish.Silica 1.0
import "../js/Settings.js" as Settings
import harbour.tox 1.0
import ".."

Dialog {
    id: multimedia_root
    height: parent.height; width: parent.width
    property int video_mode: 1
    property int audio_mode: 1

    onAccepted: {
        Settings.set("toxId", inputToxId.text)
        Settings.set("name", inputName.text)
        Settings.set("status", inputStatus.text)
        Settings.set("audio", audio_mode)
        Settings.set("video", video_mode)
        Settings.set('proxy', inputProxy.text)
        Settings.set('proxyPort', inputProxyPort.text)
        tox.setStatusMessage(inputStatus.text);
        tox.setName(inputName.text);
    }

    DialogHeader{
        id: param_h
        acceptText: qsTr("Valider")
    }

    Component.onCompleted: {
        Settings.initialize()
        video_mode = Settings.get("video")
        audio_mode = Settings.get("audio")
        inputName.text = Settings.get('name')
        inputToxId.text = tox.getAddress(); //Settings.get('toxId')
        inputStatus.text = Settings.get('status')
        inputProxy.text = Settings.get('proxy')
        inputProxyPort.text = Settings.get('proxyPort')
        inputAudio.currentIndex = audio_mode
        inputVideo.currentIndex = video_mode
    }

    Column{
        id: col
        width: parent.width
        height: childrenRect.height

        anchors {
            top: param_h.bottom
            left: parent.left;
            right: parent.right
            leftMargin: Theme.paddingLarge
            rightMargin: Theme.paddingLarge
        }

        TextField{
            placeholderText: qsTr("tox ID")
            label: placeholderText
            width: parent.width
            id: inputToxId
        }
        TextField{
            placeholderText: qsTr("Name")
            label: placeholderText
            width: parent.width
            id: inputName
        }
        TextField{
            placeholderText: qsTr("Status")
            label: placeholderText
            width: parent.width
            id: inputStatus
        }

        ComboBox{
            label: qsTr("Audio")
            id: inputAudio
            width: parent.width
            onCurrentIndexChanged: {
                audio_mode = currentIndex
            }
            Component.onCompleted: {
                currentIndex = audio_mode
            }
            menu: ContextMenu{
                MenuItem{
                    text: qsTr("No audio")
                }
                MenuItem{
                    text: qsTr("Loudspeaker")
                }
                MenuItem{
                    text: qsTr("Earphone")
                }
            }
        }

        ComboBox{
            label: qsTr("Video")
            id: inputVideo
            width: parent.width
            onCurrentIndexChanged: {
                video_mode = currentIndex
            }
            menu: ContextMenu{
                MenuItem{
                    text: qsTr("No vidéo")
                }
                MenuItem{
                    text: qsTr("Front camera 176x144")
                }
                MenuItem{
                    text: qsTr("Front camera 320x240")
                }
                MenuItem{
                    text: qsTr("Front camera native")
                }
                MenuItem{
                    text: qsTr("Back camera 176x144")
                }
                MenuItem{
                    text: qsTr("Back camera 320x240")
                }
                MenuItem{
                    text: qsTr("Back camera native")
                }
            }
            Component.onCompleted: {
                currentIndex = video_mode
            }
        }
        TextField{
            placeholderText: qsTr("Proxy")
            label: placeholderText
            width: parent.width
            id: inputProxy
        }
        TextField{
            placeholderText: qsTr("Port")
            label: placeholderText
            width: parent.width
            id: inputProxyPort
        }
    }
}
