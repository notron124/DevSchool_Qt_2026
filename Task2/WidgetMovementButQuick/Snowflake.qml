import QtQuick
import QtQuick.Controls.Basic

Button {
    id: snowflake
    text: qsTr("*")
    width: 50
    height: 50
    hoverEnabled: false
    y: Math.random() * 100
    readonly property int rotationAnimationDuration: 1000 + Math.random() * 1000
    property real fallSpeed: 1.0 + Math.random() * 2.0
    readonly property int animationDurationScale: 2

    contentItem: Text {
        text: snowflake.text
        color: "#ffffff"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 56
        font.bold: true
    }

    background: Rectangle {
        color: "transparent"
    }

    RotationAnimation on rotation {
        from: 0
        to: 360
        duration: 2000
        loops: Animation.Infinite
    }

    Component.onCompleted: {
        x = Math.random() * (parent.width - width);
    }

    // Пробовал через PropertyAnimation, с ним было
    // не очень удобно и не очевидно контроллировать скорость падения
    Timer {
        id: moveTimer
        interval: 50
        repeat: true
        running: true
        onTriggered: {
            var maxX = parent.parent.width - width;
            var maxY = parent.parent.height - height;

            y += fallSpeed;

            if (x >= maxX) {
                snowflake.destroy();
                return;
            }

            if (y < maxY) {
                return;
            }

            // Думаю, использование идентификатора родителя в общем случае - плохо,
            // но в текущем сценарии работает.
            root.color = "red";
            root.title = "You LOSE!";
            snowflake.destroy();
        }
    }


    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            snowflake.fallSpeed *= snowflake.animationDurationScale;
            snowflake.scale = 1.2;
        }

        onExited: {
            snowflake.fallSpeed /= snowflake.animationDurationScale;
            snowflake.scale = 1.0;
        }

        onClicked: snowflake.clicked()
    }

    onClicked: {
        snowflake.destroy()
    }
}
