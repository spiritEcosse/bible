import QtQuick 2.0
import Sailfish.Silica 1.0
import bible.BooksModel 1.0
import bible.ModulesModel 1.0
import bible.HistoryModel 1.0

Pages {
    id: page
    property int listLeftToRight: ListView.LeftToRight;
    property int listRightToLeft: ListView.RightToLeft;
    property int listHorizontal: ListView.Horizontal;
    property int listVertical: ListView.Vertical;
    property int timerInterval: 400
    property int bookNumber
    property int chapterIndex
    property int verseIndex

    HistoryModel {
        id: historyModel
    }

    BooksModel {
        id: booksOldTestament

        Component.onCompleted: {
            booksOldTestament.oldTestament()
        }
    }

    BooksModel {
        id: booksNewTestament

        Component.onCompleted: {
            booksNewTestament.newTestament()
        }
    }

    ModulesModel {
        id: modulesModel
    }

    VisualItemModel {
        id: visualModel

        SilicaFlickable {
            id: silicaFlickableBooks
            width: parent.width
            contentHeight: columnBooks.height
            height: parent.height
            clip: true
            VerticalScrollDecorator {}

            Column {
                id: columnBooks
                width: parent.width

                PageHeader {
                    id: pageHeader
                    title: "Новый Русский Перевод (НРП)"
                }

                ExpandingSectionGroupPatch {
                    id: expandingSectionGroup
                    property bool scrollToBook: false
                    onCurrentSectionChanged: {
                        if (currentSection) {
                            historyModel.testamentIndex = currentIndex
                        }
                    }
                    property int testamentIndex: historyModel.testamentIndex
                    onTestamentIndexChanged: {
                        currentIndex = historyModel.testamentIndex
                    }

                    ExpandingSectionBooks {
                        title: qsTrId("Old testament")
                        page: page
                        model: booksOldTestament
                    }

                    ExpandingSectionBooks {
                        title: qsTrId("New testament")
                        page: page
                        model: booksNewTestament
                        depth: 1
                    }
                }

                Column {
                    id: detail
                    x: Theme.paddingMedium
                    width: parent.width - 2 * Theme.paddingMedium

                    SectionHeader {
                        text: qsTrId("Detailed info")
                    }

                    Label {
                        width: parent.width
                        text: "<p>Святая Библия, Новый Русский Перевод<br/>
                        © Biblica, Inc.®, 2006, 2010, 2012, 2014<br/>
                        Используется с разрешения. Все права защищены по всему миру.</p>"
                        font.pixelSize: Theme.fontSizeSmall
                        wrapMode: Text.WordWrap
                        truncationMode: TruncationMode.Fade
                    }
                }
            }
        }

        SilicaFlickable {
            id: silicaFlickableModules
            width: parent.width
            contentHeight: columnBooks.height
            height: parent.height
            VerticalScrollDecorator {}
        }

        SilicaFlickable {
            width: parent.width
            height: parent.height
            contentHeight: columnA.height + Theme.paddingLarge

            VerticalScrollDecorator {}

            Column {
                id: columnA
                spacing: Theme.paddingLarge
                width: parent.width

                PageHeader { title: "Panels and sections" }

                SectionHeader {
                    text: "Expanding sections"
                }

                ExpandingSectionGroup {
                    currentIndex: 0

                    Repeater {
                        model: 5

                        ExpandingSection {
                            id: section

                            property int sectionIndex: model.index
                            title: "Section " + (model.index + 1)

                            content.sourceComponent: Column {
                                width: section.width

                                Repeater {
                                    model: (section.sectionIndex + 1) * 2

                                    TextSwitch {
                                        text: "Option " + (model.index + 1)
                                        onClicked: {
                                            console.log(index)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    SilicaFlickable {
        id: silicaFlickableMenu
        anchors.bottom: parent.bottom
        width: parent.width
        height: slideshow.height + panel.height
        contentHeight: slideshow.height + panel.height

        SlideshowView {
            id: slideshow
            width: parent.width
            itemWidth: parent.width
            height: page.height - panel.height
            model: visualModel
            anchors.bottomMargin: panel.height
        }

        PushUpMenu {
            id: pushUpMenu

            MenuItem {
                text: "Return to Top"
                onClicked: silicaFlickableBooks.scrollToTop()
            }

            MenuItem {
                text: qsTrId("Update modules")
                onClicked: modulesModel.updateModules()
            }
        }

        Image {
            width: parent.width
            height: panel.height
            source: "image://theme/graphic-gradient-edge"
            anchors.top: slideshow.bottom
        }

        SilicaListView {
            id: panel
            height: Theme.itemSizeMedium
            model: historyModel
            snapMode: ListView.SnapToItem
            orientation: listHorizontal
            anchors.bottom: parent.bottom
            currentIndex: 0
            clip: true
            x: Theme.paddingMedium
            width: parent.width - 2 * Theme.paddingMedium
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
                width: index == 0 ? panel.width : 2 * Theme.itemSizeLarge
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
                    text: {
                        var title = book_short_name;

                        if (chapter_index !== -1) {
                            title += ":" + chapter_index;

                            if (verse_index !== -1) {
                                title += ":" + verse_index;
                            }
                        }

                        return title;
                    }
                    font.pixelSize: Theme.fontSizeLarge
                }
            }
        }
    }

    Component.onCompleted: {
        expandingSectionGroup.currentIndex = historyModel.testamentIndex;
    }
}
