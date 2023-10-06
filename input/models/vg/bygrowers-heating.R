source("~/Rdev/setup.R")
setwd("/MyDocuments/QDev/UniSim3/input/models/vg")
obs = read.table("bygrowers-heating.txt", header=TRUE, stringsAsFactors=FALSE, sep="\t")
obs$DateTime = parse_date_time(obs$TimeStamp, "%d%m%Y:%H:%M")
obs$Heating = 0.23*obs$EnergyMWh/sum(obs$EnergyMWh)*1e6
obs = obs[,3:4]
summary(obs)

ggplot(subset(obs, DateTime>=dmy("1/7/2022") & DateTime<=dmy("15/7/2022")), aes(DateTime, Heating)) +
  geom_line(colour=red)
  
