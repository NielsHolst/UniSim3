sim = read_output(output_file_name)
sim$date = as.Date(sim$date)
plot_all(sim)