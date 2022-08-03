#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = cowpea

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    ageing_increment.h \
    daily_larval_demand.h \
    daily_larval_mortality.h \
    demand_per_capita.h
#    attract.h \
#    total_parasitism.h \
#    biomasscowpea.h \
#    cowpea_population.h \
#    degrees_to_days.h \
#    exponential_growth.h \
#    food_web_population.h \
#    flower_demand.h \
#    in_grams.h \
#    in_grams2.h \
#    potential_larva.h \
#    increment.h \
#    initial_biomass.h \
#    li.h \
#    rgr.h \
#    single_biomass.h \
#    threshold.h
    
SOURCES +=  \
    ageing_increment.cpp \
    daily_larval_demand.cpp \
    daily_larval_mortality.cpp \
    demand_per_capita.cpp
#    attract.cpp \
#    total_parasitism.cpp \
#    biomasscowpea.cpp \
#    cowpea_population.cpp \
#    degrees_to_days.cpp \
#    exponential_growth.cpp \
#    flower_demand.cpp \
#    food_web_population.cpp \
#    in_grams.cpp \
#    in_grams2.cpp \
#    potential_larva.cpp \
#    increment.cpp \
#    initial_biomass.cpp \
#    li.cpp \
#    rgr.cpp \
#    single_biomass.cpp \
#    threshold.cpp


