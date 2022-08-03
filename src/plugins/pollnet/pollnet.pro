#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = pollnet

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    big_bang.h \
    mediator.h \
    plant.h \
    pollinator.h \

	
SOURCES +=  \
    big_bang.cpp \
    mediator.cpp \
    plant.cpp \
    pollinator.cpp \

