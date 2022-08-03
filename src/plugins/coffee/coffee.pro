#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = coffee

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
coffee.h \
coffee_documentation.h \
foliage.h \
growth.h \
light.h \
nitrogen_supply.h \
pet.h \
phenology.h \
rain_interception.h \
senescence.h \
soil_nitrogen.h \
soil_water_thresholds.h \
water_budget.h \
water_flux.h \

SOURCES +=  \
coffee.cpp \
coffee_documentation.cpp \
foliage.cpp \
growth.cpp \
light.cpp \
nitrogen_supply.cpp \
pet.cpp \
phenology.cpp \
rain_interception.cpp \
senescence.cpp \
soil_nitrogen.cpp \
soil_water_thresholds.cpp \
water_budget.cpp \
water_flux.cpp \

