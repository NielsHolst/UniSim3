#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = vg

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    actuator_chalk.h \
    actuator_growth_light.h \
    actuators.h \
    actuator_heat_pipe.h \
    actuator_screen.h \
    actuator_ventilation.h \
    average_cover.h \
    average_cover_or_screen.h \
    average_material.h \
    average_screen.h \
    budget.h \
    budget_layer.h \
    budget_layer_cover.h \
    budget_layer_floor.h \
    budget_layer_screen.h \
    budget_layer_sky.h \
    budget_volume.h \
    canopy_photosynthesis.h \
    controllers.h \
    cover.h \
    crop.h \
    floor.h \
    geometry.h \
    growth_light_controller.h \
    growth_lights.h \
    heat_pipes.h \
    ig_indoors_humidity.h \
    layer.h \
    layer_adjusted.h \
    leakage_ventilation.h \
    outdoors.h \
    pid_controller.h \
    pipe_forced.h \
    plant.h \
    screen.h \
    sensor.h \
    shelter.h \
    sky.h \
    soil_temperature.h \
    summary.h \
    u_wind.h \
    vg_documentation.h

SOURCES += \
    actuator_chalk.cpp \
    actuator_growth_light.cpp \
    actuators.cpp \
    actuator_heat_pipe.cpp \
    actuator_heat_pump.cpp \
    actuator_screen.cpp \
    actuator_ventilation.cpp \
    average_cover.cpp \
    average_cover_or_screen.cpp \
    average_material.cpp \
    average_screen.cpp \
    budget.cpp \
    budget_layer.cpp \
    budget_layer_cover.cpp \
    budget_layer_floor.cpp \
    budget_layer_screen.cpp \
    budget_layer_sky.cpp \
    budget_volume.cpp \
    canopy_photosynthesis.cpp \
    controllers.cpp \
    cover.cpp \
    crop.cpp \
    floor.cpp \
    geometry.cpp \
    growth_light_controller.cpp \
    growth_lights.cpp \
    heat_pipes.cpp \
    ig_indoors_humidity.cpp \
    layer.cpp \
    layer_adjusted.cpp \
    leakage_ventilation.cpp \
    outdoors.cpp \
    pid_controller.cpp \
    plant.cpp \
    screen.cpp \
    sensor.cpp \
    shelter.cpp \
    sky.cpp \
    summary.cpp \
    soil_temperature.cpp \
    u_wind.cpp \
    vg_documentation.cpp
