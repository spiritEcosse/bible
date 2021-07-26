import QtQuick 2.0
import Sailfish.Silica 1.0
//import bible.BooksModel 1.0
import bible.ModelGroupModules 1.0
import bible.ModelModuleDownload 1.0
import bible.ModelModule 1.0
//import bible.HistoryModel 1.0
import "../components"

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
    property var selectedModules: []
    property var downloadedModules: []
    property var downloadModulesLater: []
    property bool isSelecting: selectedModules.length

    ModelModuleDownload {
        id: moduleDownload

        Component.onCompleted: {
            var selectionArray = [];
            var downloadedArray = [];

            for (var i = 0; i < moduleDownload.rowCount(); i++) {
                var module_id = moduleDownload.data(moduleDownload.index(i, 0), 0);
                var group_id = moduleDownload.data(moduleDownload.index(i, 0), 1);
                var selecting = moduleDownload.data(moduleDownload.index(i, 0), 2);
                var downloaded = moduleDownload.data(moduleDownload.index(i, 0), 3);

                if (selecting) {
                    selectionArray.push({"selecting": selecting, "moduleId": module_id, "groupId": group_id});
                }
                if (downloaded) {
                    downloadedArray.push({"downloaded": downloaded, "moduleId": module_id, "groupId": group_id});
                }
            }
            selectedModules = selectionArray;
            downloadedModules = downloadedArray;
            moduleDownload.clearObjects();
        }
    }

//    HistoryModel {
//        id: historyModel
//    }

//    BooksModel {
//        id: booksOldTestament

//        Component.onCompleted: {
//            booksOldTestament.oldTestament()
//        }
//    }

//    BooksModel {
//        id: booksNewTestament

//        Component.onCompleted: {
//            booksNewTestament.newTestament()
//        }
//    }

    ModelGroupModules {
        id: groupModules
    }

    ModelModule {
        id: modelModule
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
//                    property int testamentIndex: historyModel.testamentIndex
//                    onTestamentIndexChanged: {
//                        currentIndex = historyModel.testamentIndex
//                    }

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

                Column {
                    id: detail
                    x: Theme.paddingMedium
                    width: parent.width - 2 * Theme.paddingMedium

                    SectionHeader {
                        text: qsTrId("Detailed info")
                    }

                    Label {
                        width: parent.width
                        text: "<p>Святая Библия, Новый Русский Перевод<br/> © Biblica, Inc.®, 2006, 2010, 2012, 2014<br/> Используется с разрешения. Все права защищены по всему миру.</p>"
                        font.pixelSize: Theme.fontSizeSmall
                        wrapMode: Text.WordWrap
                        truncationMode: TruncationMode.Fade
                    }
                }
            }
        }

        ModulesPage {}

        SilicaFlickable {
            id: silicaFlickableSearch
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
                        model: 50

                        ExpandingSection {
                            id: section

                            property int sectionIndex: model.index
                            title: "Section wewe" + (model.index + 1)

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
        height: slideshow.height
        contentHeight: slideshow.height

        SlideshowView {
            id: slideshow
            width: parent.width
            itemWidth: parent.width
            height: page.height
            model: visualModel
        }

        SilicaListView {
            id: panel
            height: Theme.itemSizeMedium
//            model: historyModel
            snapMode: ListView.SnapToItem
            orientation: listHorizontal
            anchors.bottom: parent.bottom
            currentIndex: 0
            visible: slideshow.currentIndex != 1
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

        PushUpMenu {
            id: pushUpMenu

            MenuItem {
                id: updateModules
                text: qsTrId("Update modules")
                visible: slideshow.currentIndex == 1
                enabled: groupModules.newVersionAvailable
                property bool updateСompleted: groupModules.updateCompleted
                onUpdateСompletedChanged : {
                    pushUpMenu.busy = false
                }
                onClicked: {
                    pushUpMenu.busy = true
                    groupModules.downloadRegistry()
                }
            }

            MenuItem {
                text: qsTrId("Some action")
                visible: slideshow.currentIndex == 0
            }

            MenuItem {
                text: qsTrId("Download bulk")
                visible: slideshow.currentIndex == 1
                enabled: isSelecting
                onClicked: {
                    for (var i = 0; i < selectedModules.length; i++) {
                        var selected = selectedModules[i];
                        if (selected.module.download === false) {
                            selected.module.download = true
                        } else {
                            console.log(">>> not exists not exists not exists", selected.id)
                            downloadModulesLater.push(selected.id)
                        }

//                        var selected = selectedModules[i];
//                        console.log(selected.groupIndex, selected.moduleIndex, groupModules.index(selected.groupIndex, 0), groupModules.data(groupModules.index(selected.groupIndex, 0), 2));
//                        var mod = groupModules.data(groupModules.index(selected.groupIndex, 0), 2);
//                        console.log(mod.data(mod.index(selected.moduleIndex, 0), 0));
                    }
                    selectedModules = []
                }
            }
        }
    }

    Component.onCompleted: {
//        expandingSectionGroup.currentIndex = historyModel.testamentIndex;
    }
}
