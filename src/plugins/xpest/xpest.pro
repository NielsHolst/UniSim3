#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = xpest

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
  aphid_assimilate.h \
  aphid_rue.h \
  crop_component.h \
  damage.h \
  day_degree_scale.h \
  growth.h \
  growth_stage.h \
  reduction.h \
  remaining_partition.h \
  remobilization.h \
  rust_damage.h \
  weeds.h \
  xpest_documentation.h 
    
SOURCES +=  \
  aphid_assimilate.cpp \
  aphid_rue.cpp \
  crop_component.cpp \
  damage.cpp \
  day_degree_scale.cpp \
  growth.cpp \
  growth_stage.cpp \
  reduction.cpp \
  remaining_partition.cpp \
  remobilization.cpp \
  rust_damage.cpp \
  weeds.cpp \
  xpest_documentation.cpp
