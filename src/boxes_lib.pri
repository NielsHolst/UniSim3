#-------------------------------------------------
#
# Project to include in all dynamic library projects
# 
# Usage:
# BOXES_PLUGIN_NAME = unisimlib
# include("$$_PRO_FILE_PWD_/../../boxes_lib.pri")
# HEADERS += ...
# SOURCES += ...
# 
#-------------------------------------------------

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")
QT += widgets

# What we are building
TEMPLATE = lib
TARGET = $$BOXES_PLUGIN_NAME$${SUFFIX}
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"

# Own library source code on which we depend
DEPENDPATH += $$_PRO_FILE_PWD_/../base

# Additional folders with header files
INCLUDEPATH += "$$_PRO_FILE_PWD_/../../plugins"



