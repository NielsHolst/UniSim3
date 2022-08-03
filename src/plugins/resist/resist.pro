#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = resist

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    allele_frequency.h \
    carrying_capacity.h \
    check_vector.h \
    density.h \
    gene_frequencies.h \
    immigration.h \
    population.h \
    reproduction.h \
    proportion_bounds.h \
    reproductive_rate.h \
    spray_survival.h
	
SOURCES +=  \
    allele_frequency.cpp \
    carrying_capacity.cpp \
    density.cpp \
    gene_frequencies.cpp \
    immigration.cpp \
    population.cpp \
    reproduction.cpp \
    proportion_bounds.cpp \
    reproductive_rate.cpp \
    spray_survival.cpp


