import QtQuick 2.1
import Sailfish.Silica 1.0
import "../js/Contacts.js" as Contacts
import harbour.tox 1.0
import ".."

Page{
    id: message_prepare
    height: parent.height; width: parent.width
    property int friend_id: -1
    property string friend_name: ""
    /*property alias message_prepare_to: message_prepare_h
    property alias message_news: message_news*/
    property alias message_list: message_model
    property alias message_list_view: message_list_view

    function messageFrom(messg){
        if(messg.messg_from === friend_id){
            message_model.append(messg);
            message_list_view.scrollToBottom();

        }else{
            message_list_view.headerItem.messageSay(messg_from, messg.messg);
        }
    }
    function messageInit(friend){
        friend_id = friend.friendId;
        console.log("friend.name = ", friend.name, ", friend.toxId = ", friend.toxId);
        friend_name = friend.name;
        //console.log(message_list_view.headerItem.messageSay, message_list_view.footerItem.messageFoot, message_list_view.headerItem.message_prepare_h);

        message_list_view.headerItem.messageHead(friend.name.length < 1 ? friend.toxId : friend.name);
        message_list_view.footerItem.messageFoot();
    }

    SilicaListView{
        //VerticalScrollDecorator: {flickable: message_list_view}
        property alias message_prepare_c: message_list_view.headerItem
        id: message_list_view
        model: ListModel{
            id: message_model
        }
        anchors.fill: parent
        header: Column{
            property alias message_prepare_h: message_prepare_h
            property alias message_news: message_news
            visible: true
            id: message_prepare_c
            Label{
                id: message_news
                text: qsTr("Nothing")
            }

            Label{
                id: message_prepare_h
                text: qsTr("Header")
            }
            function messageSay(friend, messg){
                message_news.text = friend + " : \n" + messg
            }
            function messageHead(friend){
                message_prepare_h.text = friend
            }
        }
        footer: Row{
            property alias inputMessage: inputMessage
            width: parent.width
            visible: true
            id: message_prepare_f
            TextField{
                id: inputMessage
                width: parent.width - input_message_btn.width
                placeholderText: qsTr("Hello!")
                maximumLength: 1024
            }
            function messageFoot(){
                inputMessage.text = "" + input_message_btn.width;
            }

            Button{
                id: input_message_btn
                text: qsTr("Send")
                onClicked: {
                    tox.sendMessage(friend_id, "    "+inputMessage.text);
                    message_prepare.messageFrom({messg: inputMessage.text, messg_from: friend_id, messg_direction: 0, messg_date: new Date()})
                    inputMessage.text = "";
                }
            }
        }
        PageHeader {
            title: qsTr("Conversation")
        }
        delegate: ListItem {
            width: message_prepare.width
            contentHeight: message_prepare_wrapper_c.height
            id: message_prepare_wrapper

            Column{
                id: message_prepare_wrapper_c
                /*anchors{
                    rightMargin: (model.messg_direction === 1 ? 0 : 10)
                    leftMargin: (model.messg_direction === 1 ? 10 : 0)
                }*/

                Label{
                    id: message_who
                    text: (model.messg_direction === 1 ? friend_name : qsTr("You :"))
                    color: Theme.highlightColor
                }

                Label{
                    id: message_message
                    text: model.messg
                    color: Theme.primaryColor
                    width: message_prepare_wrapper.ListView.view.width - (2 * Theme.paddingLarge)
                    wrapMode: Text.Wrap
                    /*color: (model.msg_printed < new Date() ? "#004321" : "#432100")*/
                }
            }
        }
    }
}
/*
Dialog {
    id: message_prepare
    height: parent.height; width: parent.width
    property int friend_id: -1
    property alias message_prepare_to: message_prepare_to
    property alias message_news: message_news

    onAccepted: {
        console.log ("Trying to send ", inputMessage.text);
        if(inputMessage.text.length > 0)
        //Contacts.sendMessage(friend_id, inputMessage.text)
        tox.sendMessage(friend_id, inputMessage.text);
    }

    Component.onCompleted: {
        console.log('ContactWrite.qml : Dialog.Component.onCompleted');
        //console.log('page.selected_friend = ', page.selected_friend);
        //message_prepare_to.text = qsTr("To ")+friend_name+"("+friend_id+")"
    }
    DialogHeader{
        id: contact_write_header
        acceptText: qsTr("Send")
    }

    Column{
        id: message_prepare_col
        width: parent.width
        height: childrenRect.width

        anchors {
            left: parent.left;
            right: parent.right
            leftMargin: Theme.paddingLarge
            rightMargin: Theme.paddingLarge
            top: contact_write_header.bottom
        }

        Label{
            id: message_prepare_to
            text: "..."
        }

        TextField{
            placeholderText: qsTr("Hello!")
            label: qsTr("Message")
            width: parent.width
            id: inputMessage
            maximumLength: 1024
        }

        Label{
            id: message_news
            text: qsTr("nothing")
        }
    }
}
*/
