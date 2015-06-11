/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
import QtQuick 2.0
import Sailfish.Silica 1.0
import "../js/Contacts.js" as Contacts
import harbour.tox 1.0
//import "ContactWrite.qml"

Page {
    id: page
    // To enable PullDownMenu, place our content in a SilicaFlickable
    //SilicaFlickable {
    Component.onCompleted: {
        /*Contacts.initialize()
        var values = [];
        var cts = tox.getContacts();
        for(var i = 0; i< cts.length(); i++){
            contactModel.append( {rowid: i, friendId: cts[i].friendNumber, toxId: cts[i].friendNumber, name: cts[i].getName(), status: cts[i].getStatusMessage(), friend: cts[i]});
        }*/
        var nb_cts = tox.count(), cpt=0, c = null;
        console.log("FirstPage:Component.onCompleted, ", nb_cts, " contacts");
        for(cpt=0; cpt < nb_cts; cpt++){
            c = tox.getContact(cpt);
            contactModel.append({rowid: cpt, friendId: c.friendNumber(), toxId: c.toxId(), name: c.getName(), status: c.getStatusMessage(), friend: c});
            console.log("added contact N°", c.friendNumber(), " Name : ", c.getName(), " Status : ", c.getStatusMessage());
        }

        //console.log("model.modelData", list_page.model.modelData);
    }

    SilicaListView{
        id: list_page
        property Item contextMenu
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Configuration")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
            MenuItem{
                text: qsTr("Delete contacts")
                onClicked: {
                    console.log("delete all: TODO")
                }
            }
            MenuItem{
                text: qsTr("Add contact")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ContactAdd.qml"));
                    console.log("ContactAdd.qml");
                }
            }
        }

        model: ListModel{
            id: contactModel
        }
        //model: tox.getContacts();

        header: SearchField{
            id: contact_search
            width: parent.width
            onTextChanged: {
                console.log("recherche : TODO")
            }
        }
        // Tell SilicaFlickable the height of its content.
        //contentHeight: Column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        delegate: ListItem {
            id: wrapper

            property bool menuOpen: {
                list_page.contextMenu !== null && list_page.contextMenu.parent === wrapper
            }
            contentHeight: {
                menuOpen ? list_page.contextMenu.height+itemColumn.height : itemColumn.height
            }

            width: page.width

            PageHeader {
                title: qsTr("Tox")
            }
            onClicked: {
                console.log("clic!")
            }
            onPressAndHold: {
                if(!list_page.contextMenu){
                    list_page.contextMenu = contextMenuComponent.createObject(list_page)
                }
                list_page.contextMenu.index = index
                list_page.contextMenu.show(wrapper)
                console.log("cliiiiic loooong")
            }
            ListView.onRemove: RemoveAnimation{
                target: wrapper
            }

            Column{
                id: itemColumn
                Label{
                    text : "Contact!"
                }

                Label{
                    id: contactName
                    text: (model.name.length < 1 ? model.toxId : model.name)
                    truncationMode: TruncationMode.Fade
                    color: wrapper.highlighted ? Theme.highlightColor : Theme.primaryColor
                    //font.pixelSize: Theme.fontSizeNormal
                    width: wrapper.ListView.view.width - (2 * Theme.paddingLarge)
                }

                Label {
                    id: contactToxid
                    text: model.status
                    truncationMode: TruncationMode.Fade
                    color: wrapper.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }
        }
        Component {
            id: contextMenuComponent
            ContextMenu {
                property variant index
                MenuItem{
                    text: qsTr("Send message")
                    onClicked: {
                        /*contact_write.message_prepare_to.text = qsTr("To ") + contactModel.get(index).name + "(" + contactModel.get(index).friendId + ")";*/
                        contact_write.messageInit(contactModel.get(index));
                        contact_write.friend_id = contactModel.get(index).friendId;
                        //pageStack.push(Qt.resolvedUrl("ContactWrite.qml"));
                        pageStack.push(contact_write);
                    }
                }
                MenuItem {
                    text: qsTr("Delete")
                    onClicked: {
                        remorse.execute(historyList.contextMenu.parent,
                        qsTr("Deleting"),
                        getDeleteFunction(contactModel, index),
                        2000)
                    }
                }
                MenuItem {
                    text: qsTr("Copy to clipboard")
                    onClicked: {
                        Clipboard.text = contactModel.get(index).toxId
                    }
                }
                function getDeleteFunction(model, index) {
                    // Removing from list destroys the ListElement so we need a copy
                    var valueToDelete = Contacts.copyValue(model.get(index));
                    var f = function() {
                        model.remove(index)
                        Contacts.removeContactValue(valueToDelete)
                    }
                    return f
                }
            }
        }
        RemorseItem{
            id:remorse
        }
        ViewPlaceholder{
            id: placeHolder
            enabled: contactModel.count === 0
            //enabled: list_page.model.count === 0
            text: qsTr("No contacts")
        }
    }
}


