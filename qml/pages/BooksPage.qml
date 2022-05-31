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

        dock: page.isPortrait ? Dock.Bottom : Dock.Right
        height: parent.height - panelImage.height
        anchors.bottom: panelImage.top

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
            clip: true

            MouseArea {
                enabled: drawer.opened
                anchors.fill: silicaModules
                onClicked: drawer.open = false
            }

            SilicaFlickable {
                id: silicaModules
                width: parent.width
                height: parent.height
                clip: true
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

                SilicaFlickable {
                    width: parent.width
                    anchors.top: pageHeader.bottom
                    clip: true
                    height: parent.height - pageHeader.height

                    SilicaListView {
                        id: listActiveModules
                        model: modelModulesActive
                        height: parent.height
                        snapMode: ListView.SnapToItem
                        highlightRangeMode: ListView.StrictlyEnforceRange
                        clip: true
                        width: parent.width
                        VerticalScrollDecorator {}

                        delegate: Column {
                            width: parent.width

                            ExpandingSectionPatch {
                                id: modulesActive
                                title: abbreviation + ": " + description;
                                clip: true

                                content.sourceComponent: SilicaFlickable {
                                    width: parent.width
                                    height: Screen.height - pageHeader.height - modulesActive.buttonHeight - panelImage.height

                                    SilicaFlickable {
                                        width: parent.width
                                        anchors.fill: parent

                                        SilicaListView {
                                            id: listBooks
                                            model: books
                                            width: parent.width
                                            height: parent.height
                                            clip: true
                                            VerticalScrollDecorator {}
                                            snapMode: ListView.SnapToItem
                                            highlightRangeMode: ListView.StrictlyEnforceRange
                                            currentIndex: historyModel.bookIndex;

                                            delegate: Column {
                                                id: columnBook
                                                width: parent.width

                                                ExpandingSectionPatch {
                                                    title: long_name.trim() + index
                                                    width: parent.width
                                                    Timer {
                                                        id: timerOnExpandedBook
                                                        interval: 100
                                                        repeat: false
                                                        onTriggered: {
                                                            historyModel.testamentIndex = 0;
                                                            historyModel.bookIndex = index;
                                                            historyModel.bookShortName = short_name;
                                                        }
                                                    }
                                                    property int bookIndex: historyModel.bookIndex
                                                    onBookIndexChanged: {
                                                        if (bookIndex == index) {
                                                            expanded = true;
                                                        }
                                                    }
                                                    onExpandedChanged: {
                                                        if (expanded) {
                                                            listBooks.model.currentBook = book_number;
//                                                            timerOnExpandedBook.restart();
                                                            historyModel.testamentIndex = 0;
                                                            historyModel.bookIndex = index;
                                                            historyModel.bookShortName = short_name;
                                                        }
                                                    }

                                                    content.sourceComponent: SilicaFlickable {
                                                        width: parent.width
                                                        height: Screen.height - pageHeader.height - 3  * modulesActive.buttonHeight - panelImage.height

                                                        SilicaListView {
                                                            id: listChapters
                                                            model: chapters
                                                            width: parent.width
                                                            height: parent.height
                                                            clip: true
                                                            VerticalScrollDecorator {}
                                                            snapMode: ListView.SnapToItem
                                                            highlightRangeMode: ListView.StrictlyEnforceRange
                                                            currentIndex: historyModel.chapterIndex >= 1 ? historyModel.chapterIndex - 1 : 0;
//                                                            Component.onCompleted: {
//                                                                currentIndex = historyModel.chapterIndex >= 1 ? historyModel.chapterIndex - 1 : 0;
//                                                            }

                                                            delegate: Column {
                                                                width: parent.width

                                                                ExpandingSectionPatch {
                                                                    id: chapterExpandingSection
                                                                    property int chapter: index + 1
                                                                    title: "Chapter " + chapter
                                                                    width: parent.width
                                                                    property int chapterIndex: historyModel.chapterIndex
                                                                    onChapterIndexChanged: {
                                                                        if (chapterIndex == chapter) {
                                                                            expanded = true;
                                                                        }
                                                                    }
                                                                    onExpandedChanged: {
                                                                        if (expanded) {
//                                                                            listChapters.currentIndex = index;
                                                                            listBooks.model.currentChapter = chapter;
                                                                            historyModel.chapterIndex = chapter;
                                                                        }
                                                                    }

                                                                    content.sourceComponent: SilicaFlickable {
                                                                        width: parent.width
                                                                        height: Screen.height - pageHeader.height - 5  * modulesActive.buttonHeight - panelImage.height

                                                                        SilicaListView {
                                                                            id: listVerses
                                                                            model: listBooks.model.currentVerses
                                                                            width: parent.width
                                                                            height: parent.height
                                                                            spacing: 0
                                                                            clip: true
                                                                            VerticalScrollDecorator {}
                                                                            snapMode: ListView.SnapToItem
                                                                            highlightRangeMode: ListView.StrictlyEnforceRange
//                                                                            Component.onCompleted: {
//                                                                                currentIndex = historyModel.verseIndex >= 1 ? historyModel.verseIndex - 1 : 0
//                                                                            }
                                                                            onMovementEnded: historyModel.verseIndex = currentIndex + 1
                                                                            currentIndex: historyModel.verseIndex >= 1 ? historyModel.verseIndex - 1 : 0

                                                                            delegate: ListItem {
                                                                                id: listItem
                                                                                menu: contextMenu
                                                                                contentHeight: childVerse.height + separator.height + Theme.paddingMedium
                                                                                y : Theme.paddingLarge
                                                                                width: parent.width
                                                                                onClicked: {
                                                                                    listVerses.currentIndex = index
                                                                                    historyModel.verseIndex = index + 1
                                                                                }

                                                                                Label {
                                                                                    id: verseNumber
                                                                                    color: Theme.highlightColor
                                                                                    width: Math.round(3 * Theme.paddingLarge)
                                                                                    text: verse_number + '. '
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
//                                                                                        bottomMargin: listItem.y
                                                                                    }

                                                                                    Text {
                                                                                        wrapMode: Text.WordWrap
                                                                                        color: listVerses.currentIndex == index ? verseNumber.color : Theme.primaryColor
                                                                                        function removeStrongNumber(verse_text_copy) {
                                                                                            var regex = /(<S>.*?<\/S>)/g;
                                                                                            return verse_text_copy.replace(regex, "");
                                                                                        }

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

                                                                                            return removeStrongNumber(verse_text_copy);
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
                                                                                    }
                                                                                }

                                                                                ContextMenu {
                                                                                    id: contextMenu
//                                                                                    enabled: expandSectionChapter.enabled

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
                                                    }
                                                }
                                            }
                                        }

//                                        SilicaFlickable {
//                                            id: detail
//                                            x: Theme.paddingMedium
//                                            anchors.top: listBooks.bottom
//                                            width: parent.width - 2 * Theme.paddingMedium

//                                            SectionHeader {
//                                                id: detailHeader
//                                                text: qsTrId("Detailed info")
//                                            }

//                                            Label {
//                                                anchors.top: detailHeader.bottom
//                                                width: parent.width
//                                                text: "<p>Святая Библия, Новый Русский Перевод<br/> © Biblica, Inc.®, 2006, 2010, 2012, 2014<br/> Используется с разрешения. Все права защищены по всему миру.</p>"
//                                                font.pixelSize: Theme.fontSizeSmall
//                                                wrapMode: Text.WordWrap
//                                                truncationMode: TruncationMode.Fade
//                                            }
//                                        }
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
                model: historyModel
                snapMode: ListView.SnapToItem
                highlightRangeMode: ListView.StrictlyEnforceRange
                orientation: listHorizontal
                height: parent.height
                width: parent.width - iconBack.width
                spacing:  Theme.paddingLarge
                HorizontalScrollDecorator {
                    anchors.bottomMargin: Theme.paddingSmall
                }
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
                    width: listText.contentWidth
                    height: parent.height
                    focus: true
                    contentHeight: parent.height
                    onClicked: {
                        sourcePanelHistory.currentIndex = 0;
                        console.log(book_index);

                        if (book_index !== historyModel.bookIndex) {
                            historyModel.copyObject(testament_index, book_index, chapter_index, verse_index, book_short_name);
                        } else if (chapter_index !== historyModel.chapterIndex) {
                            historyModel.copyObject(testament_index, book_index, chapter_index, verse_index, book_short_name);
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
                        anchors.verticalCenter: parent.verticalCenter
                        color: sourcePanelHistory.currentIndex == index ? Theme.highlightColor : Theme.primaryColor
                        text: {
                            var title = book_short_name + book_index;

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
