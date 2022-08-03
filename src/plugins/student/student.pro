#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = student

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    fibonacci.h \
    jump.h \
    student_documentation.h
SOURCES +=  \
    fibonacci.cpp \
    jump.cpp \
    student_documentation.cpp
