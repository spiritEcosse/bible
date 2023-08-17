import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: silicaFlickableSearch
    width: parent.width
    height: parent.height
    clip: true

    Column {
        id: pageHeader
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter

        FlippingPageHeader {
            id: headerMainTextBooks
            title: qsTrId("Translation search")
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

    SilicaFlickable {
        width: parent.width
        anchors.top: pageHeader.bottom
        clip: true
        height: parent.height - pageHeader.height - panel.height

        SilicaListView {
            id: listActiveModules
            model: modelModuleBooks
            height: parent.height
            snapMode: ListView.SnapToItem
            highlightRangeMode: ListView.StrictlyEnforceRange
            clip: true
            width: parent.width
            VerticalScrollDecorator {}

            delegate: Column {
                width: parent.width
//                contentHeight: modulesActive.height

                ExpandingSectionPatch {
                    id: modulesActive
                    title: abbreviation + ": " + description;
                    property int moduleId: model.id
                    clip: true
                    width: parent.width
                    onExpandedChanged: {
                        if (expanded) {
                            listActiveModules.currentIndex = index;
                        }
                    }

                    content.sourceComponent: SilicaFlickable {
                        width: parent.width
                        height: Screen.height - pageHeader.height - 2  * modulesActive.buttonHeight - panel.height

                        SilicaListView {
                            id: listBooks
                            model: foundBooks
                            width: parent.width
                            height: parent.height
                            clip: true
                            property bool expanded : modulesActive.expanded
                            property string search: searchField.text
                            function searchVersesByText() {
                                if (expanded && search) {
                                    model.searchVersesByText(search);
                                }
                            }
                            onSearchChanged: searchVersesByText()
                            onExpandedChanged: searchVersesByText()
                            visible: count
                            VerticalScrollDecorator {}
                            snapMode: ListView.SnapToItem
                            highlightRangeMode: ListView.StrictlyEnforceRange

                            delegate: ListItem {
                                id: columnBook
                                width: parent.width
                                contentHeight: expandingSectionBook.height + Theme.paddingMedium

                                ExpandingSectionPatch {
                                    id: expandingSectionBook
                                    title: long_name.trim()
                                    width: parent.width
                                    onExpandedChanged: {
                                        if (expanded) {
                                            listBooks.currentIndex = index
                                        }
                                    }

                                    content.sourceComponent: SilicaFlickable {
                                        width: parent.width
                                        height: Screen.height - pageHeader.height - 4  * modulesActive.buttonHeight - panel.height

                                        SilicaListView {
                                            id: listVerses
                                            model: foundVerses
                                            width: parent.width
                                            height: parent.height
                                            spacing: 0
                                            clip: true
                                            VerticalScrollDecorator {}
                                            snapMode: ListView.SnapToItem
                                            highlightRangeMode: ListView.StrictlyEnforceRange

                                            delegate: ListItem {
                                                id: listItem
                                                contentHeight: childVerse.height + separator.height
                                                y : Theme.paddingLarge
                                                width: parent.width
                                                menu: contextMenu
                                                onClicked: {
                                                    listVerses.currentIndex = index;
                                                }

                                                Label {
                                                    id: verseNumber
                                                    color: Theme.highlightColor
                                                    width: Math.round(9 * Theme.paddingMedium)
                                                    text: chapter + ":" + verse + '. '
                                                    font.italic: true
                                                    font.pixelSize: Theme.fontSizeMedium
                                                    horizontalAlignment: Text.AlignRight
                                                    anchors {
                                                        left: parent.left
                                                    }
                                                }
                                                Item {
                                                    id: childVerse
                                                    height: childrenRect.height
                                                    width: parent.width - Theme.paddingMedium
                                                    anchors {
                                                        left: verseNumber.right
                                                        right: parent.right
                                                        rightMargin: Theme.paddingMedium
                                                    }

                                                    Text {
                                                        wrapMode: Text.WordWrap
                                                        color: Theme.primaryColor
                                                        text: {
                                                            var result = removeAllTags(content_text);
                                                            return Theme.highlightText(
                                                                  result.substr(0, result.length),
                                                                  searchField.text,
                                                                  Theme.highlightColor);
                                                        }
                                                        linkColor: verseNumber.color
                                                        font.pixelSize: Theme.fontSizeSmall
                                                        textFormat: Text.StyledText
                                                        width: parent.width
                                                    }
                                                }

                                                ContextMenu {
                                                    id: contextMenu
                                                    MenuItem {
                                                        text: qsTr("Go to");
                                                        onClicked: {
                                                            modelModulesActive.activateModule(modulesActive.moduleId);
                                                            modelModulesActive.updateObjectsActive();
                                                            modelRecord.createRecord(
                                                                       book_number,
                                                                       book_number / 10 - 1,
                                                                       chapter - 1,
                                                                       verse -1
                                                                       );
                                                            slideshow.currentIndex = 0;
                                                        }
                                                    }
                                                    MenuItem {
                                                        text: qsTr("Copy text");
                                                        onClicked: Clipboard.text = model.text
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
                                }
                            }
                        }

                        Label {
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeLarge
                            horizontalAlignment: Text.Center
                            visible: !listBooks.visible
                            text: qsTrId("Here will be the result of search.")
                            width: parent.width
                            truncationMode: TruncationMode.Fade
                        }
                    }
                }
            }
        }
    }

    Image {
        id: panel
        width: parent.width
        height: Theme.itemSizeMedium
        source: "image://theme/graphic-gradient-edge"
        anchors.bottom: parent.bottom

        SearchField {
            id: searchField
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
            anchors.right: parent.right
            anchors.left: parent.left
        }
    }
}
