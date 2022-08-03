sim = read_output(output_file_name)
names(sim)[names(sim)=="date"] = "Date"

file_name = paste(box_script_folder, "env-AnGiang.txt", sep="/")
obs = read.table(file_name, header=TRUE, stringsAsFactors=FALSE)
obs$Date = as.POSIXct(ymd(obs$Date))
obs$Adult[obs$Adult==0] = NA

season_label = function(season) {
  n = length(season)
  ix_mid = floor((n+1)/2)
  label = rep(NA, n)
  the_season = season[1]
  if (the_season != 0) label[ix_mid] = the_season
  label
}

show_sim = function() {
  from = min(sim$Date)
  to   = max(sim$Date)
  M = subset(obs, Date>=from & Date<=to)
  M = ddply(M, .(Season), mutate, SeasonLabel = season_label(Season))
  M$Stage = (floor(M$Vegetative) + 2*floor(M$Reproductive) + 3*floor(M$Ripening))/(floor(M$Vegetative) + floor(M$Reproductive) + floor(M$Ripening))
  ggplot(sim) +
    geom_line (aes(x=Date, y=adult, group=iteration), alpha=0.3, size=1) +
    geom_line (aes(x=Date, y=Adult), colour="forestgreen",data=M, size=1) +
    geom_point(aes(x=Date, y=Adult), colour="forestgreen", data=M, size=2) +
    geom_text(aes(x=Date, y=0, label=SeasonLabel), data=M) +
    scale_x_datetime(breaks = date_breaks('years'), labels=date_format('%Y')) +
    scale_y_continuous(trans="sqrt") +
    theme_bw()
    
}
print(show_sim())

show_fit = function() {
  M = join(obs, sim, by=c("Date"))
  names(M)[names(M)=="Adult"] = "ObsAdult"
  names(M)[names(M)=="adult"] = "SimAdult"
  M = M[M$ObsAdult>0 & M$SimAdult>0,]
  M$iteration = factor(M$iteration)
  M$Week = factor(M$Week)
  ggplot(M, aes(x=ObsAdult, y=SimAdult)) +
    geom_point(aes(colour=Week)) +
    scale_x_continuous(trans="log10") +
    scale_y_continuous(trans="log10") +
    geom_abline()
    
}

