import QtQuick 2.0
import Sailfish.Silica 1.0
import bible.QuickDownload 1.0

SilicaFlickable {
    id: silicaFlickableModules
    width: parent.width
    height: parent.height
    clip: true
    property int activeIndex: -1

    GlassItem {
        id: defaultItem
        visible: false
        falloffRadius: 0.1500
        radius: 0.1500
    }

    Column {
        id: columnModules
        width: parent.width
        height: parent.height

        PageHeader {
            id: pageHeaderModules
            title: qsTrId("Modules")
        }

        ExpandingSectionGroupPatch {
            id: expandingSectionGroupModules
            height: parent.height - pageHeaderModules.height

            SilicaListView {
                id: listViewModules
                model: groupModules
                width: parent.width
                height: parent.height
                clip: true
                VerticalScrollDecorator {}

                section {
                    property: 'region'

                    delegate: SectionHeader {
                        text: Theme.highlightText(
                                  section.substr(0, section.length),
                                  searchField.text,
                                  Theme.highlightBackgroundColor);
                        height: Theme.itemSizeSmall
                    }
                }

                delegate: Column {
                    width: parent.width

                    ExpandingSectionPatch {
                        title: Theme.highlightText(
                                   model.titleGroup.substr(0, model.titleGroup.length),
                                   searchField.text,
                                   Theme.highlightColor);
                        property int indexM: activeIndex
                        onIndexMChanged: {
                            if (activeIndex != index) {
                                expanded = false
                            }
                        }
                        onExpandedChanged: {
                            if (expanded) {
                                activeIndex = index
                            }
                        }

                        content.sourceComponent: Column {
                            width: parent.width

                            SilicaListView {
                                id: listModules
                                width: parent.width
                                model: modules
                                height: count_modules > 4 ? Screen.height / 2 : Screen.height / 4
                                focus: true
                                clip: true
                                y: Theme.paddingLarge
                                VerticalScrollDecorator {}

                                delegate: ListItem {
                                    id: listItem
                                    menu: contextMenu
                                    contentHeight: child.height + separator.height + Theme.paddingMedium
                                    width: parent.width
                                    RemorseItem { id: remorse }
                                    enabled: !hid
                                    function download() {
                                        remorseAction("Downloading", function() {
                                            console.log(index)
                                            progressBar.visible = true
                                            child.height += progressBar.implicitHeight
                                            downloadModule.running = true
                                        })
                                    }

                                    Label {
                                        id: moduleNumber
                                        color: Theme.highlightColor
                                        width: 2.5 * Theme.paddingLarge
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
                                        opacity: hid ? 0.5 : 1
                                        height: childrenRect.height
                                        anchors.left: moduleNumber.right
                                        anchors.right: parent.right
                                        anchors.rightMargin: moduleNumber.width
                                        Behavior on height {
                                            NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
                                        }

                                        Label {
                                            id: abr
                                            wrapMode: Text.WordWrap
                                            truncationMode: TruncationMode.Fade
                                            text: abbreviation
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
                                            truncationMode: TruncationMode.Fade
                                            wrapMode: Text.WordWrap
                                            text: description
                                            anchors {
                                                top: dateUpdate.bottom
                                                left: parent.left
                                                right: parent.right
                                            }
                                        }

                                        QuickDownload {
                                            id: downloadModule

                                            url: "http://s4.igrnt.info/m/10CD-p.plan.zip"
                                            destination: "file:///home/nemo/.local/share/spirit/bible/download/10CD-p.plan.zip"

                                            running: false
                                            overwrite: true

                                            followRedirects: true
//                                            onRedirected: console.log('Redirected',url,'->',redirectUrl)

                                            onStarted: console.log('Started download',url)
                                            onError: console.error(errorString)
                                            onProgressChanged: console.log(url,'progress:',progress)
                                            onFinished: console.info(url,'done')
                                        }

                                        ProgressBar {
                                            id: progressBar
                                            width: parent.width
                                            leftMargin: 5
                                            rightMargin: 5
                                            minimumValue: 0
                                            maximumValue: 1
                                            value: downloadModule.progress
                                            anchors.top: des.bottom
                                            visible: false
                                            height: visible ? height : 0
                                        }
                                    }

                                    ContextMenu {
                                        id: contextMenu

                                        MenuItem {
                                            text: qsTr("Copy text");
                                        }
                                        MenuItem {
                                            text: "Download"
                                            onClicked: download()
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
