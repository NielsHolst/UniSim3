#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = rvf

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    attack_status.h \
    lamb_slaughter.h \
    mosquito_fecundity.h \
    sheep_fecundity.h \
    transmission_to_host.h \
    transmission_to_vector.h \
    water_level.h

SOURCES += \
    attack_status.cpp \
    lamb_slaughter.cpp \
    mosquito_fecundity.cpp \
    sheep_fecundity.cpp \
    transmission_to_host.cpp \
    transmission_to_vector.cpp \
    water_level.cpp
