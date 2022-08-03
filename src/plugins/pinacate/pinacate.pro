#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = pinacate

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    fecundity.h \
    birth.h \
    herbivore.h \
    vegetation.h \
    predator.h
	
SOURCES +=  \
    fecundity.cpp \
    birth.cpp \
    herbivore.cpp \
    vegetation.cpp \
    predator.cpp
