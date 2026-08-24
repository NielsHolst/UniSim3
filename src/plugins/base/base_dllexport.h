#ifndef BASE_DLLEXPORT_H
#define BASE_DLLEXPORT_H

#if defined(BUILD_DLL)
#define UNI_API __declspec(dllexport)
#else
#define UNI_API __declspec(dllimport)
#endif

#endif // BASE_DLLEXPORT_H
