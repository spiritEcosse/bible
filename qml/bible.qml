import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    initialPage: books
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
//    property Component history: Component { History {} }
//    property Component comments: Component { CommentsPage {} }
//    property Component books: Component { BooksPage {} }
    property Component moduleInformation: Component { ModuleInformation {} }
    property int operationType: PageStackAction.Animated
}
