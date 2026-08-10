#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = callosobruchus

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
  callosobruchus_documentation.h \
  fecundity.h \
  stock.h \
  density_dependent.cpp

SOURCES +=  \
  callosobruchus_documentation.cpp \
  fecundity.cpp \
  stock.cpp \
  density_dependent.cpp
