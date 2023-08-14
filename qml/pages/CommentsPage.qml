import QtQuick 2.0
import Sailfish.Silica 1.0
import bible.ModelRecord 1.0
import bible.ModelBook 1.0
import "../components"

Pages {
    property ModelRecord modelRecord
    property string textComment
    property ExpandingSectionPatch expandingSectionBook
    property ModelBook modelBook

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
                        var link_data = link.match(regExp);
                        var short_name_book = modelBook.getShortName(link_data[1]);
                        var firstBookIndex = link_data[1] / 10 - 1;
                        var firstChapterIndex = link_data[2] - 1;
                        modelRecord.createRecord(
                                    short_name_book,
                                    firstBookIndex,
                                    firstChapterIndex,
                                    link_data[3] - 1
                        );
                        pageStack.pop(undefined)
                    }
                }
            }
        }
    }
}
