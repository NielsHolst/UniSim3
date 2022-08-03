#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = demo

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    big_bang.h \
    island.h \
    migration.h \
    population.h
#    predation.h \
#    predator.h \
#    prey.h

SOURCES +=  \
    big_bang.cpp \
    island.cpp \
    migration.cpp \
    population.cpp
#    predation.cpp \
#    predator.cpp \
#    prey.cpp

