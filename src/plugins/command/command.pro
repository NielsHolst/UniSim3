#-------------------------------------------------
#
# command DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = command

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    batch.h \
    clear.h \
    clip.h \
    command_documentation.h \
    debug.h \
    diagnose.h \
#    doc.h \
    edit.h \
    find.h \
    general.h \
    get.h \
    get_folders.h \
    go.h \
#    graph.h \
    head.h \
    help.h \
    help_class_base.h \
    help_class_html.h \
    help_class_plain.h \
    ht.h \
    latest.h \
    libr.h \
    list.h \
    list_options.h \
    list_output.h \
    load.h \
    location.h \
    prep.h \
    profile.h \
    reconfigure.h \
    reset.h \
    rows.h \
    run.h \
    save.h \
    set.h \
    set_font.h \
    set_folder.h \
    tail.h \
#    test.h \
#    update.h \
    vg.h \
    what.h \
    write.h \
    write_output.h \
    quit.h
	
SOURCES +=  \
    batch.cpp \
    clear.cpp \
    clip.cpp \
    command_documentation.cpp \
    debug.cpp \
#    doc.cpp \
    diagnose.cpp \
    edit.cpp \
    find.cpp \
    get.cpp \
    get_folders.cpp \
    general.cpp \
    go.cpp \
#    graph.cpp \
    head.cpp \
    help.cpp \
    help_class_base.cpp \
    help_class_html.cpp \
    help_class_plain.cpp \
    ht.cpp \
    latest.cpp \
    libr.cpp \
    list.cpp \
    list_options.cpp \
    list_output.cpp \
    load.cpp \
    location.cpp \
    prep.cpp \
    profile.cpp \
    reconfigure.cpp \
    reset.cpp \
    rows.cpp \
    run.cpp \
    save.cpp \
    set.cpp \
    set_font.cpp \
    set_folder.cpp \
    tail.cpp \
    vg.cpp \
#    test.cpp \
#    update.cpp \
    what.cpp \
    write.cpp \
    write_output.cpp \
    quit.cpp


