#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = apis

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    energy_budget.h \
    fecundity.h \
    forager_recruitment.h \
    honey_demand_growth.h \
    honey_demand_respiration.h \
    honey_store.h \
    honey_supply.h \
    honey_supply_growth.h \
    life_stage.h \
    nectar_flow.h
	
SOURCES +=  \
    energy_budget.cpp \
    fecundity.cpp \
    forager_recruitment.cpp \
    honey_demand_growth.cpp \
    honey_demand_respiration.cpp \
    honey_store.cpp \
    honey_supply.cpp \
    honey_supply_growth.cpp \
    life_stage.cpp \
    nectar_flow.cpp

