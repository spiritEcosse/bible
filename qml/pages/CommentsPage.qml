import QtQuick 2.0
import Sailfish.Silica 1.0
import bible.HistoryModel 1.0

Pages {
    property HistoryModel historyModel
    property string textComment

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge
        contentWidth: parent.width

        VerticalScrollDecorator {}
        Column {
            id: column
            x: Theme.paddingMedium
            spacing: Theme.paddingLarge
            width: parent.width - 2 * x

            PageHeader {
                title: qsTr("Comments")
            }

            Column {
                width: parent.width

                Label {
                    text: textComment
                    linkColor: Theme.highlightColor
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    wrapMode: Text.WordWrap
                    truncationMode: TruncationMode.Fade

                    onLinkActivated: {
                        var regExp = /^[A-Z]:(\d+)\s(\d+):(\d+).*$/;
                        var data = link.match(regExp)
                        historyModel.addFromMask(data[1], data[2], data[3]);
                        pageStack.pop(undefined)
                    }
                }
            }
        }
    }
}
