source("~/Rdev/setup.R")

lrf_juvenile <- function(temp, p){
  ymax = p[1]
  Tmin_eggs = p[2]
  Topt_eggs = p[3]
  Tmax_eggs = p[4]
  devr <- ifelse(temp >= Tmax_eggs | temp <= Tmin_eggs,
                 0,
                 ((Ropt_eggs * ((Tmax_eggs) - (temp)) * ((temp) - (Tmin_eggs))^2)/(((Tmin_eggs) - (Topt_eggs)) * (-(temp) * (Tmin_eggs) + 3 * (temp) * (Topt_eggs) - 2 * (Topt_eggs)^2 + (Tmax_eggs) * (-2 * (temp) + (Tmin_eggs) + (Topt_eggs))))))
  
  return(devr) 
  
} 


lrf_diapause = function(temp, p) {
  ymax = p[1]
  Tmin = p[2]
  Topt = p[3]
  Tmax = p[4]
  y = (ymax * (((2*Topt-Tmin)) - (-temp +Topt*2)) * ((-temp +Topt*2) - ((2*Topt-Tmax))) ^ 2) / ((((2*Topt-Tmax)) - (Topt)) * ( - (-temp +Topt*2) * ((2*Topt-Tmax)) + 3 * (-temp +Topt*2) * (Topt) - 2 * (Topt) ^ 2 + ((2*Topt-Tmin)) * ( - 2 * (-temp +Topt*2) + ((2*Topt-Tmax)) + (Topt))))
  y[ temp<Tmin | temp>Tmax ] = 0
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
  