#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = MusselBed

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS += \
    MusselBed_documentation.h \
    functional_response_g_b.h \
    mussel.h \
    mussel_growth_rate.h \
    mussel_sal_scale.h \
    mussel_temp_scale.h \
    mussel_thinning.h \
    starfish.h \
    starfish_demand.h \
    starfish_hd_scale.h \
    starfish_sal_scale.h \
    starfish_search_rate.h \
    starfish_temp_scale.h

SOURCES += \
    MusselBed_documentation.cpp \
    functional_response_g_b.cpp \
    mussel.cpp \
    mussel_growth_rate.cpp \
    mussel_sal_scale.cpp \
    mussel_temp_scale.cpp \
    mussel_thinning.cpp \
    starfish.cpp \
    starfish_demand.cpp \
    starfish_hd_scale.cpp \
    starfish_sal_scale.cpp \
    starfish_search_rate.cpp \
    starfish_temp_scale.cpp


