#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = PestTox

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS += \
    application.h \
    compartment.h \
    crop.h \
    eggs_laid.h \
    general.h \
    leaf_photo_degradation.h \
    leaf_uptake.h \
    leaf_volatilization.h \
    leaf_wash_off.h \
    loss_rate.h \
    loss_rate_total.h \
    macropore_flow.h \
    organic_carbon_water_partitioning.h \
    PestTox_documentation.h \
    primary_distribution_air.h \
    primary_distribution_base.h \
    primary_distribution_leaf.h \
    primary_distribution_soil.h \
    runoff_amount.h \
    runoff_fraction.h \
    soil_structure.h \
    soil_temperature.h \
    surface_water_degradation.h \
    topsoil_degradation.h \
    topsoil_in_soil.h \
    topsoil_volatilization.h \
    topsoil_runoff.h \
    water_viscosity.h \
    zvalues.h \
    pesticide_induced_mortality.h

SOURCES += \
    application.cpp \
    compartment.cpp \
    crop.cpp \
    eggs_laid.cpp \
    leaf_photo_degradation.cpp \
    leaf_uptake.cpp \
    leaf_volatilization.cpp \
    leaf_wash_off.cpp \
    loss_rate.cpp \
    loss_rate_total.cpp \
    macropore_flow.cpp \
    organic_carbon_water_partitioning.cpp \
    PestTox_documentation.cpp \
    primary_distribution_air.cpp \
    primary_distribution_base.cpp \
    primary_distribution_leaf.cpp \
    primary_distribution_soil.cpp \
    runoff_amount.cpp \
    runoff_fraction.cpp \
    soil_structure.cpp \
    soil_temperature.cpp \    
    surface_water_degradation.cpp \
    topsoil_degradation.cpp \
    topsoil_in_soil.cpp \
    topsoil_volatilization.cpp \
    topsoil_runoff.cpp \
    water_viscosity.cpp \
    zvalues.cpp \
    pesticide_induced_mortality.cpp
