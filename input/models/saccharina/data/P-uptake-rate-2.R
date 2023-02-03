library(ggplot2)
library(plyr)
library(lubridate)
library(reshape2)

setwd("~/QDev/UniSim3/input/models/saccharina/data")
obs = read.table("obs-2019.txt", sep="\t", header=TRUE)
env = read.table("env-2019.txt", sep="\t", header=TRUE)
obs$Date = dmy(obs$Date)
env$Date = dmy(env$Date)
M = join(obs, env, by="Date")

ggplot(M, aes(P, Pconc)) +
  geom_path() +
  geom_label(aes(label=Date), fill="grey") +
  xlim(0,1.4) +
  theme_minimal()
  