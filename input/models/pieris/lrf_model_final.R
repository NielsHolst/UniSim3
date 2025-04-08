rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(reshape2)

source("~/Rdev/setup.R")


f = function(T, p) {
  Tmin = p[1]
  Topt = p[2]
  Tmax = p[3]
  y = (T - Tmax)*(T - Tmin)^2 / 
      (Topt - Tmin) / 
      ( (Topt - Tmin)*(T - Topt) - (Topt - Tmax)*(Topt + Tmin - 2*T) )
  y[ T<=Tmin | T>=Tmax ] = 0 # Use inclusive inequalities to filter out possible 0/0 (NaN)
  y
}

lrf = function(T, p) {
  Tmin = p[1]
  Topt = p[2]
  Tmax = p[3]
  ymax = p[4]
  if (Topt > (Tmin + Tmax)/2) {
    F = f(T, p)
  } else {
    q = p
    q[2] = Tmin + Tmax - Topt  # mirror x-axis
    Trev = Tmin + Tmax - T     # mirror x-axis
    F = f(Trev, q)
  }
  
  ymax*F
}

x = seq(0, 40, length.out=100)
M = data.frame(
  T = x,
  Symmetric   = lrf(x, c(5, 20, 35, 1)),
  LeftSkewed  = lrf(x, c(5, 10, 35, 1)),
  RightSkewed = lrf(x, c(5, 30, 35, 1))
)

ggplot(melt(M, id.var="T"), aes(T, value, colour=variable)) +
  geom_line() +
  labs(y="Development rate") +
  facet_wrap(~variable, scales="free", ncol=1) +
  theme(
    legend.position = "none"
  )
