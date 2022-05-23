import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings
import "components"

ApplicationWindow {
    width: 1917
    height: 500 // 1080
    id: mainwindow
    visible: true
    title: "BibleMac"
    required property var builtInStyles
    readonly property int _animationDuration: 200
    property real paddingLarge: 50

    Settings {
        id: settings
        property string style
    }

    TabBar {
        id: bar
        width: parent.width
        currentIndex: moduleview.currentIndex
        onCurrentIndexChanged: moduleview.currentIndex = currentIndex

        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }

//    StackView {
    SwipeView {
        id: moduleview
//        width: 150
//        height: 150
        clip: true
        anchors.topMargin: bar.height
        anchors.fill: parent
        Component.onCompleted: moduleview.currentIndex = 2
        orientation: Qt.Horizontal
        layer.smooth: true

        contentItem: ListView {
            id: listView
            onFlickStarted: {
                if (moduleview.currentIndex === 2) {
                    fl.interactive = false
                }
            }
//            highlight: HighlightBar {}
//            highlightFollowsCurrentItem: false
            onFlickEnded: {
                if (moduleview.currentIndex !== 2) {
                    fl.interactive = true
                }
            }

            model: moduleview.contentModel
            currentIndex: moduleview.currentIndex
            interactive: moduleview.interactive

            spacing: moduleview.spacing
            orientation: moduleview.orientation
            snapMode: ListView.SnapOneItem
            boundsBehavior: Flickable.DragOverBounds

            highlightRangeMode: ListView.StrictlyEnforceRange
            preferredHighlightBegin: 0
            preferredHighlightEnd: 0
            highlightMoveDuration: 250

            maximumFlickVelocity: 4 * width
        }

        Item {
        }

        Item {
            height: 1200

            SplitView {
                anchors.fill: parent
                orientation: Qt.Horizontal
                contentHeight: 1200
                height: 1200

                Rectangle {
                    implicitWidth: parent.width / 4
//                    SplitView.maximumWidth: 400
                    color: "lightblue"
                    SplitView.preferredHeight: 300
                    Label {
                        text: "View 1"
                        anchors.centerIn: parent
                    }
                }
                Rectangle {
                    implicitWidth: parent.width / 4
                    id: centerItem
                    SplitView.preferredHeight: 300
                    SplitView.minimumWidth: 50
//                    SplitView.fillWidthChanged: true
                    color: "lightgray"
                    Label {
                        text: "View 2"
                        anchors.centerIn: parent
                    }
                }
                Rectangle {
                    implicitWidth: parent.width / 4
                    SplitView.minimumWidth: 50
//                    implicitWidth: 200
                    SplitView.preferredHeight: 300
//                    SplitView.fillWidth: true
                    color: "lightgreen"
                    Label {
                        text: "View 3"
                        anchors.centerIn: parent
                    }
                }
                Rectangle {
                    implicitWidth: parent.width / 4
                    SplitView.minimumWidth: 50
//                    implicitWidth: 200
                    SplitView.preferredHeight: 300
//                    SplitView.fillWidth: false
                    color: "grey"
                    Label {
                        text: "View 4"
                        anchors.centerIn: parent
                    }
                }
            }
        }

        Page {
            id: modules
            property int model: 22
            property int heightColumn: 50
            property int minWidthColumn: 50
            property int widthColumn: 1440 / model
            property int flContentHeight: modules.model * (modules.heightColumn + 6) - 6 + parent.height - 55
            property int expandedHeight: mainwindow.height / 2

            Flickable {
                id: fl
                width: parent.width
                contentHeight: modules.flContentHeight
                height: parent.height
//                height: childrenRect.height ? childrenRect.height : parent.height
                clip: true
//                anchors.fill: parent
                flickableDirection: Flickable.VerticalFlick
                ScrollBar.vertical: ScrollBar {
//                    active: true
                }
//                ScrollIndicator.vertical: ScrollIndicator { }

            SplitView {
                        id: split1
                        orientation: Qt.Vertical
                        width: parent.width
                        height: parent.height
//                        height: childrenRect.height
//                        contentHeight: childrenRect.height
//                        onContentHeightChanged: {
//                            console.log(contentHeight)
//                        }
                        SplitView.fillHeight: true

                        Repeater {
                            id: repeaterVert
                            model: modules.model

                            SplitView {
                                id: split
                                orientation: Qt.Horizontal
                                SplitView.preferredHeight: modules.heightColumn
                                SplitView.minimumHeight: modules.heightColumn
                                width: parent.width
                                property int countExpanded: 0
                                property bool expanded: countExpanded === 0 ? false : true
                                onExpandedChanged: {
                                    if (expanded) {
                                        fl.contentHeight += modules.expandedHeight
                                        SplitView.preferredHeight += modules.expandedHeight
                                    } else {
                                        fl.contentHeight -= modules.expandedHeight
                                        SplitView.preferredHeight = modules.heightColumn
                                    }
                                }

                                property real repeaterVerticalIndex: index + 1
                                readonly property real widthColumn: 1440 / rep.model
                                property bool leftDirection: false
                                Behavior on SplitView.preferredHeight {
                                    NumberAnimation {
                                        duration: mainwindow._animationDuration
                                    }
                                }
                                property real moveX
                                handle: Rectangle {
                                    implicitWidth: 4
                                    onXChanged: {
                                        if (SplitHandle.pressed) {
                                            leftDirection = split.moveX > x
                                            split.moveX = x
                                        }
                                    }
                                    implicitHeight: 4
                                    property bool pressed: SplitHandle.pressed
                                    onPressedChanged: split.moveX = x
                                    color: SplitHandle.pressed ? "#81e889"
                                        : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")

//                                    containmentMask: Item {
//                                        x: -width / 2
//                                        width: 64
//                                        height: split.height
//                                    }
                                }

                                Repeater {
                                    id: rep
                                    model: 5
                                    property int nextCollapseLeft: 0
                                    property int nextCollapseRight: 4

                                    ExpandingSection {
                                        id: group
                                        clip: true
                                        property real repeaterHorizontalIndex: index + 1
                                        title: "View " + repeaterVerticalIndex + repeaterHorizontalIndex
                                        SplitView.preferredWidth: split.widthColumn
                                        SplitView.minimumWidth: modules.minWidthColumn
                                        width : split.widthColumn
                                        onExpandedChanged: split.countExpanded += expanded ? 1 : -1
                                        onWidthChanged: {
                                            if (split.resizing) {
                                                if (leftDirection && index === rep.nextCollapseLeft) {
                                                    if (width === 50) {
                                                        if (rep.itemAt(rep.nextCollapseLeft)) {
                                                            rep.itemAt(rep.nextCollapseLeft).SplitView.fillWidth = false;
                                                            rep.itemAt(rep.nextCollapseLeft).SplitView.preferredWidth = 50;
                                                            rep.itemAt(rep.nextCollapseLeft).width = 50;
                                                            if (rep.itemAt(rep.nextCollapseLeft + 1)) {
                                                                rep.nextCollapseLeft += 1;
                                                            }
                                                        }
                                                    }
                                                } else if (!leftDirection && index === rep.nextCollapseRight) {
                                                    if (rep.itemAt(rep.nextCollapseRight) && width === 50) {
                                                        rep.itemAt(rep.nextCollapseRight).SplitView.preferredWidth = 50;
                                                        rep.itemAt(rep.nextCollapseRight).width = 50;
                                                        if (rep.itemAt(rep.nextCollapseRight - 1)) {
                                                            rep.nextCollapseRight -= 1;
                                                            rep.itemAt(rep.nextCollapseRight).SplitView.fillWidth = true;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        content.sourceComponent: Column {

                                            ListView {
                                                width: parent.width
                                                height: split.height
                                                clip: true
                                                model: 10
                                                spacing: 0
                                                snapMode: ListView.SnapToItem
                                                highlightRangeMode: ListView.StrictlyEnforceRange

                                                delegate: ItemDelegate {
                                                    property var myModule: QtObject {
                                                        property bool download: false
                                                    }
//                                                    readonly property bool isSelected: isSelecting && selectedModules.some(function(existingModuleId) {
//                                                        return existingModuleId.moduleId === model.id
//                                                    });
//                                                    readonly property bool isDownloaded: downloadedModules.some(function(existingModuleId) {
//                                                        return existingModuleId.moduleId === model.id
//                                                    });
                                                    width: parent ? parent.width : 0
                                                    onClicked: {
                                                        if (!hid && isSelecting) {
                                                            appendOrRemoveModuleSelection({"abbreviation": abbreviation, "selecting": !isSelected, "moduleId": model.id, "groupId": group.groupId, "module": myModule})
                                                        } else if (!isSelecting) {
                                                            listModules.currentIndex = index
                                                        }
                                                    }
                                                    function downloadRemorse() {
                                                        remorseAction("Downloading " + abbreviation, function() {
                                                            myModule.download = true
                                                        })
                                                    }
//                                                    Item {
//                                                        id: child
//                                                        height: childrenRect.height
//                                                        width: parent.width

                                                    Label {
                                                        id: moduleNumber
                                                        height: parent.height
//                                                        color: Theme.highlightColor
//                                                        width: Math.round(3 * paddingLarge)
                                                        width: parent.width
                                                        text: index + 1 + '. View View View View View View View View View View View '
                                                        font.italic: true
//                                                        font.pixelSize: Theme.fontSizeMedium
//                                                        horizontalAlignment: Text.AlignRight
//                                                        verticalAlignment: Text.verticalCenter
//                                                        anchors.verticalCenter: parent.verticalCenter
//                                                        anchors.left: parent.left
                                                        verticalAlignment: Text.AlignVCenter
//                                                        anchors.fill: parent
                                                        elide: Label.ElideRight
//                                                    }
                                                    }
//                                                    GlassItemLeft {
//                                                        visible: listItem.isDownloaded
//                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
//                        }
            }
        }

    }
}
