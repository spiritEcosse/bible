import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import Qt.labs.settings
import QtQuick.Controls.Material

Item {
    id: root
//    width: 1920
//    height: 1080
    property real itemSizeMedium: 50
    property real paddingMedium: 8
    property real paddingSmall: 10

    property alias title: titleLabel.text
    property alias content: loader
    property bool interactive: false

    property bool expanded
    property real buttonHeight: itemSizeMedium

    // don't use default of horizontalPageMargin, as arrow icons should be closer to the edge of the screen
    property real leftMargin: paddingMedium
    property real rightMargin: paddingMedium

    property bool highlighted

    readonly property int _animationDuration: 200

    width: parent ? parent.width : 0
    height: contentContainer.height + button.contentHeight
//    height: 200

//    GlassItem {
//        id: defaultItem
//        visible: false
//        falloffRadius: 0.1500
//        radius: 0.1500
//    }

    onExpandedChanged: {
        if (expanded) {
            loader.active = true
        }
    }

    // Dynamically animating the Loader height doesn't work consistently depending on how the
    // child content's height and implicitHeight is set, so resize this wrapper Item instead.
    Item {
        id: contentContainer

        anchors.top: button.bottom
        width: parent.width
        height: loader.item && root.expanded ? loader.height : 0
        clip: loader.item && expandAnim.running

        opacity: loader.item && root.expanded ? 1 : 0

        // Don't use Easing.InOutQuad in height behavior; that causes the flickable contentY change
        // to also animate with InOutQuad easing and jump slightly, which looks like a bug.
        Behavior on height {
            id: expandBehavior
            enabled: true

            NumberAnimation {
                id: expandAnim
                duration: root._animationDuration
            }
        }
        Behavior on opacity { NumberAnimation { duration: root._animationDuration } }

        Loader {
            id: loader
            width: parent.width
            active: false
            onStatusChanged: {
                if (status == Loader.Error) {
                    console.log("ExpandingSection: unable to loader source content")
                }
            }
        }
    }


    ItemDelegate { // BackgroundItem
        id: button

        width: parent.width
        height: root.buttonHeight
        highlighted: expanded
        onClicked: {
            expanded = !expanded
        }

        Action {
            id: rightIconAction
            icon.name: "right"
        }

        ToolButton {
            id: iconLeft
            hoverEnabled: false
            action: rightIconAction
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            width: 30
            height: 30
            states: [
                State {
                    name: "expanded"
                    when: root.expanded
                    PropertyChanges { target: iconLeft; rotation: 90 }
                },
                State {
                    name: "collapsed"
                    when: !root.expanded
                }
            ]

            transitions: [
                Transition {
                    to: "expanded"
                    RotationAnimation { target: iconLeft; property: "rotation"; duration: root._animationDuration; direction: RotationAnimation.Clockwise; easing.type: Easing.InOutQuad  }
                },
                Transition {
                    to: "collapsed"
                    RotationAnimation { target: iconLeft; property: "rotation"; duration: root._animationDuration; direction: RotationAnimation.Counterclockwise; easing.type: Easing.InOutQuad  }
                }
            ]
        }

        Label {
            id: titleLabel

//            width: Math.min(implicitWidth, parent.width - anchors.leftMargin)
            anchors {
                left: iconLeft.right
                right: parent.right
                rightMargin: root.rightMargin + paddingSmall
                verticalCenter: parent.verticalCenter
            }
            elide: Label.ElideRight
        }
    }
}
