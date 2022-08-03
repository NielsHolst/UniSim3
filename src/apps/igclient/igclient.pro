#-------------------------------------------------
#
#  app
#
#-------------------------------------------------
BOXES_APP_NAME = igclient

include("$$_PRO_FILE_PWD_/../../boxes_app.pri")

# Qt libraries that we use
QT     -= gui
QT     += xml
CONFIG += console
LIBS   += -L$$_PRO_FILE_PWD_/../../../bin -liglib$${SUFFIX}

# Where to look for iglib headers
INCLUDEPATH += $$_PRO_FILE_PWD_/../../lib

# Our project source code
HEADERS = \
    query_reader_json.h

SOURCES +=  \
    main.cpp \
    query_reader_json.cpp

