#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = collembola

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
  collembola_documentation.h \
  food_availability.h \
  logistic_growth.h \

SOURCES +=  \
  collembola_documentation.cpp \
  food_availability.cpp \
  logistic_growth.cpp \
