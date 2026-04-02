import QtQuick
import QtQuick.Controls.Basic

Button {
    id: snowflake
    text: qsTr("*")
    width: 50
    height: 50
    hoverEnabled: false
    x: Math.random() * (parent.width - width)
    y: Math.random() * 100
    property int fallDuration: 10000
    readonly property int animationDurationScale: 2

    contentItem: Text {
        text: snowflake.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 56
        font.bold: true
    }

    background: Rectangle {
        color: "transparent"
    }

    PropertyAnimation {
        id: fallAnimation
        target: snowflake
        property: "y"
        from: snowflake.y
        to: root.height - snowflake.height / 2
        duration: snowflake.fallDuration
        running: true

        onStopped: {
            snowflake.destroy();
            root.color = "red"; // Думаю, в общем случае дак делать не хорошо, но в данной задаче работает
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            fallAnimation.duration /= 2;
            snowflake.scale = 1.2;
        }

        onExited: {
            fallAnimation.duration = fallAnimation.duration * 2;
            snowflake.scale = 1.0;
        }

        onClicked: snowflake.clicked()
    }

    onClicked: {
        snowflake.destroy()
    }
}
