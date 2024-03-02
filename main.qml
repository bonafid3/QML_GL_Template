import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

import GraphicsLayer 1.0
import "qrc:/"

Item {
	id: root

	focus: true

	Keys.onPressed:
	{
		graphicsLayer.onKeyPressed(event.key, event.modifiers)
	}

	Keys.onReleased:
	{
		graphicsLayer.onKeyReleased(event.key, event.modifiers)
	}

	/*
	Timer {
		id: tmr
		running: true
		repeat: true
		property real cnt: 0.0
		interval: 10
		onTriggered: cnt += 0.1
	}*/

	GraphicsLayer {
		id: graphicsLayer
		anchors.fill: parent

		MouseArea {
			anchors.fill: parent
			hoverEnabled: true
			acceptedButtons: Qt.LeftButton | Qt.RightButton
			onPressed: graphicsLayer.onMousePressed(mouse.x, mouse.y, mouse.button)
			onReleased: graphicsLayer.onMouseReleased(mouse.x, mouse.x, mouse.button)
			onMouseXChanged: graphicsLayer.onMousePositionChanged(mouse.x, mouse.y)
			onMouseYChanged: graphicsLayer.onMousePositionChanged(mouse.x, mouse.y)

			onClicked: graphicsLayer.forceActiveFocus()

			onWheel: function(wheelEvent){
				graphicsLayer.onWheeled(wheelEvent)
			}
		}
	}

	Rectangle {
		anchors.left: parent.left
		anchors.right: parent.right
        height: 30
		color: "orange"
	}

	Text {
        text: "Sample text"
	}

	Connections {
		target: graphicsLayer
		function onWarningMessageChanged() {
			console.log("warning message changed")
			var component = Qt.createComponent("MyRect.qml");
			var object = component.createObject(root);
			object.text = graphicsLayer.warningMessage
		}
	}
}
