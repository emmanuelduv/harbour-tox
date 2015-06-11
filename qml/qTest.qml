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
import "pages"
import harbour.tox 1.0

ApplicationWindow
{
    ContactWrite {
        id: contact_write
    }
    ToxWrapper{
        id: tox;
    }
    allowedOrientations: Orientation.All
    Component{
        id: mainPage
        FirstPage{}
    }

    Component.onDestruction: {
        console.log("onDestruction");
        tox.quitter();
    }

    function newMessage(friendId, messg, type){
        console.log(friendId, " sent ", messg);
        contact_write.messageFrom({messg_from: friendId, messg: messg, messg_direction: 1, messg_date: new Date(), messg_printed: new Date(), messg_type: type})
        /*if(friendId === contact_write.friend_id){
            contact_write.message_list.append({messg: messg, direction_messg: 1, date_messg: new Date(), date_printed: new Date});
        }else{
            contact_write.message_news.text = friendId + qsTr(" says ") + messg;
        }*/
    }

    function updateMessages(){
        console.log("?");
    }

    initialPage: Component { Page { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    Component.onCompleted: {
        console.log("ApplicationWindow.Component.onCompleted");
        //tox.parent = this;
        //tox.setSavePath(".tox/save");
        tox.init();
        tox.friendMessageReceived.connect(newMessage);
        //tox.messagesToUpdate.connect(updateMessages);
        tox.start();
        pageStack.replace(mainPage);
    }
}


