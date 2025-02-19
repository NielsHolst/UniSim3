rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(reshape2)

source("~/Rdev/setup.R")

lrf_juvenile_v1 <- function(T, p){
  ymax = p[1]
  Tmin = p[2]
  Topt = p[3]
  Tmax = p[4]
  y = ymax*(T - Tmax)*(T - Tmin)^2 / 
      (Topt - Tmin) / 
      ( (Topt - Tmin)*(T - Topt) - (Topt - Tmax)*(Topt + Tmin - 2*T) )
  y[ T<Tmin | T>Tmax ] = 0
  y
}

lrf_juvenile_v2 <- function(T, p){
  ymax = p[1]
  Tmin = p[2]
  Topt = p[3]
  Tmax = p[4]
  y = ymax*(Tmax - T)*(T - Tmin)^2 / 
      (Tmin - Topt) / 
      (-T*Tmin + 3*T*Topt - 2*Topt^2 + Tmax*(Topt + Tmin - 2*T))
  y[ T<Tmin | T>Tmax ] = 0
  y
}

lrf_diapause = function(T, p) {
  ymax = p[1]
  Tmin = p[2]
  Topt = p[3]
  Tmax = p[4]
  y = (ymax*(T - Tmin)*(Tmax - T)^2) / 
      (Topt - Tmax) / 
      (-(2*Topt - T)*(2*Topt - Tmax) + 3*(2*Topt - T)*Topt - 2*Topt^2 + (2*Topt - Tmin)*(-2*(2*Topt - T) + 2*Topt-Tmax + Topt))
  y[ T<Tmin | T>Tmax ] = 0
  y
}

egg      = c(0.354,   1.9, 30.5, 36.0)
diapause = c(0.0634, -4.1,  1.6, 30.9)

x = seq(-6, 40, length.out=100)
M = data.frame(
  T = x,
  Egg_v1 = lrf_juvenile_v1(x, egg),
  Egg_v2 = lrf_juvenile_v2(x, egg),
  Diapause = lrf_diapause(x, diapause)
)

ggplot(melt(M, id.var="T"), aes(T, value, colour=variable)) +
  geom_line() +
  labs(y = "Development rate") +
  facet_wrap(~variable, scales="free", ncol=1)
  
  
x = seq(0, 40, length.out=100)
M = data.frame(
  T = x,
  Symmetric_juv = lrf_juvenile_v1(x, c(1, 5, 20, 35)),
  Symmetric_dia = lrf_diapause(x, c(1, 5, 20, 35)),
  LeftSkewed_juv = lrf_juvenile_v1(x, c(1, 5, 10, 35)),
  LeftSkewed_dia = lrf_diapause(x, c(1, 5, 10, 35)),
  RightSkewed_juv = lrf_juvenile_v1(x, c(1, 5, 30, 35)),
  RightSkewed_dia = lrf_diapause(x, c(1, 5, 30, 35))
)

ggplot(melt(M, id.var="T"), aes(T, value, colour=variable)) +
  geom_line() +
  facet_wrap(~variable, scales="free", ncol=2) +
  theme(
    legend.position = "none"
  )
  

