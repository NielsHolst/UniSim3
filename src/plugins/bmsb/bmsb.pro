#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = bmsb

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    beta_rate.h \
    briere2_dev_rate.h \
    exp_owsurv_rate.h \
    fecundity_evf.h \
    nrr_evf.h \
    poly2_model.h \
    poly3_rate.h \
    poly4_dev_rate.h \
    schoolfield_dev_rate.h \
    schoolfieldhigh_dev_rate.h \
    schoolfieldlow_dev_rate.h \
    sharpedemichele_surv_rate.h \
    soep_mort_rate.h \
    ssi_dev_rate.h \
    taylor_dev_rate.h \
    lin_dev_rate.h \
    exppoly2_mort_rate.h \
    exppoly4_mort_rate.h \
    extremeval_func.h
    
SOURCES +=  \
    beta_rate.cpp \
    briere2_dev_rate.cpp \
    exp_owsurv_rate.cpp \
    fecundity_evf.cpp \
    nrr_evf.cpp \
    poly2_model.cpp \
    poly3_rate.cpp \
    poly4_dev_rate.cpp \
    schoolfield_dev_rate.cpp \
    schoolfieldhigh_dev_rate.cpp \
    schoolfieldlow_dev_rate.cpp \
    sharpedemichele_surv_rate.cpp \
    soep_mort_rate.cpp \
    ssi_dev_rate.cpp \
    taylor_dev_rate.cpp \
    lin_dev_rate.cpp \
    exppoly2_mort_rate.cpp \
    exppoly4_mort_rate.cpp \
    extremeval_func.cpp



