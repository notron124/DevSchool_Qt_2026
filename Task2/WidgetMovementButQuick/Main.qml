import QtQuick
import QtQuick.Controls

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("WidgetMovementButQuick")
    color: "lightblue"

    Timer {
        id: spawnTimer
        interval: 100 + Math.random() * 900
        running: true
        onTriggered: {
            var component = Qt.createComponent("Snowflake.qml")

            spawnTimer.interval = 100 + Math.random() * 900;
            spawnTimer.start();

            if (component.status !== Component.Ready) {
                console.error("Error during component creation:", component.errorString());
                return;
            }

            var snowflake = component.createObject(root);

            if (snowflake === null) {
                console.error("Error during snowflake creation");
            }
        }
    }
}

