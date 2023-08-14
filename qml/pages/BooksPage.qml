import QtQuick 2.0
import bible.ModelModule 1.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: pageBooks
    width: parent.width
    height: parent.height
    clip: true

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
                                expanded: true

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

                                            property int recordBookIndex: modelRecord.bookIndex
                                            onRecordBookIndexChanged: {
                                                timerOnExpandedBook.restart();
                                            }
                                            Timer {
                                                id: timerOnExpandedBook
                                                interval: 100
                                                repeat: false
                                                onTriggered: {
                                                    listBooks.currentIndex = modelRecord.bookIndex;
                                                }
                                            }
                                            Component.onCompleted: {
                                                if (modelRecord.bookIndex !== -1 && listBooks.model.canFetchMore(listBooks.model.index(0, 0)) &&
                                                        listBooks.model.rowCount() < modelRecord.bookIndex) {
                                                    var index = 1;
                                                    while (listBooks.model.rowCount() < modelRecord.bookIndex && listBooks.model.canFetchMore(listBooks.model.index(0, 0))) {
                                                        listBooks.model.fetchMore(listBooks.model.index(0, 0));
                                                        index += 1;
                                                        if (index === maxFetchMoreCount) {
                                                            break;
                                                        }
                                                    }
                                                }
                                                listBooks.currentIndex = modelRecord.bookIndex;
//                                                console.log("listBooks: " + modelRecord.bookIndex, listBooks.currentIndex);
                                            }

                                            delegate: ListItem {
                                                id: columnBook
                                                width: parent.width
                                                contentHeight: expandingSectionBook.height + Theme.paddingMedium

                                                ExpandingSectionPatch {
                                                    _group: listBooks
                                                    id: expandingSectionBook
                                                    title: long_name.trim() + index
                                                    width: parent.width
                                                    property int bookIndex: index
                                                    property int recordBookIndex: modelRecord.bookIndex
                                                    property bool sameBook: index === modelRecord.bookIndex
                                                    onSameBookChanged: {
                                                        if (sameBook) {
//                                                            console.log(expanded, sameBook, index, recordBookIndex);
                                                            expanded = sameBook;
                                                            if (expanded) {
                                                                listBooks.currentIndex = index;
                                                            }
                                                        }
                                                    }
                                                    expanded: {
//                                                        console.log(sameBook, index, modelRecord.bookIndex);
                                                        return sameBook;
                                                    }
//                                                    onFirstBookIndexChanged: {
//                                                        console.log(modelRecord.firstBookIndex, index);
//                                                        expanded = modelRecord.firstBookIndex === index;
//                                                    }
                                                    property string bookShortName: short_name
                                                    Timer {
                                                        id: timerOnExpandedBookDelegate
                                                        interval: 100
                                                        repeat: false
                                                        onTriggered: {
//                                                            console.log(expandingSectionBook.expanded, index, modelRecord.firstBookIndex, listBooks.currentIndex);
                                                            listBooks.currentIndex = index;
                                                            modelRecord.bookIndex = listBooks.currentIndex;
//                                                            console.log(modelRecord.bookIndex, modelRecord.chapterIndex, modelRecord.verseIndex);
//                                                            modelRecord.createRecord(short_name, index);
//                                                            historyModel.bookIndex = index;
//                                                            historyModel.bookShortName = short_name;
                                                        }
                                                    }
                                                    onExpandedChanged: {
//                                                        console.log(index, expanded, listBooks.currentIndex);
                                                        if (expanded) {
//                                                            console.log(modelRecord.firstBookIndex, index);
//                                                             listBooks.model.currentBook = book_number;
                                                            timerOnExpandedBookDelegate.restart();
//                                                            historyModel.bookIndex = index;
//                                                            historyModel.bookShortName = short_name;
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
                                                            Component.onCompleted: {
                                                                if (modelRecord.chapterIndex !== -1 && expandingSectionBook.sameBook &&
                                                                        listChapters.model.canFetchMore(listChapters.model.index(0, 0)) &&
                                                                        listChapters.model.rowCount() < modelRecord.chapterIndex) {
                                                                    var index = 1;
                                                                    while (listChapters.model.rowCount() < modelRecord.chapterIndex && listChapters.model.canFetchMore(listChapters.model.index(0, 0))) {
                                                                        listChapters.model.fetchMore(listChapters.model.index(0, 0));
                                                                        index += 1;
                                                                        if (index === maxFetchMoreCount) {
                                                                            break;
                                                                        }
                                                                    }
                                                                    listChapters.currentIndex = modelRecord.chapterIndex;
                                                                }
                                                            }
                                                            property int recordChapterIndex: modelRecord.chapterIndex
                                                            onRecordChapterIndexChanged: {
                                                                timerOnExpandedChapter.restart();
                                                            }
                                                            Timer {
                                                                id: timerOnExpandedChapter
                                                                interval: 100
                                                                repeat: false
                                                                onTriggered: {
                                                                    listChapters.currentIndex = modelRecord.chapterIndex;
                                                                }
                                                            }

                                                            delegate: Column {
                                                                width: parent.width

                                                                ExpandingSectionPatch {
                                                                    id: chapterExpandingSection
                                                                    property int chapter: index + 1
                                                                    title: "Chapter " + chapter
                                                                    width: parent.width
                                                                    property int chapterIndex: index
                                                                    property bool sameChapter: index === modelRecord.chapterIndex
                                                                    onSameChapterChanged: {
                                                                        if (sameChapter) {
                                                                            expanded = expandingSectionBook.sameBook && sameChapter;
                                                                            if (expanded) {
                                                                                listChapters.currentIndex = index;
                                                                            }
                                                                        }
                                                                    }

                                                                    expanded: expandingSectionBook.sameBook && sameChapter
//                                                                    onFirstChapterIndexChanged: {
//                                                                        console.log(modelRecord.firstChapterIndex, index);
//                                                                        expanded = modelRecord.firstChapterIndex === index && expandingSectionBook.bookIndex === modelRecord.firstBookIndex;
//                                                                    }
                                                                    Timer {
                                                                        id: timerOnExpandedChapterDelegate
                                                                        interval: 100
                                                                        repeat: false
                                                                        onTriggered: {
                                                                            listChapters.currentIndex = index;
                                                                            modelRecord.chapterIndex = listChapters.currentIndex;
//                                                                            console.log(modelRecord.bookIndex, modelRecord.chapterIndex, modelRecord.verseIndex);
                                                                        }
                                                                    }
                                                                    onExpandedChanged: {
                                                                        if (expanded) {
//                                                                            console.log(index, modelRecord.firstChapterIndex);
                                                                            timerOnExpandedChapterDelegate.restart();
//                                                                            listChapters.currentIndex = index;
//                                                                            listBooks.model.currentChapter = chapter;
//                                                                            historyModel.chapterIndex = chapter;
                                                                        }
                                                                    }

                                                                    content.sourceComponent: SilicaFlickable {
                                                                        width: parent.width
                                                                        height: Screen.height - pageHeader.height - 5  * modulesActive.buttonHeight - panelImage.height

                                                                        SilicaListView {
                                                                            id: listVerses
                                                                            model: verses
                                                                            width: parent.width
                                                                            height: parent.height
                                                                            spacing: 0
                                                                            clip: true
                                                                            VerticalScrollDecorator {}
                                                                            snapMode: ListView.SnapToItem
                                                                            highlightRangeMode: ListView.StrictlyEnforceRange

                                                                            Component.onCompleted: {
                                                                                if (expandingSectionBook.sameBook && chapterExpandingSection.sameChapter &&
                                                                                        modelRecord.verseIndex !== -1 &&
                                                                                        listVerses.model.rowCount() < modelRecord.verseIndex &&
                                                                                        listVerses.model.canFetchMore(listVerses.model.index(0, 0))) {
                                                                                    var index = 1;
//                                                                                    console.log(listVerses.model.rowCount(), modelRecord.verseIndex, listVerses.model.canFetchMore(listVerses.model.index(0, 0)));
                                                                                    while (listVerses.model.rowCount() < modelRecord.verseIndex && listVerses.model.canFetchMore(listVerses.model.index(0, 0))) {
                                                                                        listVerses.model.fetchMore(listVerses.model.index(0, 0));
                                                                                        index += 1;
                                                                                        if (index === maxFetchMoreCount) {
                                                                                            break;
                                                                                        }
                                                                                    }
                                                                                    listVerses.currentIndex = modelRecord.verseIndex;
                                                                                } else {
                                                                                    listVerses.currentIndex = modelRecord.verseIndex !== -1 ? modelRecord.verseIndex : 0
                                                                                }

                                                                                timerOnExpandedVerse.restart();
                                                                            }
                                                                            onMovementEnded: {
                                                                                create_record();
                                                                            }
                                                                            function create_record() {
                                                                                var index = currentIndex;
                                                                                if (index === -1) {
                                                                                    index = 0;
                                                                                }
//                                                                                console.log(expandingSectionBook.bookShortName, expandingSectionBook.bookIndex, chapterExpandingSection.chapterIndex, index, currentIndex);
//                                                                                modelRecord.firstBookIndex = expandingSectionBook.bookIndex;
//                                                                                modelRecord.firstChapterIndex = chapterExpandingSection.chapterIndex;
                                                                                modelRecord.createRecord(
                                                                                            expandingSectionBook.bookShortName,
                                                                                            expandingSectionBook.bookIndex,
                                                                                            chapterExpandingSection.chapterIndex,
                                                                                            index
                                                                                );
                                                                            }
                                                                            Timer {
                                                                                id: timerOnExpandedVerse
                                                                                interval: 100
                                                                                repeat: false
                                                                                onTriggered: {
                                                                                    listVerses.create_record();
                                                                                }
                                                                            }

                                                                            delegate: ListItem {
                                                                                id: listItem
                                                                                menu: contextMenu
                                                                                property bool reached: modelRecord.verseIndex === index && expandingSectionBook.sameBook && chapterExpandingSection.sameChapter;
                                                                                onReachedChanged: {
//                                                                                    console.log(listVerses.currentIndex, index, modelRecord.bookIndex, modelRecord.chapterIndex, modelRecord.verseIndex, expandingSectionBook.sameBook, chapterExpandingSection.sameChapter);

                                                                                    if (reached) {
                                                                                        timerOnReachedVerse.restart();
//                                                                                        console.log(index);
//                                                                                        listVerses.currentIndex = index;
                                                                                    }
                                                                                }
                                                                                contentHeight: childVerse.height + separator.height + Theme.paddingMedium
                                                                                y : Theme.paddingLarge
                                                                                width: parent.width
                                                                                onClicked: {
//                                                                                    console.log(listVerses.currentIndex, index);
                                                                                    listVerses.currentIndex = index;
                                                                                    listVerses.create_record();
//                                                                                    historyModel.verseIndex = index + 1
                                                                                }

                                                                                Timer {
                                                                                    id: timerOnReachedVerse
                                                                                    interval: 100
                                                                                    repeat: false
                                                                                    onTriggered: {
//                                                                                       if ( expandingSectionBook.sameBook && chapterExpandingSection.sameChapter) {
//                                                                                        console.log(listVerses.currentIndex, index, modelRecord.bookIndex, modelRecord.chapterIndex, modelRecord.verseIndex, expandingSectionBook.sameBook, chapterExpandingSection.sameChapter);
                                                                                        listVerses.currentIndex = index;
//                                                                                        console.log(index);
//                                                                                        console.log(listVerses.model.rowCount(), listVerses.model.canFetchMore(listVerses.model.index(0, 0)), index, modelRecord.verseIndex);
//                                                                                       }
                                                                                    }
                                                                                }
                                                                                Label {
                                                                                    id: verseNumber
                                                                                    color: Theme.highlightColor
                                                                                    width: Math.round(3 * Theme.paddingLarge)
                                                                                    text: verse + '. '
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
                                                                                        color: listVerses.currentIndex === index ? verseNumber.color : Theme.primaryColor

                                                                                        text: {
                                                                                            var regex = /<f>\[\d+\]<\/f>/g;
                                                                                            var matches = content_text.match(regex);
                                                                                            var verse_text_copy = content_text;

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
                                                                                            var textComment = "";

                                                                                            for (var pos = 0; pos < comments.rowCount(); pos++) {
                                                                                                var marker_db = comments.data(comments.index(pos, 0), 1);
                                                                                                var verse_from_number = comments.data(comments.index(pos, 0), 2);
                                                                                                if (link === marker_db && verse_from_number === index + 1) {
                                                                                                    textComment = comments.data(comments.index(pos, 0), 0);
                                                                                                }
                                                                                            }

                                                                                            if (textComment) {
                                                                                                listVerses.currentIndex = index;
                                                                                                listVerses.create_record();
                                                                                                pageStack.push(
                                                                                                            commentsPage,
                                                                                                            {
                                                                                                                "textComment": textComment,
                                                                                                                "modelRecord": modelRecord,
                                                                                                                "expandingSectionBook": expandingSectionBook,
                                                                                                                "modelBook": listBooks.model
                                                                                                            })
                                                                                            }
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
                model: modelRecord
                visible: modelRecord.rowExists
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
                        modelRecord.createRecord(
                                    book_short_name,
                                    book_index,
                                    chapter_index,
                                    verse_index
                        );

//                        if (book_index !== historyModel.bookIndex) {
//                            historyModel.copyObject(testament_index, book_index, chapter_index, verse_index, book_short_name);
//                        } else if (chapter_index !== historyModel.chapterIndex) {
//                            historyModel.copyObject(testament_index, book_index, chapter_index, verse_index, book_short_name);
//                        } else if (verse_index !== historyModel.verseIndex) {
//                            historyModel.verseIndex = verse_index;
//                        }
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
                        color: sourcePanelHistory.currentIndex === index ? Theme.highlightColor : Theme.primaryColor
                        text: {
                            var real_chapter_index = chapter_index + 1;
                            var real_verse_index = verse_index + 1;
                            return book_short_name + book_index + ":" + real_chapter_index + ":" + real_verse_index;
                        }
                        font.pixelSize: Theme.fontSizeLarge
                    }
                }
            }

            Label {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Theme.fontSizeLarge
                visible: !sourcePanelHistory.visible
                text: qsTrId("Here will be the history of texts.")
                width: parent.width - iconBack.width
                truncationMode: TruncationMode.Fade
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
                id: expanderIcon
                icon.source: "image://theme/icon-s-arrow"
                onClicked: drawer.open = !drawer.open
                anchors {
                    right: iconHistory.left
                    verticalCenter: parent.verticalCenter
                }
            }

            IconButton {
                id: iconHistory
                icon.source: "image://theme/icon-m-history"
                icon.sourceSize: Qt.size(Theme.iconSizeMedium, Theme.iconSizeMedium)
                onClicked: panelHistory.enabled = true
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }

            states: [
                State {
                    name: "drawerOpen"
                    when: drawer.opened
                    PropertyChanges { target: expanderText; text: "Close modules list" }
                    PropertyChanges { target: expanderIcon; rotation: 180 }
                },
                State {
                    name: "drawerClose"
                    when: !drawer.opened
                    PropertyChanges { target: expanderText; text: "Open modules list" }
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
