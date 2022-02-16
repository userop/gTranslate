import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.3

import Defined.translator 1.0


Window {
    id: wind;
    width: table.contentWidth+2>5?table.contentWidth + 2:5;
    height: table.contentHeight>5?table.contentHeight:5;
    x: clip.cursor.x;
    y: clip.cursor.y;
    flags: Qt.CustomizeWindowHint;

    onActiveFocusItemChanged: {
        if (!activeFocusItem) wind.visible = false;
    }

    ClipTranslate{
        id: clip;
        onTransTextChanged: {wind.visible = true;}
        onHideChanged: {
            popup.open();
            time.start();
        }
    }

    Timer{
        id: time;
        interval: 1000;
        repeat: false;
        onTriggered: {
            popup.close();
        }
    }

    Text{
        id: table;
        focus: true;
        text: clip.transText;
        Keys.onEscapePressed: {wind.visible = false;}
    }

    Popup {
        id: popup;
        x: 100;
        y: 50;
        margins: 0;
        height: tiptext.contentHeight;
        width: tiptext.contentWidth;
        modal: true;
        Rectangle {
            anchors.fill: parent;
            Text {
                id: tiptext;
                anchors.fill: parent;
                text: clip.hide?"翻译功能关闭，可以使用<Ctrl+Shift+Q>重新开启":"翻译功能开启，可以使用<Ctrl+Shift+Q>关闭";
            }
        }
    }
}
