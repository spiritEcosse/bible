import QtQuick 2.0
import Sailfish.Silica 1.0

Pages {
    id: page
    property bool searchMode

    SilicaFlickable {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width

        FlippingPageHeader {
            id: header
            animate: page.status === PageStatus.Active
            width: parent.width
            title: listView.currentSection
        }

        Image {
            width: parent.width
            height: header.height
            source: "image://theme/graphic-gradient-edge"
            rotation: 180
        }

        SilicaListView {
            id: listView
            clip: true
            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
                bottom: bottomImg.top
            }
            spacing: Theme.paddingLarge
            snapMode: ListView.SnapToItem
            highlightRangeMode: ListView.StrictlyEnforceRange

            header: PageHeader {
                title: qsTrId("Modules")
            }

            model: moduleProxyModel

            section {
                property: 'section'

                delegate: SectionHeader {
                    text: section
                    height: Theme.itemSizeExtraSmall
                }
            }

            VerticalScrollDecorator {}

            delegate: ListItem {
                id: listItem
                menu: contextMenu
                contentHeight: child.height
                onClicked: remorse.execute(
                               listItem,
                               qsTrId("Upload"),
                               function() { console.log("Upload") }
                               )


                RemorseItem { id: remorse }

                Item {
                    id: child
                    x: Theme.paddingMedium
                    height: childrenRect.height
                    width: parent.width - 2 * x

                    Label {
                        id: desc
                        wrapMode: Text.WordWrap
                        truncationMode: TruncationMode.Fade
                        text: {
                            var string = description + ' (' + abbreviation + ')';
                            string ? Theme.highlightText(
                                          string.substr(0, string.length),
                                          searchField.text,
                                          Theme.highlightColor) : "";
                        }
                        font.pixelSize: Theme.fontSizeMedium
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                    }
                    Label {
                        id: dateUpdate
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: Theme.fontSizeExtraSmall
                        text: qsTrId("Last module update date: ") + Qt.formatDate(updateDate, 'dd.MM.yyyy')
                        anchors {
                            top: desc.bottom
                            topMargin: Theme.paddingSmall
                            left: parent.left
                        }
                    }
                    Label {
                        text: "- " + Format.formatFileSize(size, 2)
                        font.pixelSize: Theme.fontSizeExtraSmall * 3 / 4
                        font.italic: true
                        anchors {
                            top: desc.bottom
                            topMargin: Theme.paddingSmall
                            left: dateUpdate.right
                            right: parent.right
                        }
                    }
                }

                ContextMenu {
                    id: contextMenu

                    MenuItem {
                        text: qsTrId("Information")
                        onClicked: pageStack.push(
                                       moduleInformation,
                                       {
                                           'information': information,
                                           'comment': comment
                                       })
                    }
                }
            }
        }

        PushUpMenu {
            id: pushUpMenu
            MenuItem {
                text: qsTrId("Search")
                onDelayedClick: searchMode = true
            }
        }

        Image {
            id: bottomImg
            width: parent.width
            property int defHeight: searchField.enabled ? 0 : Theme.itemSizeExtraSmall
            height: defHeight + searchField.height
            source: "image://theme/graphic-gradient-edge"
            anchors.bottom: parent.bottom
        }

        SearchField {
            id: searchField
            property bool active: activeFocus || text.length > 0
            property bool needle: text.length > 2
            onActiveChanged: if (!active) searchMode = false
            onEnabledChanged: {
                if (enabled) {
                    text = ""
                    forceActiveFocus()
                }
            }
            anchors.bottom: parent.bottom
            onTextChanged: {
                if (text.length > 2 || text.length === 0) {
                    listView.model.setFilterRegExp(text)
                    listView.currentIndex = 0
                }
            }
            enabled: searchMode
            width: parent.width
            height: enabled ? implicitHeight : 0.0
            opacity: enabled ? 1.0 : 0.0
            Behavior on opacity { FadeAnimator {} }
            Behavior on height { NumberAnimation { duration: 200; easing.type: Easing.InOutQuad } }
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
        }
    }
}
