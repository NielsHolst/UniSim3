# Prelude
rm(list=ls(all=TRUE))
setwd("~/QDev/UniSim3/input/models/vg")
source("energy-budget-functions.R")

# Retrieve short-wave energy budget
source("energy-budget-sw.R")
sw_budget = R1

# Constants
sigma = 5.6704e-8
T0    = 273.15

update_emissions = function(rad) {
  ix = !is.na(rad$T)
  rad$E [ix] = sigma*rad$a_[ix]*(rad$T[ix] + T0)**4
  rad$E_[ix] = sigma*rad$a [ix]*(rad$T[ix] + T0)**4
  rad$F      = rad$E
  rad$F_     = rad$E_
  rad
}

update_temperature = function(rad, dt=120) {
  dT     = (rad$A + rad$A_ - rad$E - rad$E_)/rad$C*dt
  rad$T  = rad$T + dT
  rad$A  = 0
  rad$A_ = 0
  rad
}

# Long-wave radiation budget
lw = function() {
  M = read.table(header=TRUE, sep=",", text=
   "Layer,a,r,t,a_,r_,t_,T, C
    Sky,1,0,0,1,0,0,-10,NA
    Glass,0.88,0.12,0,0.88,0.12,0,15,8400
    Screen,0.07,0.93,0,0.57,0.43,0,18,2280
    Light,0,0,1,0,0,1,NA,NA
    Plant,0.79,0.01,0.20,0.79,0.01,0.20,24,NA
    Heating,0,0,1,0,0,1,NA,NA
    Floor,0.6,0.4,0,1,0,0,12,42000
    Soil,0,1,0,0,1,0,8,NA"
  )
  M$E  = 0
  M$E_ = 0
  M$A  = sw_budget$A
  M$A_ = sw_budget$A_
  M = update_emissions(M)
  M$F[4]  = 10
  M$F_[4] = 10
  M$F[6]  = 100
  M$F_[6] = 100
  M$C[is.na(M$C)] = Inf
  M
}
lw()


R0 = lw()
R1 = distribute_rad_down(R0)
R2 = distribute_rad_up  (R1)
llply(list(R0, R1, R2), round_rad)

R0 = lw()
R1 = distribute_rad_iteratively(R0)
R2 = update_temperature(R1)
R3 = update_emissions(R2)
R4 = distribute_rad_iteratively(R3)
llply(list(R0, R1, R2, R3, R4), round_rad)

R0 = R3 = lw()
iter = 0
precision = 0.5
deviation = Inf
while (deviation > precision) {
  iter = iter + 1
  R1 = distribute_rad_iteratively(R3, FALSE)
  R2 = update_temperature(R1)
  R3 = update_emissions(R2)
  deviation = max(abs(R1$T - R2$T), na.rm=TRUE)
}
print(paste(iter, "iterations needed"))
llply(list(R0, R1, R2, R3), round_rad)

solution    = R3
solution$A  = R1$A
solution$A_ = R1$A_
solution$F  = 0
solution$F_ = 0
solution$Balance = with(solution, A + A_ - E -E_)
sum(solution$Balance)
round_rad(solution)

