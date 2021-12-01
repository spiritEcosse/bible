import QtQuick 2.0
import bible.ModelModule 1.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: pageBooks
    width: parent.width
    height: parent.height
    clip: true

    ModelModule {
        id: modelModulesActive

        Component.onCompleted: {
            modelModulesActive.updateObjectsActive();
            flagUpdateObjectsActive = true;
        }
    }

    Drawer {
        id: drawer
        width: parent.width
        onOpenChanged: {
            if (open && !flagUpdateObjectsDownloaded) {
                modelModuleBooks.updateObjectsDownloaded();
                flagUpdateObjectsDownloaded = true;
            }
        }

        anchors.bottom: panelImage.top
        dock: page.isPortrait ? Dock.Bottom : Dock.Right
        height: parent.height - panelImage.height

        background: SilicaFlickable {
            width: parent.width
            anchors.fill: parent

            PageHeader {
                id: headerDrawer
                title: "Select module"
            }

            SilicaListView {
                id: panelModules
                model: modelModuleBooks
                width: parent.width
                anchors.top: headerDrawer.bottom
                anchors.bottom: parent.bottom
                snapMode: ListView.SnapToItem
                highlightRangeMode: ListView.StrictlyEnforceRange
                clip: true
                VerticalScrollDecorator {}

                delegate: ListItem {
                    width: parent.width
                    y : Theme.paddingLarge
                    contentHeight: child.height + separator.height + Theme.paddingMedium
                    menu: contextMenu
                    onClicked: panelModules.currentIndex = index
                    RemorseItem { id: remorse }
                    function deleteRemorse() {
                        remorseAction("Deleting from device" + abbreviation, function() {

                        })
                    }
                    function activeRemorse() {
                        remorseAction("Activating " + abbreviation, function() {
                            modelModuleBooks.activateModule(model.id);
                            modelModulesActive.updateObjectsActive();
                        })
                    }

                    Label {
                        id: moduleNumber
                        color: Theme.highlightColor
                        width: Math.round(3 * Theme.paddingLarge)
                        text: index + 1 + '. '
                        font.italic: true
                        font.pixelSize: Theme.fontSizeMedium
                        horizontalAlignment: Text.AlignRight
                        anchors {
                            left: parent.left
                        }
                    }

                    Item {
                        id: child
                        width: parent.width
                        height: childrenRect.height
                        anchors.left: moduleNumber.right
                        anchors.right: parent.right
                        anchors.rightMargin: moduleNumber.width

                        Label {
                            id: abr
                            wrapMode: Text.WordWrap
                            truncationMode: TruncationMode.Fade
                            text: abbreviation;
                            font.pixelSize: moduleNumber.font.pixelSize
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        Label {
                            id: des
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: Theme.fontSizeExtraSmall
                            textFormat: Text.RichText
                            wrapMode: Text.WordWrap
                            truncationMode: TruncationMode.Fade
                            text: description
                            anchors {
                                top: abr.bottom
                                left: parent.left
                                right: parent.right
                            }
                        }
                    }

                    ContextMenu {
                        id: contextMenu

                        MenuItem {
                            text: qsTr("Active")
                            onClicked: activeRemorse()
                        }
                        MenuItem {
                            text: qsTr("Copy text");
                            onClicked: Clipboard.text = abbreviation
                        }
                        MenuItem {
                            text: qsTr("Delete")
                            onClicked: deleteRemorse()
                        }
                    }

                    Separator {
                        id: separator
                        anchors {
                            bottom: parent.bottom
                            bottomMargin: -1
                        }

                        width: parent.width
                        color: Theme.primaryColor
                        horizontalAlignment: Qt.AlignHCenter
                    }
                }
            }
        }

        SilicaFlickable {
            id: silicaFlickableBooks
            width: parent.width
            height: parent.height
            anchors.fill: parent

            MouseArea {
                enabled: drawer.opened
                anchors.fill: silicaModules
                onClicked: drawer.open = false
            }

            SilicaFlickable {
                id: silicaModules
                width: parent.width
                height: parent.height
                enabled: !drawer.opened

                Column {
                    id: pageHeader
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter

                    FlippingPageHeader {
                        id: headerMainTextBooks
                        title: qsTrId("Translations")
                        width: parent.width

                        FlippingLabelPatch {
                            id: headerAddTextBooks
                            text: "Новый Русский Перевод (НРП)"
                            width: parent.width
                            fontSize: isPortrait ? Theme.fontSizeExtraSmall : Theme.fontSizeTiny
                            fontFamily: Theme.fontFamilyHeading
                            y: isPortrait ? Theme.itemSizeSmall : Theme.itemSizeExtraSmall
                        }
                    }

                    Item {
                        width: parent.width
                        height: Theme.paddingLarge
                    }
                }

                SilicaListView {
                    id: listActiveModules
                    model: modelModulesActive
                    snapMode: ListView.SnapToItem
                    highlightRangeMode: ListView.StrictlyEnforceRange
                    clip: true
                    width: parent.width
                    anchors.top: pageHeader.bottom
                    height: parent.height - pageHeader.height
                    VerticalScrollDecorator {}

                    delegate: Column {
                        width: parent.width

                        ExpandingSectionPatch {
                            id: modulesActive
                            title: abbreviation + ": " + description;

                            content.sourceComponent: Column {
                                width: parent.width

                                ExpandingSectionGroupPatch {
                                    id: expandingSectionGroup
                                    property bool scrollToBook: false
                                    onCurrentSectionChanged: {
                                        if (currentSection) {
                                            historyModel.testamentIndex = currentIndex
                                        }
                                    }
//                                            property int testamentIndex: historyModel.testamentIndex
//                                            onTestamentIndexChanged: {
//                                                currentIndex = historyModel.testamentIndex
//                                            }

                                    ExpandingSectionBooks {
                                        title: qsTrId("Old testament")
                                        page: page
                //                        model: booksOldTestament
                                    }

                                    ExpandingSectionBooks {
                                        title: qsTrId("New testament")
                                        page: page
                //                        model: booksNewTestament
                                        depth: 1
                                    }
                                }

                                SilicaFlickable {
                                    id: detail
                                    x: Theme.paddingMedium
                                    anchors.top: expandingSectionGroup.bottom
                                    width: parent.width - 2 * Theme.paddingMedium

                                    SectionHeader {
                                        id: detailHeader
                                        text: qsTrId("Detailed info")
                                    }

                                    Label {
                                        anchors.top: detailHeader.bottom
                                        width: parent.width
                                        text: "<p>Святая Библия, Новый Русский Перевод<br/> © Biblica, Inc.®, 2006, 2010, 2012, 2014<br/> Используется с разрешения. Все права защищены по всему миру.</p>"
                                        font.pixelSize: Theme.fontSizeSmall
                                        wrapMode: Text.WordWrap
                                        truncationMode: TruncationMode.Fade
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Image {
        id: panelImage
        height: Theme.itemSizeMedium
        source: "image://theme/graphic-gradient-edge"
        anchors.bottom: parent.bottom
        x: Theme.paddingMedium
        width: parent.width - 2 * x

        Item {
            id: panelHistory
            height: parent.height
            width: parent.width
            anchors.top: parent.top
            enabled: true

            SilicaListView {
                id: sourcePanelHistory
//                model: historyModel
                snapMode: ListView.SnapToItem
                orientation: listHorizontal
                height: parent.height
                width: parent.width - iconBack.width
                currentIndex: 0
                VerticalScrollDecorator {}
//                    visible: slideshow.currentIndex != 1
                clip: true
                anchors.left: parent.left
                anchors.right: iconBack.left
                layoutDirection: {
                    switch (page.orientation) {
                        case Orientation.Portrait: return listLeftToRight;
                        case Orientation.Landscape: return listLeftToRight;
                        case Orientation.PortraitInverted: return listRightToLeft;
                        case Orientation.LandscapeInverted: return listRightToLeft;
                        default: return listLeftToRight;
                    }
                }

                delegate: ListItem {
                    width: index == 0 ? panelImage.width : 2 * Theme.itemSizeLarge
                    height: parent.height
                    focus: true
                    contentHeight: parent.height
                    onClicked: {
                        var changeTestamentIndex = false;
                        if (expandingSectionGroup.currentIndex !== testament_index) {
                            expandingSectionGroup.currentIndex = -1;
                            expandingSectionGroup.currentIndex = testament_index;
                            changeTestamentIndex = true;
                        }

                        if (book_index !== historyModel.bookIndex || changeTestamentIndex) {
                            if (chapter_index === -1) {
                                expandingSectionGroup.scrollToBook = true;
                            }
                            historyModel.copyObject(testament_index, book_index, chapter_index, verse_index);
                        } else if (chapter_index !== historyModel.chapterIndex) {
                            historyModel.copyObject(testament_index, book_index, chapter_index, verse_index);
                        } else if (verse_index !== historyModel.verseIndex) {
                            historyModel.verseIndex = verse_index;
                        }
                    }

                    Text {
                        id: listText
                        horizontalAlignment: {
                            switch (page.orientation) {
                                case Orientation.Portrait: return Text.AlignLeft;
                                case Orientation.Landscape: return Text.AlignLeft;
                                case Orientation.PortraitInverted: return Text.AlignRight;
                                case Orientation.LandscapeInverted: return Text.AlignRight;
                                default: return Text.AlignLeft;
                            }
                        }
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        color: index == 0 ? Theme.highlightColor : Theme.primaryColor
                        text: model.text
//                            text: {
//                                var title = book_short_name;

//                                if (chapter_index !== -1) {
//                                    title += ":" + chapter_index;

//                                    if (verse_index !== -1) {
//                                        title += ":" + verse_index;
//                                    }
//                                }

//                                return title;
//                            }
                        font.pixelSize: Theme.fontSizeLarge
                    }
                }
            }

            IconButton {
                id: iconBack
                icon.source: "image://theme/icon-m-back"
                rotation: 180
                icon.sourceSize: Qt.size(Theme.iconSizeMedium, Theme.iconSizeMedium)
                onClicked: panelHistory.enabled = false
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        states: [
            State {
                name: "panelHistoryActive"
                when: panelHistory.enabled
                PropertyChanges { target: panelHistory; opacity: 1 }
                PropertyChanges { target: panelHistory; height : panelImage.height }
                PropertyChanges { target: expander; opacity: 0 }
                PropertyChanges { target: expander; height : 0 }
            },

            State {
                name: "panelModuleSelect"
                when: expander.enabled
                PropertyChanges { target: panelHistory; opacity: 0 }
                PropertyChanges { target: panelHistory; height : 0 }
                PropertyChanges { target: expander; opacity: 1 }
                PropertyChanges { target: expander; height : panelImage.height }
            }
        ]

        transitions: [
            Transition {
                to: "panelHistoryActive"
                ParallelAnimation {
                    PropertyAnimation { target: panelHistory; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                    PropertyAnimation { target: panelHistory; property: "opacity"; duration: 200; from: 0; to: 1; easing.type: Easing.InOutQuad  }
                    RotationAnimation { target: expander; property: "height"; duration: 200; easing.type: Easing.InOutQuad  }
                    PropertyAnimation { target: expander; property: "opacity"; duration: 200; from: 1; to: 0; easing.type: Easing.InOutQuad  }
                }
            },
            Transition {
                to: "panelModuleSelect"
                ParallelAnimation {
                    PropertyAnimation { target: panelHistory; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                    PropertyAnimation { target: panelHistory; property: "opacity"; duration: 200; from: 1; to: 0; easing.type: Easing.InOutQuad  }
                    PropertyAnimation { target: expander; property: "height"; duration: 200; easing.type: Easing.InOutQuad  }
                    PropertyAnimation { target: expander; property: "opacity"; duration: 200; from: 0; to: 1; easing.type: Easing.InOutQuad  }
                }
            }
        ]

        BackgroundItem {
            id: expander
            width: parent.width
            enabled: !panelHistory.enabled
            onClicked: drawer.open = !drawer.open
            anchors.bottom: parent.bottom

            Label {
                id: expanderText
                anchors.verticalCenter: parent.verticalCenter
            }

            IconButton {
                id: iconHistory
                icon.source: "image://theme/icon-m-history"
                icon.sourceSize: Qt.size(Theme.iconSizeMedium, Theme.iconSizeMedium)
                onClicked: panelHistory.enabled = true
                anchors {
                    right: expanderIcon.left
                    verticalCenter: parent.verticalCenter
                }
            }

            IconButton {
                id: expanderIcon
                icon.source: "image://theme/icon-s-arrow"
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }

            states: [
                State {
                    name: "drawerOpen"
                    when: drawer.opened
                    PropertyChanges { target: expanderText; text: "Close list modules" }
                    PropertyChanges { target: expanderIcon; rotation: 180 }
                },
                State {
                    name: "drawerClose"
                    when: !drawer.opened
                    PropertyChanges { target: expanderText; text: "Open list modules" }
                }
            ]

            transitions: [
                Transition {
                    to: "drawerOpen"
                    ParallelAnimation {
                        PropertyAnimation { target: panelModules; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                        RotationAnimation { target: expanderIcon; property: "rotation"; duration: 200; direction: RotationAnimation.Clockwise; easing.type: Easing.InOutQuad  }
                    }
                },
                Transition {
                    to: "drawerClose"
                    ParallelAnimation {
                        PropertyAnimation { target: panelModules; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                        RotationAnimation { target: expanderIcon; property: "rotation"; duration: 200; direction: RotationAnimation.Counterclockwise; easing.type: Easing.InOutQuad  }
                    }
                }
            ]
        }
    }
}
