#ifndef BASE_DLLEXPORT_H
#define BASE_DLLEXPORT_H

#include <QtCore/QtGlobal>

#if defined(BUILD_DLL)
#define UNI_API Q_DECL_EXPORT
#else
#define UNI_API Q_DECL_IMPORT
#endif

#endif // BASE_DLLEXPORT_H
