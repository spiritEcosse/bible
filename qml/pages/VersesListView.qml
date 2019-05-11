import QtQuick 2.0
import Sailfish.Silica 1.0
import QtFeedback 5.0

SilicaListView {
    id: listView
    width: parent.width
//    property bool navigateByBook: false
//    property int bottomY: 0
//    property int topY: 0
//    property int heightPage: Screen.height
//    property int nextPage: contentY + heightPage - header.height - bottomY
//    property int prevPage: contentY + topY
//    property var verseIdDown: indexAt(0, nextPage)
//    property int actionArea: Theme.horizontalPageMargin
//    currentIndex: page.currentIndex ? page.currentIndex : 0
    spacing: Theme.paddingMedium
    snapMode: ListView.SnapToItem
    highlightRangeMode: ListView.StrictlyEnforceRange
//    onMovementEnded: listView.history()
//    Component.onCompleted: listView.history()
//    onContentHeightChanged: listView.history()
    highlightFollowsCurrentItem: true

    delegate: ListItem {
        id: listItem
        menu: contextMenu
        focus: true
        contentHeight: child.height
        width: parent.width

        Item {
            id: child
            height: childrenRect.height
            x: Theme.paddingMedium
            width: parent.width - 2*Theme.paddingMedium

            Label {
                color: Theme.highlightColor
                id: indexModel
                width: Theme.horizontalPageMargin
                text: verse + '.'
                font.italic: true
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                anchors {
                    left: parent.left
                }
            }
            Label {
                wrapMode: Text.WordWrap
                truncationMode: TruncationMode.Fade
                text: model.text
                textFormat: Text.PlainText
                font.pixelSize: Theme.fontSizeSmall
                width: parent.width
                anchors {
                    leftMargin: Theme.paddingMedium
                    left: indexModel.right
                    right: parent.right
                }
            }
        }

        ContextMenu {
            id: contextMenu

//            MenuItem {
//                visible: searchField.needle && searchField.enabled
//                onDownChanged: longBuzz.play()
//                text: qsTrId("Go to")
//                onClicked: {
//                    pageStack.replace(
//                                versesModelComponent,
//                                {'currentIndex': id - 1, 'searchText': searchField.text})
//                    searchField.text = '';
//                }
//            }

            MenuItem {
                text: qsTr("Copy text");
                onClicked: Clipboard.text = model.text
            }
        }
    }

//    function downIndex() {
//        for (bottomY = 0; verseIdDown === -1 && prevPage < nextPage; bottomY += spacing) {}
//    }

//    function history() {
//        downIndex()
//        timerStoriesAdd.restart()

//        if (currentIndex !== -1) {
//            var QModelIndex = model.index(currentIndex, 0)
//            var bookId = model.data(QModelIndex, 1);
//            if (bookId)
//                listBooks.bookId = bookId
//        }
//    }

//    function verticalSlip(direction) {
//        timerVerticalSlip.direction = direction

//        if (timerVerticalSlip.running) {
//            timerVerticalSlip.stop()
//            doubleClick(direction)
//        } else {
//            timerVerticalSlip.start()
//        }
//    }

//    Timer {
//        id: timerStoriesAdd
//        interval: 2000
//        repeat: false
//        onTriggered: {
//            if (listView.verseIdDown !== -1 && listView.currentIndex) {
//                stories.addInBeginList(new Date(), listView.currentIndex, listView.verseIdDown)
//            }
//        }
//    }

//    Timer {
//        id: slideListView
//        interval: 100
//        repeat: true
//        property MouseArea direction
//        onTriggered: listView.doubleClick(direction)
//        triggeredOnStart: true
//    }

//    Timer {
//        id: timerVerticalSlip
//        interval: 200
//        property MouseArea direction
//        onTriggered: listView.singleClick(direction)
//    }

//    function singleClick(direction) {
//        switch (direction) {
//        case up:
//            listView.decrementCurrentIndex()
//            break
//        case down:
//            listView.currentIndex = listView.verseIdDown
//            break
//        }

//        listView.history()
//    }
//    function doubleClick(direction) {
//        if (listView.navigateByBook === true) {
//            listView.toBook(direction)
//        } else {
//            listView.toChapter(direction)
//        }
//    }
//    function toChapter(direction) {
//        var chaptersCurrentIndex = direction.chaptersCurrentIndex

//        if ( chaptersCurrentIndex < chaptersListModel.rowCount() && chaptersCurrentIndex >= 0) {
//            listBooks.currentIndex = chaptersCurrentIndex
//            var QModelIndex = chaptersListModel.index(chaptersCurrentIndex, 0)
//            listView.currentIndex = chaptersListModel.data(QModelIndex, 0) - 1
//            listView.history()
//        }
//    }
//    function toBook(direction) {
//        var chapterIndex = listBooks.currentIndex
//        var chaptersBook, QModelIndex, chapterCurrent

//        switch (direction) {
//        case up:
//            QModelIndex = chaptersListModel.index(chapterIndex, 0)
//            chapterCurrent = chaptersListModel.data(QModelIndex, 1)
//            chapterIndex += -chapterCurrent

//            if (chapterIndex >= 0) {
//                QModelIndex = chaptersListModel.index(chapterIndex, 0)
//                chapterIndex += -chaptersListModel.data(QModelIndex, 4) + 1
//            }
//            break
//        case down:
//            QModelIndex = chaptersListModel.index(chapterIndex, 0)
//            chaptersBook = chaptersListModel.data(QModelIndex, 4)
//            chapterCurrent = chaptersListModel.data(QModelIndex, 1)
//            chapterIndex += -chapterCurrent + chaptersBook + 1
//            break
//        }

//        if (chapterIndex < chaptersListModel.rowCount() && chapterIndex >= 0) {
//            listBooks.currentIndex = chapterIndex
//            QModelIndex = chaptersListModel.index(chapterIndex, 0)
//            listView.currentIndex = chaptersListModel.data(QModelIndex, 0) - 1
//            listView.history()
//        }
//    }

//    MouseArea {
//        id: up
//        enabled: !drawerBooks.open
//        width: Theme.itemSizeMedium
//        height: Screen.height / 2 - listView.anchors.topMargin
//        anchors.top: parent.top
//        anchors.left: parent.left
//        onClicked: listView.verticalSlip(up)
//        property int chaptersCurrentIndex: listBooks.currentIndex - 1
//        property int index: listView.currentIndex
//        property real positionMode: ListView.End
//        onPressAndHold: {
//            slideListView.direction = up
//            slideListView.restart()
//        }
//    }

//    MouseArea {
//        id: down
//        width: up.width
//        enabled: up.enabled
//        anchors.top: up.bottom
//        anchors.bottom: parent.bottom
//        anchors.left: up.left
//        onClicked: listView.verticalSlip(down)
//        property int chaptersCurrentIndex: listBooks.currentIndex + 1
//        property int index: listView.verseIdDown
//        property real positionMode: ListView.Beginning
//        onPressAndHold: {
//            slideListView.direction = down
//            slideListView.restart()
//        }
//    }

//    MouseArea {
//        id: upRight
//        width: up.width
//        enabled: up.enabled
//        height: up.height
//        anchors.top: parent.top
//        anchors.right: parent.right
//        onClicked: listView.verticalSlip(up)
//        onPressAndHold: {
//            slideListView.direction = up
//            slideListView.restart()
//        }
//    }

//    MouseArea {
//        width: upRight.width
//        enabled: down.enabled
//        anchors.top: upRight.bottom
//        anchors.bottom: parent.bottom
//        anchors.right: upRight.right
//        onClicked: listView.verticalSlip(down)
//        onPressAndHold: {
//            slideListView.direction = down
//            slideListView.restart()
//        }
//    }
}
