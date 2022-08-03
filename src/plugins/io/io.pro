#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = io

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS += \
    log_logistic.h \
    pollen.h \
    pollen_onset_date_base.h \
#    pollen_onset_date_from_maize_obs.h \
    pollen_onset_date_from_trap_obs.h

SOURCES += \
    log_logistic.cpp \
    pollen.cpp \
    pollen_onset_date_base.cpp \
#    pollen_onset_date_from_maize_obs.cpp \
    pollen_onset_date_from_trap_obs.cpp

