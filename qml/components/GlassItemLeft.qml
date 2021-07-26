import QtQuick 2.0
import Sailfish.Silica 1.0


GlassItem {
    color: palette.highlightColor
    falloffRadius: Math.exp(Math.log(0.1500))
    radius: Math.exp(Math.log(0.1500))
    cache: false
    anchors {
        left: parent.left
        leftMargin: -Math.round(4.5 * Theme.paddingMedium)
        topMargin: -Math.round(1.2 * Theme.paddingMedium)
    }
}
