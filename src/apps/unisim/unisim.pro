#-------------------------------------------------
#
# unisim app
#
#-------------------------------------------------
BOXES_APP_NAME = unisim

include("$$_PRO_FILE_PWD_/../../boxes_app.pri")

HEADERS +=  \
    main_window.h

SOURCES +=  \
    main.cpp \
    main_window.cpp


# Generate QRC resource file
QRC_FILE = $$clean_path($$_PRO_FILE_PWD_/../../resources/application.qrc)
QRC_SCRIPT = $$clean_path($$_PRO_FILE_PWD_/../../generate_qrc.R)

win32 {
    RSCRIPT_CHECK = $$system(where Rscript 2>NUL)

    isEmpty(RSCRIPT_CHECK) {
        error(Rscript was not found. You need to add the R bin folder containing Rscript.exe to the system PATH.)
    }

    system(Rscript $$quote($$QRC_SCRIPT))
}

unix {
    RSCRIPT_CHECK = $$system(command -v Rscript)

    isEmpty(RSCRIPT_CHECK) {
        error(Rscript was not found. You need to add the R bin folder containing Rscript to PATH.)
    }

    system(Rscript $$quote($$QRC_SCRIPT))
}

exists($$QRC_FILE) {
    RESOURCES += $$QRC_FILE
} else {
    error(QRC generation failed: $$QRC_FILE does not exist)
}
