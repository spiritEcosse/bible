import QtQuick 2.0
import Sailfish.Silica 1.0
import bible.ModelGroupModules 1.0
import bible.ModelModule 1.0
import bible.HistoryModel 1.0
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

    ModelModule {
        id: modelModuleBooks
    }

    HistoryModel {
        id: historyModel
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
    }

    VisualItemModel {
        id: visualModel

        BooksPage {}

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
            onCurrentIndexChanged: {
                if (slideshow.currentIndex === 1 && !initPageModules) {
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
        }
    }

    Component.onCompleted: {
//        expandingSectionGroup.currentIndex = historyModel.testamentIndex;
    }
}
