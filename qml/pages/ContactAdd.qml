import QtQuick 2.1
import Sailfish.Silica 1.0
import "../js/Contacts.js" as Contacts
import harbour.tox 1.0
import ".."

Dialog {
    id: contact_add
    //height: parent.height; width: parent.width
    height: childrenRect.height

    onAccepted: {
        //Contacts.addContact(contactToxId.text, contactName.text, contact_send_msg.checked);
        tox.addContact(contactToxId.text, contactName.text, contact_send_msg.checked);
        var nb_cts = tox.count();
        console.log(nb_cts, " contacts now.");
    }

    DialogHeader{
        id: contact_add_h
        acceptText: qsTr("Create contact")
        acceptTextVisible: true;
    }

    Component.onCompleted: {
        /*video_mode = Settings.get("video")
        audio_mode = Settings.get("audio")
        inputName.text = Settings.get('name')
        inputToxId.text = Settings.get('toxId')
        inputStatus.text = Settings.get('status')
        inputProxy.text = Settings.get('proxy')
        inputProxyPort.text = Settings.get('proxyPort')
        inputAudio.currentIndex = audio_mode
        inputVideo.currentIndex = video_mode*/
        console.log("ContactAdd completed")
    }

    Column{
        id: col
        width: parent.width
        height: parent.height

        anchors {
            top: contact_add_h.bottom
            left: parent.left
            right: parent.right
            leftMargin: Theme.paddingLarge
            rightMargin: Theme.paddingLarge
        }

        /*Rectangle{
            color: "green"
            id: contactToxIdRect
            //top: 100
            height: 90;
        }

        Rectangle{
            color: "blue"
            id: contactNameRect
            //top: contactToxIdRect.top + ontactToxIdRect.height + Theme.paddingLarge
            width: parent.width
            visible: true
            height: 300;
        }*/
        TextField{
            placeholderText: qsTr("tox ID")
            label: placeholderText
            width: parent.width
            id: contactToxId
            color: "green"
        }
        TextSwitch{
            id: contact_send_msg
            text: qsTr("Send message")
            description: qsTr("Send a invitation message")
            onCheckedChanged: {
                if(contact_send_msg.checked){
                    contactNameRect.visible = true;
                    contactName.readOnly = false;
                }else{
                    contactNameRect.visible = false;
                    contactName.readOnly = true;
                }
            }
            checked: true
        }
        /*Rectangle{
            color: "red"
            id: contact_send_msgRect
            //top: contactNameRect.top + contactNameRect.height + Theme.paddingLarge
            height: 30;
        }*/
        TextField{
            placeholderText: qsTr("Hello, I would like to tox with you!")
            label: qsTr("Invitation message")
            width: parent.width
            id: contactName
        }


/*        TextField{
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
        }*/
    }
}
