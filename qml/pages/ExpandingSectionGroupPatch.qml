import QtQuick 2.0
import Sailfish.Silica 1.0

ExpandingSectionGroup {
    id: expandGroup
    property ExpandingSectionGroup parentGroup
    property int headerHeight: pageHeader ? pageHeader.height : 0
    property int depth: 0
    property int buttonHeight: currentSection ? currentSection.buttonHeight : Theme.itemSizeMedium
    property bool toTop: false
    property bool load: false

    function findFlickable(item) {
        var parentItem = item.parent
        while (parentItem) {
            if (parentItem.maximumFlickVelocity && !parentItem.hasOwnProperty('__silica_hidden_flickable')) {
                return parentItem
            }
            parentItem = parentItem.parent
        }
        return null
    }

    function _updateFlickableContentY(oldSection, newSection) {
        if (!expandGroup._flickable) {
            expandGroup._flickable = findFlickable(expandGroup)
        }
        if (!expandGroup._flickable) {
            return
        }

        var expGrY = currentIndex * buttonHeight
        var absY = toTop || newSection === null ? - buttonHeight : newSection.mapToItem(null, 0, 0).y

        if (parentGroup) {
            expGrY += (parentGroup.currentIndex + depth) * parentGroup.buttonHeight
        }

        contentYAnimation.to = expGrY + headerHeight - absY
        contentYAnimation.start()
    }

    NumberAnimation {
        id: contentYAnimation
        target: expandGroup._flickable
        property: "contentY"
        duration: expandGroup.animationDuration
    }
}
