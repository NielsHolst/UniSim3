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
    actuator_humidifier.h \
    actuator_pad_and_fan.h \
    actuator_screen.h \
    actuator_ventilation.h \
    average_cover.h \
    average_screen.h \
    average_shelter_layer.h \
    budget.h \
    budget_layer.h \
    budget_layer_cover.h \
    budget_layer_growth_lights.h \
    budget_layer_heat_pipes.h \
    budget_layer_screen.h \
    budget_layer_sky.h \
    budget_volume.h \
    cover.h \
    energy_saving.h \
    face.h \
    faces.h \
    floor.h \
    geometry.h \
    growth_light_controller.h \
    growth_light_product.h \
    growth_lights.h \
    heat_buffer.h \
    heat_pipes.h \
    heat_pump_product.h \
    heat_pumps.h \
    heat_pumps_max_state.h \
    humidification_controller.h \
    humidifier_product.h \
    humidifiers.h \
    ig_indoors_humidity.h \
    layer.h \
    layer_as_input.h \
    layer_as_output.h \
    layer_parameters.h \
    leakage_ventilation.h \
    light_sum.h \
    outdoors.h \
    pad_and_fan_product.h \
    pad_and_fans.h \
    pid_controller.h \
    plant.h \
    plant_instrument.h \
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
    actuator_humidifier.cpp \
    actuator_pad_and_fan.cpp \
    actuator_screen.cpp \
    actuator_ventilation.cpp \
    average_cover.cpp \
    average_screen.cpp \
    average_shelter_layer.cpp \
    budget.cpp \
    budget_layer.cpp \
    budget_layer_cover.cpp \
    budget_layer_growth_lights.cpp \
    budget_layer_heat_pipes.cpp \
    budget_layer_screen.cpp \
    budget_layer_sky.cpp \
    budget_volume.cpp \
    cover.cpp \
    energy_saving.cpp \
    face.cpp \
    faces.cpp \
    floor.cpp \
    geometry.cpp \
    growth_light_controller.cpp \
    growth_light_product.cpp \
    growth_lights.cpp \
    heat_buffer.cpp \
    heat_pipes.cpp \
    heat_pump_product.cpp \
    heat_pumps.cpp \
    heat_pumps_max_state.cpp \
    humidification_controller.cpp \
    humidifier_product.cpp \
    humidifiers.cpp \
    ig_indoors_humidity.cpp \
    layer.cpp \
    layer_as_input.cpp \
    layer_as_output.cpp \
    layer_parameters.cpp \
    leakage_ventilation.cpp \
    light_sum.cpp \
    outdoors.cpp \
    pad_and_fan_product.cpp \
    pad_and_fans.cpp \
    pid_controller.cpp \
    plant.cpp \
    plant_instrument.cpp \
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
