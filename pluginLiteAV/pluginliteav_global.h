#ifndef PLUGINLITEAV_GLOBAL_H
#define PLUGINLITEAV_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef PLUGINLITEAV_LIB
# define PLUGINLITEAV_EXPORT Q_DECL_EXPORT
#else
# define PLUGINLITEAV_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGINLITEAV_GLOBAL_H
