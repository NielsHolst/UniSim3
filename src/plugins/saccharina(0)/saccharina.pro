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
    area_erosion.h \
    beta_limits.h \
    biomass.h \
    calibration.h \
    carbon_init_limits.h \
    carbon_reserves_max.h \
    controlled_environment.h \
    controlled_environment_base.h \
    controlled_environment_chapman.h \
    demand_carbon_erosion.h \
    demand_carbon_exudation.h \
    demand_carbon_reserves.h \
    demand_carbon_respiration.h \
    demand_carbon_structure.h \
    demand_nitrogen_erosion.h \
    demand_nitrogen_reserves.h \
    demand_nitrogen_structure.h \
    f_current.h \
    f_nitrogen.h \
    f_salinity.h \
    growth_rate.h \
    lab_calibration.h \
    light_absorption_canopy.h \
    light_absorption_leaf.h \
    pmax_minimum.h \
    nitrogen_uptake.h \
    photosynthesis.h \
    reserves.h \
    saccharina_documentation.h \
    structure.h \
    trapezoid.h
    
SOURCES +=  \
    allocation.cpp \
    area.cpp \
    area_erosion.cpp \
    beta_limits.cpp \
    biomass.cpp \
    calibration.cpp \
    carbon_init_limits.cpp \
    carbon_reserves_max.cpp \
    controlled_environment.cpp \
    controlled_environment_base.cpp \
    controlled_environment_chapman.cpp \
    demand_carbon_erosion.cpp \
    demand_carbon_exudation.cpp \
    demand_carbon_reserves.cpp \
    demand_carbon_respiration.cpp \
    demand_carbon_structure.cpp \
    demand_nitrogen_erosion.cpp \
    demand_nitrogen_reserves.cpp \
    demand_nitrogen_structure.cpp \
    f_current.cpp \
    f_nitrogen.cpp \
    f_salinity.cpp \
    growth_rate.cpp \
    lab_calibration.cpp \
    light_absorption_canopy.cpp \
    light_absorption_leaf.cpp \
    pmax_minimum.cpp \
    nitrogen_uptake.cpp \
    photosynthesis.cpp \
    reserves.cpp \
    saccharina_documentation.cpp \
    structure.cpp \
    trapezoid.cpp
