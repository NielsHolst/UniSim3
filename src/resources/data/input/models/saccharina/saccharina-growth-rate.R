# Prelude
rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(ggpubr)
library(lubridate)
library(plyr)

# Set folder
setwd("~/QDev/UniSim3/input/models/saccharina/data")

# Read obs
read_obs = function(file_name) {
  o = read.table(file_name, sep="\t", header=TRUE)
  o$DataSet = substr(file_name, 1, nchar(file_name)-4)
  o
}
obs = rbind(
  read_obs("obs-2019.txt"),
  obs = read_obs("obs-2019-soe3-top.txt")
)
obs$Date = dmy(obs$Date)
obs$WtotalSE  = with(obs, sd_Wtotal/sqrt(n_Wtotal)) 
obs$Wtotal_t  = qt(0.025, obs$n_Wtotal, lower.tail=FALSE)
obs      = ddply(obs, .(DataSet), mutate, Day = as.numeric(Date - Date[1]))

slopes = function(o) {
  # Compute slopes
  dx     = as.numeric(diff(o$Date))
  dy     = diff(log(o$Wtotal))
  slope  = signif(dy/dx, 2)
  n      = nrow(o)
  # Collect data frame
  data.frame(
    Date   = o$Date[1:(n-1)]   + diff(o$Date)/2,
    Wtotal = 10^(log10(o$Wtotal[1:(n-1)]) + diff(log10(o$Wtotal))/2),
    Slope  = slope,
    Nconc  = o$Nconc[1:(n-1)] + diff(o$Nconc)/2,
    Cconc  = o$Cconc[1:(n-1)] + diff(o$Cconc)/2
  )
}
Slopes = ddply(obs, .(DataSet), slopes)


ggplot(obs, aes(Date,Wtotal, colour=DataSet)) +
  geom_point() +
  geom_errorbar(aes(ymin=Wtotal - Wtotal_t*WtotalSE, ymax=Wtotal + Wtotal_t*WtotalSE), width=0) + 
  geom_line()+
  geom_label(aes(label=Slope), size=2, data=Slopes) +
  scale_x_date(date_breaks="months", date_labels="%m", name="Month") +
  scale_y_continuous(trans='log10', labels=scales::comma, name="Biomass (g/m)") +
  facet_wrap(~DataSet, ncol=1) +
  theme_minimal() +
  theme(legend.position="none")

windows()
ggplot(subset(Slopes, !is.na(Nconc)), aes(Nconc, Slope, colour=DataSet)) +
  geom_jitter(size=3, aes(shape=factor(Date))) +
  geom_line()+
  theme_minimal()
