#-------------------------------------------------
#
# Project to include in all app projects
# 
# Usage:
# BOXES_APP_NAME = test
# include("$$_PRO_FILE_PWD_/../../app.pri")
# HEADERS += ...
# SOURCES += ...
# 
#-------------------------------------------------

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

QT += widgets

# What we are building
TEMPLATE = app
#DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
DESTDIR = "$${BIN_PATH}"
TARGET = $$BOXES_APP_NAME$${SUFFIX}

# Own library source code on which we depend
DEPENDPATH += $$_PRO_FILE_PWD_/../../plugins/base

# Additional folders with header files
INCLUDEPATH += "$$_PRO_FILE_PWD_/../../plugins"

# Load application icon
RC_FILE = "$$_PRO_FILE_PWD_/../../resources/unisim.rc"

# Load file resource
RESOURCES = "$$_PRO_FILE_PWD_/../../resources/application.qrc"

# Load icon on OS X
ICON = ../icon/squirrel.icns

