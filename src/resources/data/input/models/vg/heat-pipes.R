rm(list=ls(all=TRUE))
library(ggplot2)
library(plyr)
library(reshape2)
source("~/QDev/UniSim3/input/scripts/begin.R")

pipe_density  = 2   # m/m2
pipe_diameter = 30  # mm
area          = 10000 # m2
pipe_length   = pipe_density*area # m
pipe_volume   = pi/4*(pipe_diameter/1000)^2*pipe_length # m3
pipe_volume

transit_time  = function(flow_rate) {
  # flow_rate    : m3/h
  # transit_time : min
  pipe_volume/flow_rate*60
}
transit_time(20)

k     = 1.7e-4
b     = 1.25
d     = 37
Tair  = 20

M = expand.grid(
  FlowRate = c(5, 10),
  T0       = 30:80
)
M = mutate(M,
  TransitTime = transit_time(FlowRate),
  DT_pipe = (k*d*(b-1)*TransitTime + (T0 - Tair)^(1-b))^(1/(1-b)),
  E_pipe  = 4184*DT_pipe*pipe_volume/area/TransitTime*1000
)
M = melt(M, id.vars=c("FlowRate", "TransitTime", "T0"))

ggplot(M, aes(T0, value, colour=factor(FlowRate))) +
  scale_colour_manual(values=c(blue, red)) +
  guides(color = guide_legend(reverse = TRUE)) + 
  ylim(0, NA) +
  labs(x="Pipe inlet temperature (oC)",
       y="Drop in pipe temperature (oC)",
       colour="Flow rate\n(m3/h)") +
  geom_line() +
  facet_wrap(~variable, scales="free")
