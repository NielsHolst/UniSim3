rm(list=ls(all=TRUE))
graphics.off()
source("~/Rdev/setup.R")

lrf_juvenile <- function(T, p){
  ymax = p[1]
  Tmin = p[2]
  Topt = p[3]
  Tmax = p[4]
  y = ymax*(Tmax - (T))*((T) - Tmin)^2 / (Tmin - Topt) / (-T*Tmin + 3*T*Topt - 2*Topt^2 + Tmax*(-2*T + Tmin + Topt))
  y[ T<Tmin | T>Tmax ] = 0
  y
}


lrf_diapause = function(T, p) {
  ymax = p[1]
  Tmin = p[2]
  Topt = p[3]
  Tmax = p[4]
  y = (ymax * ((2*Topt-Tmin) - (2*Topt - T)) * ((-T +Topt*2) - ((2*Topt-Tmax))) ^ 2) / ((((2*Topt-Tmax)) - (Topt)) * ( - (-T +Topt*2) * ((2*Topt-Tmax)) + 3 * (-T +Topt*2) * (Topt) - 2 * (Topt) ^ 2 + ((2*Topt-Tmin)) * ( - 2 * (-T +Topt*2) + ((2*Topt-Tmax)) + (Topt))))
  y[ T<Tmin | T>Tmax ] = 0
  y
}

egg      = c(0.354,   1.9, 30.5, 36.0)
diapause = c(0.0634, -4.1,  1.6, 30.9)

x = -6:40
M = data.frame(
  T = x,
  Egg = lrf_juvenile(x, egg),
  Diapause = lrf_diapause(x, diapause)
)

ggplot(melt(M, id.var="T"), aes(T, value, colour=variable)) +
  geom_line()
  