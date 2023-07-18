import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: silicaFlickableSearch
    width: parent.width
    height: parent.height

    Column {
        id: pageHeader
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter

        FlippingPageHeader {
            id: headerMainTextBooks
            title: qsTrId("Translation search")
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
                        height: Screen.height - pageHeader.height - modulesActive.buttonHeight - panel.height

                        SilicaFlickable {
                            width: parent.width
                            anchors.fill: parent
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

        SearchField {
            id: searchField
            property var needle: groupModules.needle
            property bool searchByModules: false
            property bool searchByGroups: true
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
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
            anchors.right: parent.right
            anchors.left: parent.left
        }
    }
}
