#-------------------------------------------------
#
# Project to include in all plugin projects
# 
# Usage:
# BOXES_PLUGIN_NAME = savanna
# include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")
# HEADERS += ...
# SOURCES += ...
# 
#-------------------------------------------------

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

# What we are building
TEMPLATE = lib
CONFIG += plugin
TARGET = $$BOXES_PLUGIN_NAME$${SUFFIX}
DESTDIR = "$${BIN_PLUGINS_PATH}"
DEFINES += BOXES_PLUGIN_NAME=$$BOXES_PLUGIN_NAME
DEFINES += BOXES_PLUGIN_NAME_TEXT=\\\"$${BOXES_PLUGIN_NAME}\\\"

### # Own library source code on which we depend
### DEPENDPATH += $$_PRO_FILE_PWD_/../base

# Additional folders with header files
INCLUDEPATH += "$$_PRO_FILE_PWD_/.."

# Common plugin headers and sources
HEADERS +=  \
    ../base/publish.h \
    ../base/factory.h

SOURCES +=  \
    ../base/publish.cpp \
    ../base/factory.cpp



