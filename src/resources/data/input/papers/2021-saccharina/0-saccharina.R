sim = read_output(output_file_name)
if (exists("plot_all")) plot_all(sim)

sim$date = as.Date(sim$date)

obs = read.table(paste0(box_script_folder,"/obs_avg.txt"), header=TRUE, sep="\t")
obs$Date = ymd(obs$Date)

replace_obs_name = function(old_name, new_name) {
  colnames(obs)[colnames(obs)==old_name] <<- new_name
}

colnames(obs)
replace_obs_name("Date", "date")
replace_obs_name("Wtotal", "biomassTotal")
replace_obs_name("Wstruct", "biomassStruct")
replace_obs_name("A", "area")
replace_obs_name("L", "length")
replace_obs_name("Cconc", "concC")
replace_obs_name("Nconc", "concN")
colnames(obs)

calib_cols = c(
  "date",
  "biomassTotal",
  "biomassStruct",
  "area",
  "length",
  "concC",
  "concN")
  
sim = sim[,calib_cols]
obs = obs[,calib_cols]
M = melt(sim, id.vars="date")
M2 = melt(obs, id.vars="date")

windows(8,5)
P = ggplot(M, aes(date, value, colour=variable)) +
  geom_line() +
  geom_point(data=M2, size=3, shape=1) +
  scale_x_date(breaks="months", date_labels="%b", name="Month") +
  facet_wrap(~variable, scales="free_y") +
  theme(legend.position='none')
print(P)
