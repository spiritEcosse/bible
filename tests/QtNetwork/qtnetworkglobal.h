#ifndef QTNETWORKGLOBAL_H
#define QTNETWORKGLOBAL_H

#include <QtCore/qglobal.h>
#include <QtNetwork/qtnetwork-config.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_NETWORK_LIB)
#    define Q_NETWORK_EXPORT Q_DECL_EXPORT
#  else
#    define Q_NETWORK_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_NETWORK_EXPORT
#endif

QT_END_NAMESPACE

#endif

