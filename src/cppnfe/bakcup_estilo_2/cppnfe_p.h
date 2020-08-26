#ifndef CPPNFE_GLOBAL_H
#define CPPNFE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CPPNFE_LIBRARY)
#  define CPPNFE_EXPORT Q_DECL_EXPORT
#else
#  define CPPNFE_EXPORT Q_DECL_IMPORT
#endif

#endif // CPPNFE_GLOBAL_H
