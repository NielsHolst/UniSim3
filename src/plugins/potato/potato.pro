#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = potato

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
  fecundity.h \
  potato_development_at_temp.h \
  potato_development_base.h \
  potato_development_by_day.h \
  potato_development_by_hour.h \
  potato_development_we.h \
  survival.h

SOURCES +=  \
  fecundity.cpp \
  potato_development_at_temp.cpp \
  potato_development_base.cpp \
  potato_development_by_day.cpp \
  potato_development_by_hour.cpp \
  potato_development_we.cpp \
  survival.cpp
