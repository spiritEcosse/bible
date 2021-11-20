import QtQuick 2.0
import Sailfish.Silica 1.0
import bible.QuickDownload 1.0
import "../components"

SilicaFlickable {
    id: silicaFlickableModules
    width: parent.width
    height: parent.height
    clip: true

    function selectedModulesDelete()
    {
        modelModule.updateSelectedBulk(selectedModules);
        selectedModules = [];
    }

    function foundModule(moduleId) {
        var foundIndex = -1;
        if(selectedModules.length > 0) {
            for(var i = 0; i < selectedModules.length; i += 1) {
                if(selectedModules[i].moduleId === moduleId) {
                    foundIndex = i;
                    break;
                }
            }
        }
        return foundIndex;
    }

    function foundModuleDownloaded(moduleId) {
        var foundIndex = -1;
        if(downloadedModules.length > 0) {
            for(var i = 0; i < downloadedModules.length; i += 1) {
                if(downloadedModules[i].moduleId === moduleId) {
                    foundIndex = i;
                    break;
                }
            }
        }
        return foundIndex;
    }

    function appendOrRemoveModuleSelection(object) {
        var selectionArray = selectedModules;
        var foundIndex = foundModule(object.moduleId);
        if (foundIndex > -1) {
            selectionArray.splice(foundIndex, 1);
        } else {
            selectionArray.push(object);
        }
        modelModule.updateSelected(object.moduleId, object.selecting)
        selectedModules = selectionArray;
    }

    function appendOrRemoveModuleDownloaded(object) {
        var downloadedArray = downloadedModules;
        var foundIndex = foundModuleDownloaded(object.moduleId);
        if (foundIndex > -1) {
            downloadedArray.splice(foundIndex, 1);
        } else {
            downloadedArray.push(object);
        }
        modelModule.updateDownloaded(object.moduleId, object.downloaded)
        downloadedModules = downloadedArray;
    }

    SilicaFlickable {
        width: parent.width
        height: parent.height

        Column {
            id: header
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter

            FlippingPageHeader {
                id: headerMainText
                title: qsTrId("Modules")
                width: parent.width

                FlippingLabelPatch {
                    id: headerAddText
                    text: qsTr("%Ln available, ", "available", modelModule.countActive()) +
                          qsTr("%Ln downloaded, ", "downloaded", downloadedModules.length) +
                          qsTr("%Ln selected", "selected", selectedModules.length)
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
            id: listViewModules
            model: groupModules
            width: parent.width
            height: parent.height - header.height - panel.height
            clip: true
            snapMode: ListView.SnapToItem
            highlightRangeMode: ListView.StrictlyEnforceRange
            anchors.bottom: panel.top
            VerticalScrollDecorator {}
            section {
                property: 'region'

                delegate: SectionHeader {
                    text: searchField.searchByGroups ? Theme.highlightText(
                              section.substr(0, section.length),
                              searchField.needle,
                              Theme.highlightBackgroundColor) : section;
                    height: Theme.itemSizeSmall
                }
            }

            delegate: Column {
                id: columnGroup
                width: parent.width

                ExpandingSectionPatch {
                    id: group
                    title: searchField.searchByGroups ? Theme.highlightText(
                               model.titleGroup.substr(0, model.titleGroup.length),
                               searchField.needle,
                               Theme.highlightColor) : model.titleGroup;
                    property int groupId: model.group_id
                    readonly property bool isSelected: selectedModules.some(function(existingModule) {
                        return existingModule.groupId === groupId
                    });
                    readonly property bool isDownloaded: downloadedModules.some(function(existingModule) {
                        return existingModule.groupId === groupId
                    });
                    GlassItemLeft {
                        visible: group.isDownloaded
                    }
                    highlighted: group.isSelected
                    width: parent.width
                    onExpandedChanged: {
                        if (expanded) {
                            listViewModules.currentIndex = index
                        }
                    }

                    content.sourceComponent: Column {
                        id: columnModules
                        width: parent.width

                        states: [
                            State {
                                when: group.expanded
                                PropertyChanges { target: columnModules; height : listViewModules.height - 3 * group.buttonHeight }
                            },

                            State {
                                when: !group.expanded
                                PropertyChanges { target: columnModules; height : 0 }
                            }
                        ]

                        SilicaListView {
                            id: listModules
                            width: parent.width
                            model: modules
                            focus: true
                            height: parent.height
                            clip: true
                            VerticalScrollDecorator {}
                            spacing: 0
                            snapMode: ListView.SnapToItem
                            highlightRangeMode: ListView.StrictlyEnforceRange

                            delegate: ListItem {
                                id: listItem
                                property var myModule: QtObject {
                                    property bool download: false
                                }
                                menu: contextMenu
                                readonly property bool isSelected: isSelecting && selectedModules.some(function(existingModuleId) {
                                    return existingModuleId.moduleId === model.id
                                });
                                readonly property bool isDownloaded: downloadedModules.some(function(existingModuleId) {
                                    return existingModuleId.moduleId === model.id
                                });
                                highlighted: (down || isSelected) && !menuOpen
                                contentHeight: child.height + separator.height + Theme.paddingMedium
                                width: parent.width
                                y : Theme.paddingLarge
                                RemorseItem { id: remorse }
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
                                GlassItemLeft {
                                    visible: listItem.isDownloaded
                                }
                                Item {
                                    id: child
                                    width: parent.width - 2 * moduleNumber.width
                                    opacity: hid ? 0.5 : 1
                                    height: childrenRect.height
                                    anchors.left: moduleNumber.right
                                    anchors.right: parent.right
                                    anchors.rightMargin: moduleNumber.width

                                    Label {
                                        id: abr
                                        wrapMode: Text.WordWrap
                                        truncationMode: TruncationMode.Fade
                                        text: searchField.searchByModules ? Theme.highlightText(
                                                   abbreviation.substr(0, abbreviation.length),
                                                   searchField.needle,
                                                   Theme.highlightColor) : abbreviation;
                                        font.pixelSize: moduleNumber.font.pixelSize
                                        anchors {
                                            left: parent.left
                                            right: parent.right
                                        }
                                    }
                                    Label {
                                        id: dateUpdate
                                        font.pixelSize: Theme.fontSizeExtraSmall
                                        text: qsTrId("Last update date: ") + Qt.formatDate(date, 'dd.MM.yyyy')
                                        anchors {
                                            top: abr.bottom
                                            left: parent.left
                                            topMargin: Theme.paddingSmall
                                        }
                                    }
                                    Label {
                                        text: "- " + Format.formatFileSize(size, 2)
                                        font.pixelSize: Theme.fontSizeExtraSmall * 3 / 4
                                        font.italic: true
                                        anchors {
                                            top: abr.bottom
                                            right: parent.right
                                            left: dateUpdate.right
                                            topMargin: Theme.paddingSmall
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
                                            top: dateUpdate.bottom
                                            left: parent.left
                                            right: parent.right
                                        }
                                    }

                                    Label {
                                        id: addInfo
                                        horizontalAlignment: Text.AlignLeft
                                        font.pixelSize: Theme.fontSizeExtraSmall
                                        textFormat: Text.RichText
                                        onLinkActivated: Qt.openUrlExternally(link)
                                        wrapMode: Text.WordWrap
                                        truncationMode: TruncationMode.Fade
                                        text: additional_info
                                        anchors {
                                            top: des.bottom
                                            left: parent.left
                                            right: parent.right
                                        }

                                        states: [
                                            State {
                                                name: "expandedAddInfo"
                                                when: expander.expanded
                                                PropertyChanges { target: addInfo; opacity: 1 }
                                                PropertyChanges { target: addInfo; height : addInfo.implicitHeight }
                                                PropertyChanges { target: expander; rotation: 180 }
                                            },

                                            State {
                                                name: "collapsedAddInfo"
                                                when: !expander.expanded
                                                PropertyChanges { target: addInfo; opacity: 0 }
                                                PropertyChanges { target: addInfo; height : 0 }
                                            }
                                        ]

                                        transitions: [
                                            Transition {
                                                to: "expandedAddInfo"
                                                ParallelAnimation {
                                                    PropertyAnimation { target: addInfo; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                                                    PropertyAnimation { target: addInfo; property: "opacity"; duration: 200; from: 0; to: 1; easing.type: Easing.InOutQuad  }
                                                    RotationAnimation { target: expander; property: "rotation"; duration: 200; direction: RotationAnimation.Clockwise; easing.type: Easing.InOutQuad  }
                                                }
                                            },
                                            Transition {
                                                to: "collapsedAddInfo"
                                                ParallelAnimation {
                                                    PropertyAnimation { target: addInfo; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                                                    PropertyAnimation { target: addInfo; property: "opacity"; duration: 200; from: 1; to: 0; easing.type: Easing.InOutQuad  }
                                                    RotationAnimation { target: expander; property: "rotation"; duration: 200; direction: RotationAnimation.Counterclockwise; easing.type: Easing.InOutQuad  }
                                                }
                                            }
                                        ]
                                    }
                                    ProgressBar {
                                        id: progressBar
                                        width: parent.width
                                        leftMargin: 5
                                        rightMargin: 5
                                        minimumValue: 0
                                        maximumValue: 1
                                        value: downloadModule.progress
                                        anchors.top: addInfo.bottom
                                        visible: false
                                        height: 0
                                        signal onDataChangedd;

                                        QuickDownload {
                                            id: downloadModule

                                            running: false
                                            overwrite: true
                                            moduleName: name

                                            followRedirects: true
                                            onStarted: console.log('Started download',url)
                                            onError: console.error(errorString)
                                            onProgressChanged: console.log(url,'progress:',progress)
                                            onFinished: {
                                                appendOrRemoveModuleDownloaded({"abbreviation": abbreviation, "downloaded": 1, "moduleId": model.id, "module": myModule, "groupId": group.groupId,})
                                                console.info(url,'done')
                                            }
                                        }

                                        states: [
                                            State {
                                                name: "expandedProgressBar"
                                                when: myModule.download
                                                PropertyChanges { target: downloadModule; running: true }
                                                PropertyChanges { target: progressBar; visible: true }
                                                PropertyChanges { target: progressBar; height: implicitHeight }
                                            }
                                        ]

                                        transitions: [
                                            Transition {
                                                to: "expandedProgressBar"
                                                ParallelAnimation {
                                                    PropertyAnimation { target: progressBar; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                                                }
                                            }
                                        ]
                                    }
                                }

                                IconButton {
                                    id: expander
                                    icon.source: "image://theme/icon-s-arrow"
                                    enabled: addInfo.text.length
                                    visible: enabled
                                    property bool expanded: false
                                    anchors {
                                        left: child.right
                                        top: child.top
                                        right: parent.right
                                        verticalCenter: parent.verticalCenter
                                    }
                                    onClicked: expanded = !expanded
                                }

                                ContextMenu {
                                    id: contextMenu

                                    MenuItem {
                                        text: qsTr("Copy text");
                                        onClicked: Clipboard.text = abbreviation
                                    }
                                    MenuItem {
                                        text: qsTr("Download")
                                        enabled: !hid
                                        onClicked: downloadRemorse()
                                    }
                                    MenuItem {
                                        text: qsTr("Select Modules")
                                        enabled: !hid && !isSelecting
                                        onClicked: {
                                            appendOrRemoveModuleSelection({"abbreviation": abbreviation, "selecting": !listItem.isSelected, "moduleId": model.id, "groupId": group.groupId, "module": myModule})
                                        }
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

        Image {
            id: panel
            width: parent.width
            height: Theme.itemSizeMedium
            source: "image://theme/graphic-gradient-edge"
            anchors.bottom: parent.bottom

            states: [
                State {
                    name: "searchFieldActive"
                    when: searchField.enabled
                    PropertyChanges { target: searchField; opacity: 1 }
                    PropertyChanges { target: searchField; height : panel.height }
                    PropertyChanges { target: iconDownload; opacity: 1 }
                    PropertyChanges { target: iconDownload; height : panel.height }
                    PropertyChanges { target: selectedMessagesActions; opacity: 0 }
                    PropertyChanges { target: selectedMessagesActions; height : 0 }
                },

                State {
                    name: "selectedMessagesActionsActive"
                    when: selectedMessagesActions.enabled
                    PropertyChanges { target: searchField; opacity: 0 }
                    PropertyChanges { target: searchField; height : 0 }
                    PropertyChanges { target: selectedMessagesActions; opacity: 1 }
                    PropertyChanges { target: selectedMessagesActions; height : panel.height }
                }
            ]

            transitions: [
                Transition {
                    to: "searchFieldActive"
                    ParallelAnimation {
                        PropertyAnimation { target: searchField; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                        PropertyAnimation { target: searchField; property: "opacity"; duration: 200; from: 0; to: 1; easing.type: Easing.InOutQuad  }
                        PropertyAnimation { target: iconDownload; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                        PropertyAnimation { target: iconDownload; property: "opacity"; duration: 200; from: 0; to: 1; easing.type: Easing.InOutQuad  }
                        RotationAnimation { target: selectedMessagesActions; property: "height"; duration: 200; easing.type: Easing.InOutQuad  }
                        PropertyAnimation { target: selectedMessagesActions; property: "opacity"; duration: 200; from: 1; to: 0; easing.type: Easing.InOutQuad  }
                    }
                },
                Transition {
                    to: "selectedMessagesActionsActive"
                    ParallelAnimation {
                        PropertyAnimation { target: searchField; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                        PropertyAnimation { target: searchField; property: "opacity"; duration: 200; from: 1; to: 0; easing.type: Easing.InOutQuad  }
                        PropertyAnimation { target: iconDownload; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
                        PropertyAnimation { target: iconDownload; property: "opacity"; duration: 200; from: 1; to: 0; easing.type: Easing.InOutQuad  }
                        PropertyAnimation { target: selectedMessagesActions; property: "height"; duration: 200; easing.type: Easing.InOutQuad  }
                        PropertyAnimation { target: selectedMessagesActions; property: "opacity"; duration: 200; from: 0; to: 1; easing.type: Easing.InOutQuad  }
                    }
                }
            ]

            SearchField {
                id: searchField
                property var needle: groupModules.needle
                property bool searchByModules: false
                property bool searchByGroups: true
                enabled: !selectedMessagesActions.enabled
                onNeedleChanged: {
                    searchByModules = groupModules.searchByModules()
                    searchByGroups = groupModules.searchByGroups()
                }
                onTextChanged: {
                    if (text.length >= 2) {
                        groupModules.search(text)
                    } else if (text.length === 0) {
                        searchByModules = false
                        groupModules.getAll()
                    }
                }
//                width: parent.width - iconDownload.width
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: focus = false
                anchors.right: iconDownload.left
                anchors.left: parent.left
            }

            IconButton {
                id: iconDownload
                icon.source: "image://theme/icon-m-back"
                rotation: 180
                icon.sourceSize: Qt.size(Theme.iconSizeMedium, Theme.iconSizeMedium)
                visible: searchField.enabled && isSelecting
                onClicked: selectedMessagesActions.enabled = true
                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
            }

            Loader {
                id: selectedMessagesActions
                asynchronous: true
                anchors.bottom: parent.bottom
                enabled: isSelecting
                property var selecting: isSelecting
                onSelectingChanged: enabled = isSelecting
                width: parent.width
                RemorseItem {
                    id: remorseLoader
                }
                sourceComponent: Component {
                    Item {
                        width: parent.width

                        IconButton {
                            anchors {
                                left: parent.left
                                leftMargin: Theme.horizontalPageMargin
                                verticalCenter: parent.verticalCenter
                            }
                            icon.source: "image://theme/icon-m-cancel"
                            onClicked: {
                                selectedModulesDelete()
                            }
                        }

                        Row {
                            spacing: Theme.paddingSmall
                            anchors {
                                right: parent.right
                                rightMargin: Theme.horizontalPageMargin
                                verticalCenter: parent.verticalCenter
                            }

                            IconButton {
                                icon.source: "image://theme/icon-m-cloud-download"
                                icon.sourceSize: Qt.size(Theme.iconSizeMedium, Theme.iconSizeMedium)
                                enabled: modelModule.downloadCompleted
                                onClicked: {
                                    remorseLoader.execute(selectedMessagesActions,
                                                          qsTr("Download %Ln selected modules", "selected", selectedModules.length), function() {
                                        for (var i = 0; i < selectedModules.length; i++) {
                                            var selected = selectedModules[i];
                                            try {
                                                selected.module.download = true
                                            }
                                            catch(err) {
                                                console.log(err.message);
                                                console.log(">>> not exists not exists not exists", selected.id)
                                                downloadModulesLater.push(selectedModules[i]);
                                            }
                                        }
                                        modelModule.downloadModules(downloadModulesLater);
                                    });
                                }
                            }

                            IconButton {
                                id: buttonDeleteModules
                                icon.source: "image://theme/icon-m-delete"
                                icon.sourceSize: Qt.size(Theme.iconSizeMedium, Theme.iconSizeMedium)
                                enabled: modelModule.deleteCompleted
                                onClicked: {
                                    remorseLoader.execute(selectedMessagesActions,
                                                          qsTr("Delete %Ln selected modules", "selected", selectedModules.length), function() {
                                        modelModule.deleteModules(selectedModules);
                                    });
                                }
                            }

                            IconButton {
                                id: buttonSwitchSearch
                                icon.source: "image://theme/icon-m-search"
                                icon.sourceSize: Qt.size(Theme.iconSizeMedium, Theme.iconSizeMedium)
                                onClicked: {
                                    selectedMessagesActions.enabled = false
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
