import QtQuick 2.0
import Sailfish.Silica 1.0
//import bible.BooksModel 1.0

ExpandingSectionPatch {
    id: expandingSection
//    property BooksModel model
    property Page page
    property int depth: 0

    content.sourceComponent: Column {
        width: parent.width

        ExpandingSectionGroupPatch {
            id: expandGroupBooks
            depth: 1
            property int bookIndex: historyModel.bookIndex
            onBookIndexChanged: {
                if (load && expandingSection.expanded && parentGroup.currentIndex === expandingSection.depth) {
                    currentIndex = historyModel.bookIndex;

                    if (historyModel.chapterIndex != -1) {
                        model.currentChapter = historyModel.chapterIndex
                    } else if (parentGroup.scrollToBook) {
//                      Need fix. It is not clear why the scroll works only through timerInterval.
                        timerAutoScroll.restart()
                    }
                }
            }
            parentGroup: expandingSection._group
            onCurrentSectionChanged: {
                if (expandingSection.expanded && currentSection) {
                    expandingSection.model.currentBook = currentSection.obj.book_number
                    timerOnCurrentSectionChangedBooks.restart()
                }
            }
            Component.onCompleted: {
                if (expandingSection.expanded && parentGroup.currentIndex === expandingSection.depth) {
                    currentIndex = historyModel.bookIndex;

                    if (historyModel.chapterIndex != -1) {
                        model.currentChapter = historyModel.chapterIndex
                    } else {
                        _updateFlickableContentY(null, null);
                    }
                }
                load = true;
            }
            Timer {
                id: timerOnCurrentSectionChangedBooks
                interval: timerInterval
                repeat: false
                onTriggered: {
                    if (expandGroupBooks.currentSection) {
                        historyModel.testamentIndex = expandGroupBooks.parentGroup.currentIndex
                        historyModel.bookIndex = expandGroupBooks.currentIndex
                        historyModel.bookShortName = expandGroupBooks.currentSection.obj.short_name
                    }
                }
            }
            Timer {
                id: timerAutoScroll
                interval: timerInterval
                repeat: false
                onTriggered: {
                    expandGroupBooks._updateFlickableContentY(null, null);
                    expandGroupBooks.parentGroup.scrollToBook = false;
                }
            }

            Repeater {
                model: expandingSection.model

                ExpandingSectionPatch {
                    id: expandingSectionBooks
                    title: long_name
                    obj: model

                    content.sourceComponent: Column {
                        width: parent.width

                        ExpandingSectionGroupPatch {
                            id: expandGroupChapter
                            parentGroup: expandGroupBooks
                            depth: expandingSection.depth + 2
                            toTop: true
                            property int chapterIndex: historyModel.chapterIndex
                            onChapterIndexChanged: {
                                if (load) {
                                    auto();
                                }
                            }
                            onCurrentSectionChanged: {
                                if (expandingSectionBooks.expanded && currentSection) {
                                    expandingSection.model.currentChapter = currentIndex + 1
                                    timerOnCurrentSectionChangedChapter.restart();
                                }
                            }
                            function auto() {
                                if (expandingSectionBooks.expanded &&
                                        parentGroup.parentGroup.currentIndex === expandingSection.depth &&
                                        parentGroup.currentIndex === historyModel.bookIndex) {
                                    currentIndex = historyModel.chapterIndex >= 1 ? historyModel.chapterIndex - 1 : -1;
                                }
                            }
                            Component.onCompleted: {
                                auto();
                                load = true;
                            }
                            Timer {
                                id: timerOnCurrentSectionChangedChapter
                                interval: timerInterval
                                repeat: false
                                onTriggered: {
                                    expandGroupChapter._updateFlickableContentY(null, null);
                                    historyModel.chapterIndex = expandGroupChapter.currentIndex >= 0 ? expandGroupChapter.currentIndex + 1 : -1
                                }
                            }

                            Repeater {
                                model: chapters

                                ExpandingSectionPatch {
                                    id: expandSectionChapter
                                    title: index + 1
                                    content.height: Screen.height - panel.height - buttonHeight

                                    content.sourceComponent: SilicaListView {
                                        id: listVerse
                                        focus: true
                                        spacing: Theme.paddingMedium
                                        model: expandSectionChapter.expanded ? expandingSection.model.currentVerses : 0
                                        onModelChanged: {
                                            currentIndex = 0
                                        }

                                        clip: true
                                        snapMode: ListView.SnapToItem
                                        highlightRangeMode: ListView.StrictlyEnforceRange
                                        onMovementEnded: {
                                            timerOnVerseIndexChanged.restart()
                                        }
                                        property int verseIndex: historyModel.verseIndex
                                        onVerseIndexChanged: {
                                            if (expandSectionChapter.expanded) {
                                                currentIndex = historyModel.verseIndex >= 1 ? historyModel.verseIndex - 1 : 0
                                            }
                                        }
                                        currentIndex: historyModel.verseIndex
                                        Component.onCompleted: {
                                            currentIndex = historyModel.verseIndex >= 1 ? historyModel.verseIndex - 1 : 0
                                        }

                                        Timer {
                                            id: timerOnVerseIndexChanged
                                            interval: timerInterval
                                            repeat: false
                                            onTriggered: {
                                                historyModel.verseIndex = listVerse.currentIndex + 1
                                            }
                                        }

                                        VerticalScrollDecorator {}

                                        delegate: ListItem {
                                            id: listItem
                                            menu: contextMenu
                                            contentHeight: child.height
                                            width: parent.width
                                            onClicked: {
                                                listVerse.currentIndex = index
                                                timerOnVerseIndexChanged.restart()
                                            }

                                            Item {
                                                id: child
                                                height: childrenRect.height
                                                x: Theme.paddingMedium
                                                width: parent.width - 2*Theme.paddingMedium

                                                Label {
                                                    id: verseNumber
                                                    color: Theme.highlightColor
                                                    width: Theme.horizontalPageMargin
                                                    text: verse_number + '.'
                                                    font.italic: true
                                                    font.pixelSize: Theme.fontSizeSmall
                                                    horizontalAlignment: Text.AlignRight
                                                    anchors {
                                                        left: parent.left
                                                    }
                                                }
                                                Text {
                                                    wrapMode: Text.WordWrap
                                                    color: listVerse.currentIndex == index ? verseNumber.color : Theme.primaryColor
                                                    text: {
                                                        var regex = /<f>\[\d+\]<\/f>/g;
                                                        var matches = verse_text.match(regex);
                                                        var verse_text_copy = verse_text;

                                                        if (matches) {
                                                            for (var pos = 0, href, content; pos < matches.length; pos++ ) {
                                                                content = matches[pos].match(/\[\d+\]/)[0];
                                                                href = "<a href='" + content + "'>  " + content + "  </a>";
                                                                verse_text_copy = verse_text_copy.replace(matches[pos], href);
                                                            }
                                                        }

                                                        return verse_text_copy
                                                    }
                                                    linkColor: verseNumber.color
                                                    font.pixelSize: Theme.fontSizeSmall
                                                    onLinkActivated: {
                                                        pageStack.push(
                                                                    comments,
                                                                    {
                                                                        "book": expandingSection.model.currentBook,
                                                                        "chapter": expandingSection.model.currentChapter,
                                                                        "verse": index + 1,
                                                                        "marker": link,
                                                                        "historyModel": historyModel
                                                                    })
                                                    }
                                                    textFormat: Text.StyledText
                                                    width: parent.width
                                                    anchors {
                                                        leftMargin: Theme.paddingMedium
                                                        left: verseNumber.right
                                                        right: parent.right
                                                        top: parent.top
                                                    }
                                                }
                                            }

                                            ContextMenu {
                                                id: contextMenu
                                                enabled: expandSectionChapter.enabled

                                                MenuItem {
                                                    text: qsTr("Copy text");
                                                    onClicked: Clipboard.text = model.text
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
        }
    }
}
