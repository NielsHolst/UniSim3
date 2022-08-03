#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = tuta

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    Capacity.h\
    Tomato.h\
    Parasitoid.h\
    NesidiocorisSurvival.h\
    TutaSurvival.h\
    NesidiocorisFecundity.h\
    TutaFecundity.h\
    Nesidiocoris.h\
    Tuta.h
    
SOURCES +=  \
    Capacity.cpp\
    Tomato.cpp\
    Parasitoid.cpp\
    NesidiocorisSurvival.cpp\
    TutaSurvival.cpp\
    NesidiocorisFecundity.cpp\
    TutaFecundity.cpp\
    Nesidiocoris.cpp\
    Tuta.cpp