library(lubridate)
library(plyr)
setwd("~/QDev/UniSim2/input/papers/2021-saccharina")
obs = read.table("env_file_2019_2020.txt", TRUE, "\t")
obs$Date = dmy(obs$Date)

M = ddply(obs, .(Date), summarise,
  T  = signif(mean(T),4),
  E0 = signif(sum (I)/1e6*3600,4),
  N  = signif(mean(N)*1e3,4),
  S  = signif(mean(S),4),
  U  = signif(mean(U),4)
)
head(M)

write.table(M, "env-2019.txt", quote=F, sep="\t", row.names=F)

