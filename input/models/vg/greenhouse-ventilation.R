rm(list=ls(all=TRUE))
library(ggplot2)
library(plyr)
library(reshape2)
source("~/QDev/UniSim3/input/scripts/begin.R")

v = 1
Dt = 30
T_out = 10
T_in  = 20
h_gh = 3.938
C_air = 1124

M = expand.grid(
  v     = c(1,4),
  T_out = -5:25
)

M = mutate(M,
  v_r   = 1 - exp(-v*Dt/3600),
  P_adv = ((T_out - T_in)*v_r*h_gh*C_air)/Dt,
  v_label = paste("v =", v)
)

ggplot(M, aes(T_out, P_adv, colour=v_label)) +
  geom_hline(yintercept=0,  linetype="dashed") +
  geom_vline(xintercept=T_in, linetype="dashed") +
  geom_line() +
  labs(x="Outdoors temperature (oC)", 
       y="Advective heat gain (W/m2)", 
       colour="Ventilation\nrate") 
