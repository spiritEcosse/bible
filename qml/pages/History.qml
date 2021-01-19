import QtQuick 2.0
import Sailfish.Silica 1.0

Pages {
    SilicaListView {
        keyNavigationWraps: true
        id: listView
        anchors.fill: parent
        currentIndex: 0
        model: stories
        VerticalScrollDecorator {}
        header: PageHeader {
            title: qsTrId("History")
        }
        section {
            property: 'section'
            delegate: Item {
                height: Theme.itemSizeSmall
                width: parent.width

                Rectangle {
                    anchors.fill: parent
                    color: Theme.primaryColor
                    opacity: 0.15
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.highlightColor
                    text: section
                    font.pixelSize: Theme.fontSizeMedium
                }
            }
        }
        delegate: ListItem {
            id: mainColumn
            menu: contextMenu
            property string sectionUp: {
                var QModelIndex = listView.model.index(model.indexUp, 0)
                return listView.model.data(QModelIndex, 5) + ': ' + listView.model.data(QModelIndex, 3)
            }
            property string sectionDown: {
                var QModelIndex = listView.model.index(model.indexDown, 0)
                return listView.model.data(QModelIndex, 5) + ': ' + listView.model.data(QModelIndex, 3)
            }

            Item {
                x: Theme.paddingMedium
                width: parent.width - 2*x
                height: childrenRect.height
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    id: origin
                    text: mainColumn.sectionUp + ' - ' + mainColumn.sectionDown
                    font.pixelSize: Theme.fontSizeMedium
                    truncationMode: TruncationMode.Fade
                    horizontalAlignment: Text.AlignLeft
                    anchors {
                        left: parent.left
                        right: date.left
                        rightMargin: Theme.paddingSmall
                    }
                }
                Label {
                    id: date
                    text: Qt.formatDateTime(model.dateTime, 'hh:mm:ss, dd.MM.yyyy')
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                    horizontalAlignment: Text.AlignRight
                    anchors {
                        right: parent.right
                        baseline: origin.baseline
                    }
                }
            }

            ContextMenu {
                id: contextMenu

                MenuItem {
                    text: mainColumn.sectionUp
                    onClicked: pageStack.push(Qt.resolvedUrl("VersesModel.qml"), {'currentIndex': model.indexUp})
                }
                MenuItem {
                    text: mainColumn.sectionDown
                    onClicked: pageStack.push(Qt.resolvedUrl("VersesModel.qml"), {'currentIndex': model.indexDown})
                }
            }
        }

        MouseArea {
            id: up
            width: Theme.horizontalPageMargin
            height: Screen.height / 2
            anchors.top: parent.top
            anchors.left: parent.left
            onClicked: listView.decrementCurrentIndex()
        }

        MouseArea {
            id: down
            width: up.width
            anchors.top: up.bottom
            anchors.bottom: parent.bottom
            anchors.left: up.left
            onClicked: listView.incrementCurrentIndex()
        }

        MouseArea {
            id: upRight
            width: up.width
            height: up.height
            anchors.top: parent.top
            anchors.right: parent.right
            onClicked: listView.decrementCurrentIndex()
        }

        MouseArea {
            width: upRight.width
            anchors.top: upRight.bottom
            anchors.bottom: parent.bottom
            anchors.right: upRight.right
            onClicked: listView.incrementCurrentIndex()
        }
    }
}
