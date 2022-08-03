#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = eq

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")
#message("QMAKE_CXXFLAGS")
#message($$QMAKE_CXXFLAGS)

#message("QMAKE_CXXFLAGS_DEBUG")
#message($$QMAKE_CXXFLAGS_DEBUG)

#message("QMAKE_CXXFLAGS_RELEASE")
#message($$QMAKE_CXXFLAGS_RELEASE)

HEADERS +=  \
    exponential.h \
    gompertz.h \
    inv_log10.h \
    log10.h \
    transition.h
	
SOURCES +=  \
    exponential.cpp \
    gompertz.cpp \
    inv_log10.cpp \
    log10.cpp \
    transition.cpp


