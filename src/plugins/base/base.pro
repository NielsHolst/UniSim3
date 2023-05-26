#-------------------------------------------------
#
# base DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = base

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

QT += widgets

# What we are building
TEMPLATE = lib
#CONFIG += lib_bundle
#win32:DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
#unix:DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
win32:DESTDIR = "$${BIN_PATH}"
unix:DESTDIR  = "$${BIN_PATH}"
macx:DESTDIR = "~/lib"
TARGET = universal_simulator_base$${SUFFIX}



# Our project header and source code
HEADERS +=  \
    altova_xml.h \
    bare_date.h \
    base_signal.h \
    box.h \
    box_builder.h \
    boxscript.h \
    boxscript_ast.h \
    boxscript_ast_adapted.h \
    boxscript_config.h \
    boxscript_def.h \
    boxscript_error_handler.h \
    boxscript_parser.h \
    boxscript_preprocessor.h \
    boxscript_skipper.h \
    caller.h \
    convert.h \
    convert_operator.h \
    circular_buffer.h \
    command.h  \
    command_help.h  \
    computation.h \
    copy_folder.h \
    data_frame.h \
    data_grid.h \
    dialog.h \
    dialog_base.h \
    dialog_minimal.h \
    dialog_quiet.h \
    dialog_stub.h \
    dialog_widget.h \
    documentation.h \
    environment.h \
    exception.h \
    expression.h \
    factory_plug_in.h \
    flags.h \
#    food_web_box_base.h \
    history.h \
    interpolate.h \
    logger.h \
    matrix.h \
    matrix2D.h \
    mega_factory.h \
    minimise.h \
    monitor.h \
    node.h \
    organisation.h \
    operate.h \
    operator.h \
    path.h \
    port.h \
    port_status.h \
    port_type.h \
    product.h \
    product_base.h \
    physiological_time.h \
    phys_math.h \
    reader_base.h \
    reader_boxscript.h \
    reader_xml.h \
    reader_xml_strategy_base.h \
    reader_xml_strategy_generic.h \
    resolved_references.h \
    proportions.h \
    psychrolib.h \
    random_generator.h \
    random_order.h \
    save_grammar_atom.h \
    save_grammar_base.h \
    save_grammar_notepad.h \
#    save_graph_base.h \
#    save_graph_graphviz.h \
    success.h \
    table.h \
    test_num.h \
    to_q_string_list.h \
    time_unit.h \
    timer.h \
    unique_names.h \
    value.h \
    value_collection.h \
    value_typed.h \
    vector_op.h \
    version.h \
    win_taskbar_progress.h

SOURCES +=  \
    altova_xml.cpp \
    base_signal.cpp \
    bare_date.cpp \
    box.cpp \
    box_builder.cpp \
    boxscript.cpp \
    boxscript_ast.cpp \
    boxscript_parser.cpp \
    boxscript_preprocessor.cpp \
    caller.cpp \
    convert.cpp \
    convert_operator.cpp \
    command.cpp \
    command_help.cpp  \
    computation.cpp \
    copy_folder.cpp \
    data_frame.cpp \
    data_grid.cpp \
    dialog.cpp \
    dialog_base.cpp \
    dialog_minimal.cpp \
    dialog_quiet.cpp \
    dialog_stub.cpp \
    dialog_widget.cpp \
    documentation.cpp \
    environment.cpp \
    exception.cpp \
    expression.cpp \
    history.cpp \
    logger.cpp \
    mega_factory.cpp \
    node.cpp \
    operate.cpp \
    operator.cpp \
    path.cpp \
    physiological_time.cpp \
    phys_math.cpp \
    port.cpp \
    port_status.cpp \
    port_type.cpp \
    product_base.cpp \
    proportions.cpp \
    psychrolib.c \
    random_generator.cpp \
    random_order.cpp \
    reader_base.cpp \
    reader_boxscript.cpp \
    reader_xml.cpp \
    reader_xml_strategy_base.cpp \
    reader_xml_strategy_generic.cpp \
    resolved_references.cpp \
    save_grammar_atom.cpp \
    save_grammar_base.cpp \
    save_grammar_notepad.cpp \
#    save_graph_base.cpp \
#    save_graph_graphviz.cpp \
    table.cpp \
    test_num.cpp \
    time_unit.cpp \
    timer.cpp \
    unique_names.cpp \
    value.cpp \
    value_collection.cpp \
    vector_op.cpp \
    version.cpp

