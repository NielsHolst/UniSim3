#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = cmbp

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS += \
    cmbp_documentation.h \
    dev_rate.h \
    emigration.h \
    exponential_peak.h \
    fecundity.h \
    grain_moisture_content.h \
    grain_store.h \
    immigration.h \
    survival_by_density.h
		
SOURCES += \
    cmbp_documentation.cpp \
    dev_rate.cpp \
    emigration.cpp \
    exponential_peak.cpp \
    fecundity.cpp \
    grain_moisture_content.cpp \
    grain_store.cpp \
    immigration.cpp \
    survival_by_density.cpp


