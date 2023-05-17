#-------------------------------------------------
#
# Project to include in all boxes projects
# 
# Usage
# include("$$_PRO_FILE_PWD_/../../boxes_common.pri") 
#-------------------------------------------------

# AUTO-CONFIG-BEGIN
CONFIG += release
VERSION = 3.0.9
CONFIG += skip_target_version_ext
# AUTO-CONFIG-END

CONFIG += skip_target_version_ext   # Simplify suffix on Mac
CONFIG += sdk_no_version_check      # Avoid SDK check on Mac

# Set suffix 'd' for debug version
CONFIG(debug, debug|release) {
    SUFFIX = d
}
else {
    SUFFIX =
}

# What we are building
CONFIG += c++17
QT += core widgets
#contains(QT_ARCH, i386) {
#    BIN = bin32
#} else {
#    BIN = bin
#}
BIN = bin
BIN_PATH = $$join(_PRO_FILE_PWD_, , , /../../../)
BIN_PATH = $$join(BIN_PATH, , , $${BIN})
BIN_PLUGINS_PATH = $$join(BIN_PATH, , , /plugins)
#message("Target folders for binaries")
#message($${BIN_PATH})
#message($${BIN_PLUGINS_PATH})

# Module winextras not yet implemented in Qt version 6
win32:lessThan(QT_MAJOR_VERSION, 6) {
    QT += winextras
}

# Define macro to generate spirit debug output on cerr
# DEFINES += BOOST_SPIRIT_X3_DEBUG

# Show warning when using deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS

# Compiler options add-ons
QMAKE_CXXFLAGS += -Wall -Wextra # -Wconversion

# Compiler options to silence warnings
QMAKE_CXXFLAGS += -Wno-padded -Wno-int-in-bool-context -Wno-maybe-uninitialized

#greaterThan(QT_MAJOR_VERSION, 5) {
#    QMAKE_CXXFLAGS += -Wno-clazy-non-pod-global-static
#}

# Compiler options to silence warnings when compiling Boost
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs -Wno-attributes -Wno-deprecated-declarations -Wno-misleading-indentation -Wno-nonnull
DEFINES += BOOST_ALLOW_DEPRECATED_HEADERS
DEFINES += BOOST_MATH_DISABLE_DEPRECATED_03_WARNING

# Compiler options to silence warnings when compiling under Mac OS X
MY_HASH = $$LITERAL_HASH
NO_PRAGMA_MESSAGES = $$join(MY_HASH, , -Wno-\\, pragma-messages)
macx:QMAKE_CXXFLAGS += $${NO_PRAGMA_MESSAGES} -Wno-inconsistent-missing-override -Wno-unknown-warning-option

# Compiler options to silence warnings when compiling under Win and Linux
!macx:QMAKE_CXXFLAGS += -Wno-unknown-pragmas

# Compiler options for extra debug info (can be toggled off)
QMAKE_CXXFLAGS_DEBUG -= -g
QMAKE_CXXFLAGS_DEBUG += -g3

# Compiler options to speed up code
QMAKE_CXXFLAGS_RELEASE += -ffast-math -O2

# Compiler option that would have saved me some days' work
QMAKE_CXXFLAGS += -Wdelete-non-virtual-dtor


# Additional folders with header files
BOOST_PATH = $$(BOOST_ROOT)
isEmpty(BOOST_PATH) {
    BOOST_PATH = $$_PRO_FILE_PWD_/../../../../boost
}
INCLUDEPATH += $${BOOST_PATH}
message("Using boost libraries found in")
message($${BOOST_PATH})

# Own libraries that we use, except 'base' does not use itself
!equals(BOXES_PLUGIN_NAME, "base") {
#    win32:LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -luniversal_simulator_base$${SUFFIX}
#    unix:LIBS += -L$$_PRO_FILE_PWD_/../../../bin/ -luniversal_simulator_base$${SUFFIX}
    win32:LIBS += -L"$${BIN_PATH}" -luniversal_simulator_base$${SUFFIX}
    unix:LIBS  += -L"$${BIN_PATH}" -luniversal_simulator_base$${SUFFIX}
    macx:LIBS  += -L"$$(HOME)/lib" -luniversal_simulator_base$${SUFFIX}
}

