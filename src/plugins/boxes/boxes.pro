#-------------------------------------------------
#
# boxes DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = boxes

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

QT += widgets

HEADERS +=  \
    accumulator.h \
    all_types.h \
    average.h \
    boxes_documentation.h \
    briere.h \
    buffer.h \
    calendar.h \
    combinations.h \
    computation.h \
    date.h \
    date_split.h \
    day_degrees.h \
    date_time_signal.h \
    demand_budget.h \
    dew_point.h \
    distributed_delay_base.h \
    distributed_delay.h \
    distributed_delay_2d.h \
    fixed_signal.h \
    food_web.h \
    food_web_calc_base.h \
    food_web_calc_parasite_host.h \
    food_web_calc_predator_prey.h \
    functional_response.h \
    host.h \
    hump.h \
    layout_r.h \
    maker.h \
    maximum.h \
    maximum_at.h \
    mean.h \
    message.h \
    minimum.h \
    multiplum.h \
    numbered_file.h \
    offset_date_time.h \
    on_off.h \
    on_off_base.h \
    on_off_by_date.h \
    output_r.h \
    output_selector.h \
    output_text.h \
    output_writer.h \
    page_r.h \
    parasite.h \
    plot_r.h \
    pop_up.h \
    population.h \
    predator.h \
    prey.h \
    priority_signal.h \
    proportional_signal.h \
    randomiser_base.h \
    randomiser_monte_carlo.h \
    randomiser_sobol_sequence.h \
    randomiser_stratified.h \
    random_base.h \
    random_base_typed.h \
    random_binomial.h \
    random_log_normal.h \
    random_log_uniform.h \
    random_normal.h \
    random_uniform.h \
    random_uniform_int.h \
    ratio.h \
    records.h \
    running_base.h \
    running_average_steps.h \
    running_average_timed.h \
    running_sum_steps.h \
    running_sum_timed.h \
    sandy.h \
    scenarios.h \
    select_file.h \
    sequence_base.h \
    sequence_by_iteration.h \
    sequence_by_step.h \
    series_base.h \
    series_by_iteration.h \
    series_by_step.h \
    simulation.h \
    sine_wave_temperature.h \
    sliding_signal.h \
    sobol_g_function.h \
    solar_time.h \
    split.h \
    stage_base.h \
    stage.h \
    stage_and_phase.h \
    sum.h \
    sun.h \
    super_functional_response.h \
    supply_budget.h \
    threshold_signal.h \
    values_base.h \
    vector_scaled.h \
    vector_sum.h
	
SOURCES +=  \
    accumulator.cpp \
    all_types.cpp \
    average.cpp \
    boxes_documentation.cpp \
    briere.cpp \
    buffer.cpp \
    calendar.cpp \
    combinations.cpp \
    computation.cpp \
    date.cpp \
    date_split.cpp \
    date_time_signal.cpp \
    day_degrees.cpp \
    demand_budget.cpp \
    dew_point.cpp \
    distributed_delay_base.cpp \
    distributed_delay.cpp \
    distributed_delay_2d.cpp \
    fixed_signal.cpp \
    food_web.cpp \
    food_web_calc_base.cpp \
    food_web_calc_parasite_host.cpp \
    food_web_calc_predator_prey.cpp \
    functional_response.cpp \
    host.cpp \
    hump.cpp \
    layout_r.cpp \
    maker.cpp \
    maximum.cpp \
    maximum_at.cpp \
    minimum.cpp \
    mean.cpp \
    message.cpp \
    multiplum.cpp \
    numbered_file.cpp \
    offset_date_time.cpp \
    on_off.cpp \
    on_off_by_date.cpp \
    output_r.cpp \
    output_selector.cpp \
    output_text.cpp \
    output_writer.cpp \
    page_r.cpp \
    parasite.cpp \
    plot_r.cpp \
    pop_up.cpp \
    population.cpp \
    predator.cpp \
    prey.cpp \
    priority_signal.cpp \
    proportional_signal.cpp \
    randomiser_base.cpp \
    randomiser_monte_carlo.cpp \
    randomiser_sobol_sequence.cpp \
    randomiser_stratified.cpp \
    random_base.cpp \
    random_binomial.cpp \
    random_log_normal.cpp \
    random_log_uniform.cpp \
    random_normal.cpp \
    random_uniform.cpp \
    random_uniform_int.cpp \
    ratio.cpp \
    records.cpp \
    running_base.cpp \
    running_average_steps.cpp \
    running_average_timed.cpp \
    running_sum_steps.cpp \
    running_sum_timed.cpp \
    sandy.cpp \
    scenarios.cpp \
    select_file.cpp \
    sequence_base.cpp \
    sequence_by_iteration.cpp \
    sequence_by_step.cpp \
    series_base.cpp \
    series_by_iteration.cpp \
    series_by_step.cpp \
    simulation.cpp \
    sine_wave_temperature.cpp \
    sliding_signal.cpp \
    sobol_g_function.cpp \
    solar_time.cpp \
    split.cpp \
    stage_base.cpp \
    stage.cpp \
    stage_and_phase.cpp \
    sum.cpp \
    sun.cpp \
    super_functional_response.cpp \
    supply_budget.cpp \
    threshold_signal.cpp \
    values_base.cpp \
    vector_scaled.cpp \
    vector_sum.cpp


