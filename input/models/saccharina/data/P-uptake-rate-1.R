library(ggplot2)
library(plyr)
library(lubridate)
library(reshape2)

setwd("~/QDev/UniSim3/input/models/saccharina/data")
obs = read.table("obs-2019.txt", sep="\t", header=TRUE)
env = read.table("env-2019.txt", sep="\t", header=TRUE)
obs$Date = dmy(obs$Date)
env$Date = dmy(env$Date)

obs = mutate(obs,
  CPratio = Cconc/Pconc,
  NPratio = Nconc/Pconc
)
M = melt(obs, id.vars="Date", measure.vars= c("CPratio", "NPratio"))
ggplot(M, aes(Date, value, colour=variable)) +
  geom_point(size=3) +
  geom_line(size=1)
  

M = join(obs[c("Date", "Wtotal", "Pconc")], env[c("Date", "Wtotal", "Pconc")], by="Date")
M$Date = dmy(M$Date)

M = mutate(M,
  Pdw     = Wtotal*Pconc,
  DPdw    = c(NA, diff(Pdw)),
  Dt      = c(NA, diff(Date)),
  RelDPdw = DPdw/Wtotal/Dt
)
M

W = melt(M, id.vars=c("Date"), measure.vars=c("P", "DPdw"))

ggarrange(
  ggplot(W, aes(Date, value, colour=variable)) +
    geom_point(size=3) +
    geom_line(size=1)
  ,
  ggplot(M, aes(P, RelDPdw)) +
    geom_label(aes(label=Date)) +
    geom_point(size=3, alpha=0.5) +
    xlim(0,NA) +
    ylim(0,NA)
  ,
  ncol=1
)

model = lm(RelDPdw ~ P-1, data=subset(M, P<0.9))
summary(model)
round(coef(model), 6)

