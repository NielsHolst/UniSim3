source("~/Rdev/setup.R")
library(reshape2)

## LRF TPC function
LRF <- function(temp, Tmin, Tmax, Topt, Ropt) {
  devr <- ifelse(temp>=Tmax|temp<=Tmin,
                 0,
                 (Ropt * ((Tmax) - (temp)) * ((temp) - (Tmin))^2)/(((Tmin) - (Topt)) * (-(temp) * (Tmin) + 3 * (temp) * (Topt) - 2 * (Topt)^2 + (Tmax) * (-2 * (temp) + (Tmin) + (Topt)))))
  return(devr) 
}

LRF <- function(temp, Tmin, Tmax, Topt, Ropt) {
  devr <- ifelse(temp>=Tmax|temp<=Tmin,
                 0,
(Ropt * (((2*Topt-Tmin)) - (-temp +Topt*2)) * ((-temp +Topt*2) - ((2*Topt-Tmax))) ^ 2) / ((((2*Topt-Tmax)) - (Topt)) * ( - (-temp +Topt*2) * ((2*Topt-Tmax)) + 3 * (-temp +Topt*2) * (Topt) - 2 * (Topt) ^ 2 + ((2*Topt-Tmin)) * ( - 2 * (-temp +Topt*2) + ((2*Topt-Tmax)) + (Topt)))))
  return(devr) 

}


x = seq(-6, 40, length.out=100)
M = data.frame(
  T = x,
  Egg = LRF(x, 1.9, 36.0, 30.5, 0.354),
  Diapause = LRF(x, -4.1,  30.9, 1.6, 0.0634)
)

ggplot(melt(M, id.var="T"), aes(T, value, colour=variable)) +
  geom_line() +
  facet_wrap(~variable, scales="free", ncol=1)
  
