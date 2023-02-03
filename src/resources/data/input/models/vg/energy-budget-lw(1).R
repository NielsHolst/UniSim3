# Prelude
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

update_temperature = function(rad, dt) {
  dT     = (rad$A + rad$A_ - rad$E - rad$E_)/rad$C*dt
  rad$T  = rad$T + dT
  rad$A  = 0
  rad$A_ = 0
  rad
}

update_driving = function(rad) {
  # Short-wave absorption
  rad$A     = sw_budget$A
  rad$A_    = sw_budget$A_
  # Light long-wave emission
  rad$F[4]  = 10
  rad$F_[4] = 10
  # Heating long-wave emission
  rad$F[6]  = 100
  rad$F_[6] = 100
  rad
}

max_dT = function(rad1, rad2) {
  max(abs(rad1$T - rad2$T), na.rm=TRUE)
}

# Long-wave radiation budget
lw = function() {
  M = read.table(header=TRUE, sep=",", text=
   "Layer,a,r,t,a_,r_,t_,T, C
    Sky,0,0,1,1,0,0,-10,NA
    Glass,0.88,0.12,0,0.88,0.12,0,15,8400
    Screen,0.07,0.93,0,0.57,0.43,0,18,2280
    Light,0,0,1,0,0,1,NA,NA
    Plant,0.79,0.01,0.20,0.79,0.01,0.20,24,NA
    Heating,0,0,1,0,0,1,NA,NA
    Floor,0.6,0.4,0,1,0,0,12,42000"
  )
  M$E  = 0
  M$E_ = 0
  M = update_emissions(M)
  M = update_driving(M)
  M$C[is.na(M$C)] = Inf
  M
}
lw()


R1 = lw()
R2 = distribute_rad_down(R1)
R3 = distribute_rad_up  (R2)
llply(list(R1, R2, R3), round_rad)

R1 = lw()
R2 = distribute_rad_iteratively(R1)
R3 = update_temperature(R2,30)
llply(list(R1, R2, R3), round_rad)
DeltaT2 = (R2$A[2]+R2$A_[2]-R2$E[2]-R2$E_[2])/R2$C[2]*30

# sigma*(T0+24)**4
# sigma*(T0+25)**4

rad = lw()
dt = 180
sub_step = 30

distribute_lw = function(rad, dt, sub_step) {
  steps = ceiling(dt/sub_step)
  sub_dt = dt/steps
  sky_balance   = 0
  plant_balance = 0
  R4 = rad 
  for (i in 1:steps) {
    R1 = distribute_rad_iteratively(R4, FALSE)
    R2 = update_temperature(R1,sub_dt)
    R3 = update_emissions(R2)
    R4 = update_driving(R3)
    print(max_dT(R1, R2))
    sky_balance   = sky_balance   + (R1$A_[1] - R1$E[1])*sub_dt
    plant_balance = plant_balance + (R1$A[5]  + R1$A_[5] - R1$E[5] - R1$E_[5])*sub_dt
  }
  list(R1, R2, sky_balance, plant_balance)
}

R = distribute_lw(lw(), 180, 30)
llply(R[1:2], round_rad)
R[3:4]

R0 = lw()
. = data.frame( 
  influx_watt        = sum(c(sw_budget$A, sw_budget$A_)) + 220 
)
.$influx_joule       = .$influx_watt*180
.$glass_dT           = R[[2]]$T[2] - R0$T[2]
.$screen_dT          = R[[2]]$T[3] - R0$T[3]
.$floor_dT           = R[[2]]$T[7] - R0$T[7]
.$glass_joule        = .$glass_dT *R0$C[2]
.$screen_joule       = .$screen_dT*R0$C[3]
.$floor_joule        = .$floor_dT *R0$C[7]
.$sky_joule          = R[[3]]
.$plant_joule        = R[[4]]
.$outflux_joule      = sum(.[,6:10])
round(t(.),1)

