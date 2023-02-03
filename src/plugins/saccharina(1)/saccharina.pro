#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = saccharina

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    allocation.h \
    demand_cost.h \
    demand_exudation.h \
    demand_respiration.h \
    demand_growth.h \
    demand_reserves.h \
    plant.h \
    plant_geometry.h \
    plant_reset.h \
    saccharina_documentation.h \
    scaling_nitrogen.h \
    scaling_salinity.h \
    scaling_temperature.h \
    supply_carbon.h \
    supply_nitrogen.h \
    supply_resorption.h

SOURCES +=  \
    allocation.cpp \
    demand_cost.cpp \
    demand_exudation.cpp \
    demand_respiration.cpp \
    demand_growth.cpp \
    demand_reserves.cpp \
    plant.cpp \
    plant_geometry.cpp \
    plant_reset.cpp \
    saccharina_documentation.cpp \
    scaling_nitrogen.cpp \
    scaling_salinity.cpp \
    scaling_temperature.cpp \
    supply_carbon.cpp \
    supply_nitrogen.cpp \
    supply_resorption.cpp

