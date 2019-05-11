import QtQuick 2.0
import Sailfish.Silica 1.0

Pages {
    property string information
    property string comment

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
                title: qsTr("Details")
            }

            SectionHeader {
                text: qsTr("Information")
            }

            Column {
                width: parent.width

                Label {
                    text: information
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    wrapMode: Text.WordWrap
                    truncationMode: TruncationMode.Fade
                }
            }

            SectionHeader {
                text: qsTr("Comment")
            }

            Column {
                width: parent.width

                Label {
                    width: parent.width
                    text: comment
                    font.pixelSize: Theme.fontSizeSmall
                    wrapMode: Text.WordWrap
                    truncationMode: TruncationMode.Fade
                }
            }
        }
    }
}
