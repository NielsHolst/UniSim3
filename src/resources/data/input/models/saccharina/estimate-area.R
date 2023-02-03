rm(list=ls(all=TRUE))
library(clipr)
library(ggplot2)
library(ggpubr)
library(reshape2)
library(stringr)
source("~/QDev/UniSim3/input/scripts/begin.R")

# solve[U==(Sb + Cd)/W && V==(Sm + Nn)/W && W==S+C+N, {S,C,N}]
# solution:
# k = b n + d (m - n)
# S = (W (d (V - n) + n U))/k 
# C = (W (b (n - V) + U (m - n)))/k 
# N = (W (b V + d (m - V) - m U))/k



b = cSC = 0.21
d = cCC = 0.47
m = cSN = 0.02
n = cNN = 0.37

setwd("~/QDev/UniSim3/input/models/saccharina")
obs = read.table("data/obs-2019.txt", TRUE, "\t")[-(1:2), c(1,2,5,6, 7,8)]
obs
W = obs$Wtotal
U = obs$Cconc
V = obs$Nconc

k = b*n + d*(m - n)
obs$S = (W*(d*(V - n) + n*U))/k 

ggarrange(
  ggplot(obs, aes(S, A)) +
    geom_point(size=3)
  ,
  ggplot(obs, aes(log(S), log(A))) +
    geom_point(size=3) +
    geom_smooth(method="lm")
  ,
  ggplot(obs, aes(S, L)) +
    geom_point(size=3)
  ,
  ggplot(obs, aes(log(S), log(L))) +
    geom_point(size=3) +
    geom_smooth(method="lm")
)

model = lm(log(A) ~ log(S), obs)
summary(model)
iceptA = exp(coef(model)[1])
slopeA = coef(model)[2]

model = lm(log(L) ~ log(S), obs)
summary(model)
iceptL = exp(coef(model)[1])
slopeL = coef(model)[2]

x = 4*(0:100)
M = data.frame(
  S = x,
  Al = iceptA*x^slopeA,
  L = iceptL*x^slopeL
)
M$Az  = with(M, 2*L)
M$Ai = with(M, Al/Az)
M$fabs = with(M, 1 - exp(-0.7*Ai))

M2 = melt(M, id.vars=1)
ggplot(M2, aes(S, value, colour=variable)) +
  geom_line(size=1) +
  facet_wrap(~variable, scales="free", nrow=1) +
  labs(x="Structural weight (g dw/m)", y="") +
  theme_classic() +
  theme(legend.position="none")
  
  
list(iceptA, slopeA, iceptL, slopeL)

s0 = c("k = b n + d (m - n)", " S = ( W (d (V - n) + n U))/k")
s = str_replace_all(s0, "b", "1*cSC")
s = str_replace_all(s , "d", "1*cCC")
s = str_replace_all(s , "m", "1*cSN")
s = str_replace_all(s , "n", "1*cNN")
s = str_replace_all(s , " S", "1*wSdw")
s = str_replace_all(s , " W", "(1*wWdw")
s = str_replace_all(s , "U", "1*cWC")
s = str_replace_all(s , "V", "1*cWN")
write_clip(paste(s))
