import QtQuick 2.0
import Sailfish.Silica 1.0

ExpandingSection {
//    property int currentIndexParent: _group.parentGroup ? _group.parentGroup.currentIndex : -1
    property int currentIndexParent: -1
    property var obj

    onCurrentIndexParentChanged: {
        if (currentIndexParent == -1) {
            _group.currentIndex = -1;
        } else {
            expanded = false;
        }
    }
}
