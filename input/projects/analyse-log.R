library(ggplot2)
library(lubridate)
library(plyr)

source("C:/MyDocuments/RDev/setup.R")

obs = read.table("C:/MyDocuments/QDev/UniSim3/output/unisim_log.txt", sep="\t")
colnames(obs) = c("Step", "DateTime", "i", "TimeStep", "DeltaT")
obs$DateTime = ymd_hms(obs$DateTime)
obs = subset(obs, DateTime >= dmy_hms("1/1/2022T0:0:0") )


# f = function(m) {
  # zero = which(m$i==0)
  # zero = c(zero, nrow(m)+1)
  # zero
  
  # turn = rep(0, nrow(m))
  # turn
  # for (j in 1:(length(zero)-1)) {
    # ix = zero[j]:(zero[j+1]-1)
    # turn[ix] = j
  # }
  # M = m
  # M$Turn = turn
  # M
# }

# obs = ddply(obs, .(Step), f)
# obs$Code = paste(obs$Step, obs$Turn, sep="-")

# M = ddply(obs, .(Code), summarize, MaxDeltaT=max(DeltaT), N=max(i)+1)
# W = M[order(M$MaxDeltaT, decreasing=TRUE),]
# head(W,20)

# P = subset(obs, Code %in% W$Code[1:100])

# ggplot(obs, aes(i, DeltaT, group=Code)) +
  # geom_line(colour=red) +
  # theme(legend.position='none')
  
# ggplot(obs, aes(i, DeltaT, group=Code)) +
  # geom_line(colour=red) +
  
ggplot(subset(obs, DeltaT>3), aes(TimeStep, DeltaT)) +
  geom_point(colour=red)

ggplot(subset(obs, DeltaT>5), aes(i, DeltaT)) +
  geom_point(colour=red)
  