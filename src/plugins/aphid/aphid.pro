#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = aphid

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
  aphid_documentation.h \
  aphid_immigration.h \
  aphid_index.h \
  aphid_juvenile_survival.h \
  aphid_net_reproduction.h \
  aphid_offspring.h \
  biocontrol.h \
  cadaver_conversion.h \
  cadaver_time.h \
  crop_growth_stage.h \
  crop_is_growing.h \
  infection_rate.h \
  prevalence.h \
  threshold_base.h \
  threshold_norway.h \
  threshold_sweden.h \
  yield.h

SOURCES +=  \
  aphid_documentation.cpp \
  aphid_immigration.cpp \
  aphid_index.cpp \
  aphid_juvenile_survival.cpp \
  aphid_net_reproduction.cpp \
  aphid_offspring.cpp \
  biocontrol.cpp \
  cadaver_conversion.cpp \
  cadaver_time.cpp \
  crop_growth_stage.cpp \
  crop_is_growing.cpp \
  infection_rate.cpp \
  prevalence.cpp \
  threshold_base.cpp \
  threshold_norway.cpp \
  threshold_sweden.cpp \
  yield.cpp
