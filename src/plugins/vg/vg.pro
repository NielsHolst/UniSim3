#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = vg

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    absolute_humidity.h \
    actuators.h \
    actuator_chalk.h \
    actuator_growth_light.h \
    actuator_heat_pump.h \
    actuator_heat_pipe.h \
    actuator_screen.h \
    actuator_ventilation.h \
    average_cover.h \
    average_screen.h \
    average_shelter_layer.h \
    budget.h \
    budget_layer.h \
    budget_layer_cover.h \
    budget_layer_floor.h \
    budget_layer_screen.h \
    budget_layer_sky.h \
    budget_volume.h \
    canopy_photosynthesis.h \
    cover.h \
    energy_saving.h \
    face.h \
    faces.h \
    floor.h \
    geometry.h \
    growth_light_controller.h \
    growth_light_product.h \
    growth_lights.h \
    heat_pipes.h \
    heat_pump_product.h \
    heat_pumps_max_state.h \
    ig_indoors_humidity.h \
    layer.h \
    layer_parameters.h \
    leakage_ventilation.h \
    light_sum.h \
    outdoors.h \
    pid_controller.h \
    pipe_forced.h \
    plant.h \
    screen.h \
    sensor.h \
    shading.h \
    shading_agent.h \
    shelter.h \
    shelter_layers.h \
    sky.h \
    soil_temperature.h \
    summary.h \
    u_wind.h \
    vg_documentation.h

SOURCES += \
    absolute_humidity.cpp \
    actuators.cpp \
    actuator_chalk.cpp \
    actuator_growth_light.cpp \
    actuator_heat_pipe.cpp \
    actuator_heat_pump.cpp \
    actuator_screen.cpp \
    actuator_ventilation.cpp \
    average_cover.cpp \
    average_screen.cpp \
    average_shelter_layer.cpp \
    budget.cpp \
    budget_layer.cpp \
    budget_layer_cover.cpp \
    budget_layer_floor.cpp \
    budget_layer_screen.cpp \
    budget_layer_sky.cpp \
    budget_volume.cpp \
    canopy_photosynthesis.cpp \
    cover.cpp \
    energy_saving.cpp \
    face.cpp \
    faces.cpp \
    floor.cpp \
    geometry.cpp \
    growth_light_controller.cpp \
    growth_light_product.cpp \
    growth_lights.cpp \
    heat_pipes.cpp \
    heat_pump_product.cpp \
    heat_pumps_max_state.cpp \
    ig_indoors_humidity.cpp \
    layer.cpp \
    layer_parameters.cpp \
    leakage_ventilation.cpp \
    light_sum.cpp \
    outdoors.cpp \
    pid_controller.cpp \
    plant.cpp \
    screen.cpp \
    sensor.cpp \
    shading.cpp \
    shading_agent.cpp \
    shelter.cpp \
    shelter_layers.cpp \
    sky.cpp \
    summary.cpp \
    soil_temperature.cpp \
    u_wind.cpp \
    vg_documentation.cpp
