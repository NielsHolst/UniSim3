#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = bph

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    bph_reproduction.h \
    bph_survival.h \
    immigrant_oviposition.h

SOURCES +=  \
    bph_reproduction.cpp \
    bph_survival.cpp \
    immigrant_oviposition.cpp


