#-------------------------------------------------
#
# unisimlib DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = iglib
include("$$_PRO_FILE_PWD_/../../boxes_lib.pri")

# File resources
# RESOURCES = "$$_PRO_FILE_PWD_/../../resources/igclient.qrc"

HEADERS = \
    iglib.h

SOURCES = \
    iglib.cpp
