import Sailfish.Silica 1.0
import QtQuick 2.0
import "pages"
import "cover"

ApplicationWindow
{
    id: app
    initialPage: main
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
//    property Component history: Component { History {} }
    property Component commentsPage: Component { CommentsPage {} }
    property Component main: Component { Main {} }
    property Component moduleInformation: Component { ModuleInformation {} }
    property int operationType: PageStackAction.Animated
    cover: Component { CoverPage {} }
}
