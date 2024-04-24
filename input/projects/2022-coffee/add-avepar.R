rm(list=ls(all=TRUE))
setwd("~/QDev/UniSim2/input/papers/2022-coffee")

### Read weather
weather = read.table("Las_Colinas_orig.txt", sep="\t", header=TRUE)
weather$AvgPar30 = rollmean(0.5*weather$GlobRad, 30, na.pad=TRUE, align="right")

M = weather
M$Month = month(weather$Date)
ddply(M, .(Month), summarize, AveParM = mean(AvgPar30, na.rm=TRUE))

summary(weather)
weather$AvgPar30[is.na(weather$AvgPar30)] = 8
summary(weather)

write.table(weather, "Las_Colinas.txt", sep="\t", row.names=FALSE, quote=FALSE)



