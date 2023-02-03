#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = saccharina

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    allocation.h \
    area.h \
    calibration.h \
    demand_building.h \
    demand_exudation.h \
    demand_structure.h \
    demand_maintenance.h \
    demand_store.h \
    demand_total.h \
    density.h \
    plant.h \
    plant_compartment.h \
    proportions.h \
    saccharina_documentation.h \
    scaling_nitrogen.h \
    scaling_phosphorus.h \
    scaling_salinity.h \
    scaling_temperature.h \
    supply_base.h \
    supply_carbon.h \
    supply_nitrogen.h \
    supply_phosphorus.h

SOURCES +=  \
    allocation.cpp \
    area.cpp \
    calibration.cpp \
    demand_building.cpp \
    demand_exudation.cpp \
    demand_structure.cpp \
    demand_maintenance.cpp \
    demand_store.cpp \
    demand_total.cpp \
    density.cpp \
    plant.cpp \
    plant_compartment.cpp \
    proportions.cpp \
    saccharina_documentation.cpp \
    scaling_nitrogen.cpp \
    scaling_phosphorus.cpp \
    scaling_salinity.cpp \
    scaling_temperature.cpp \
    supply_base.cpp \
    supply_carbon.cpp \
    supply_nitrogen.cpp \
    supply_phosphorus.cpp
