import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {

    Image {
        opacity: .30
        anchors{
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: Theme.paddingMedium * 5
        }
        source: "harbour-freebible.png"
    }

    Label {
        id: label
        anchors.centerIn: parent
        text: {
            "FreeBible"
        }
    }
}
