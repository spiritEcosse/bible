import QtQuick 2.0
import Sailfish.Silica 1.0
import bible.ModelGroupModules 1.0
import bible.ModelModule 1.0
import bible.ModelRecord 1.0
import bible.ManagerRegistry 1.0
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
    property var selectedModules: [];
    property var downloadedModules: [];
    property var downloadModulesLater: [];
    property bool isSelecting: selectedModules.length;
    property bool initPageModules: false
    property bool flagUpdateObjectsDownloaded: false
    property bool flagUpdateObjectsActive: false
    property int maxFetchMoreCount: 10

    function removeAllTags(text) {
        var result = removeStrongNumber(text);
        result = result.replace(/<i>/g, "");
        result = result.replace(/<\/i>/g, "");
        result = result.replace(/<t>/g, "");
        result = result.replace(/<\/t>/g, "");
        result = result.replace(/<pb\/>/g, "");
        return result;
    }
    function removeStrongNumber(verse_text_copy) {
        return verse_text_copy.replace(/(<S>.*?<\/S>)/g, "");
    }
    function foundObjectInArray(obj, array) {
        if(array.length > 0) {
            for(var i = 0; i < array.length; i += 1) {
                if(array[i] === obj) {
                    return 1;
                }
            }
        }
        return 1;
    }

    ManagerRegistry {
        id: managerRegistry
    }

    ModelModule {
        id: modelModuleBooks
    }

    ModelModule {
        id: modelModulesActive

        Component.onCompleted: {
            modelModulesActive.updateObjectsActive();
            flagUpdateObjectsActive = true;
        }
    }

    ModelRecord {
        id: modelRecord
        property bool rowExists: modelRecord.rowCount()
        property int firstBookIndex: rowExists ? modelRecord.data(modelRecord.index(0, 0), 2) : -1
        property int firstChapterIndex: rowExists ? modelRecord.data(modelRecord.index(0, 0), 3) : -1
        property int firstVerseIndex: rowExists ? modelRecord.data(modelRecord.index(0, 0), 4) : -1
        onRowsInserted: rowExists = true
    }

    ModelModule {
        id: modelModule

        onChangeDownloaded: {
            downloadedModules = modelModule.downloaded;
            modelModuleBooks.updateObjectsDownloaded();
            flagUpdateObjectsDownloaded = true;
        }
        onChangeSelected: {
            selectedModules = modelModule.selected;
        }
    }

    ModelGroupModules {
        id: groupModules
        property bool rowExists: groupModules.rowCount()
        onRowsInserted: rowExists = true
    }

    VisualItemModel {
        id: visualModel

        BooksPage {}

        ModulesPage {}

        SearchPage {}
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
            onCurrentIndexChanged: {
                if (foundObjectInArray(slideshow.currentIndex, [1, 2]) && !initPageModules) {
                    modelModule.init();
                    groupModules.init();
                    initPageModules = true;
                } else if (slideshow.currentIndex === 0 && !flagUpdateObjectsActive) {
                    modelModulesActive.updateObjectsActive();
                    flagUpdateObjectsActive = true;
                }
            }
        }

        PushUpMenu {
            id: pushUpMenu

            MenuItem {
                id: checkUpdates
                text: qsTrId("Check for updates")
                visible: slideshow.currentIndex == 1 && !managerRegistry.newVersionAvailable
//                enabled: !managerRegistry.newVersionAvailable
//                property bool newVersionAvailable:
                property bool checkVersionCompleted: managerRegistry.checkVersionCompleted
                onCheckVersionCompletedChanged : {
                    pushUpMenu.busy = false
                }
                onClicked: {
                    pushUpMenu.busy = true
                    managerRegistry.checkNewVesion()
                }
            }

            MenuItem {
                id: updateModules
                text: qsTrId("Update modules")
                visible: slideshow.currentIndex == 1 && !checkUpdates.visible
                //enabled: !groupModules.updateCompleted
                property bool updateСompleted: groupModules.updateCompleted
                onUpdateСompletedChanged : {
                    pushUpMenu.busy = false
                    managerRegistry.newVersionAvailable = false
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
        }
    }

    Component.onCompleted: {
//        expandingSectionGroup.currentIndex = historyModel.testamentIndex;
    }
}
