# Prelude
setwd("~/QDev/UniSim3/input/models/vg")
source("0-energy-budget-functions.R")

# Retrieve short-wave energy budget
source("1-energy-budget-sw.R")

# Constants
sigma = 5.6704e-8
T0    = 273.15

# Long-wave radiation budget
budget_lw = function() {
  M = read.table(header=TRUE, sep=",", text=
   "Layer,a,r,t,a_,r_,t_,T, C
    Sky,0,0,1,1,0,0,-10,NA
    Glass,0.88,0.12,0,0.88,0.12,0,15,8400
    Screen,0.07,0.93,0,0.57,0.43,0,18,2280
    Light,0,0,1,0,0,1,NA,NA
    Plant,0.79,0.01,0.20,0.79,0.01,0.20,24,NA
    Heating,0,0,1,0,0,1,NA,NA
    Floor,0.6,0.4,0,0,0,1,12,42000"
  )
  M$C[is.na(M$C)] = Inf
  M = update_lw_emission(M)
  M = update_driving(M)
  M
}

# Update long-wave emission by Boltzman's Law
update_lw_emission = function(budget) {
  budget$E  = 0
  budget$E_ = 0
  ix = !is.na(budget$T)
  budget$E [ix] = sigma*budget$a_[ix]*(budget$T[ix] + T0)**4
  budget$E_[ix] = sigma*budget$a [ix]*(budget$T[ix] + T0)**4
  budget
}

# Update driving variables
update_driving = function(budget) {
  # Short-wave absorption (computed earlier)
  budget$A     = final_sw$A
  budget$A_    = final_sw$A_
  # Light long-wave emission
  budget$E[4]  = 15
  budget$E_[4] = 15
  # Heating long-wave emission
  budget$E[6]  = 75
  budget$E_[6] = 75
  budget
}

# Update temperature from net radiation absorption
update_temperature = function(budget, dt) {
  dT        = with(budget, (A + A_ - E - E_)/C*dt)
  budget$T  = budget$T + dT
  budget$A  = 
  budget$A_ = 0
  budget
}

# Distribute long-wave radiation over a total time step ('dt') 
# divided into 'num_sub_steps minor' time steps
distribute_lw = function(rad, dt, num_sub_steps) {
  max_dT = function(budget1, budget2) {
    max(abs(budget1$T - budget2$T), na.rm=TRUE)
  }
  sub_dt = dt/num_sub_steps
  rad$SE = 0
  rad$SA = 0
  R4 = rad 
  dT_step = NULL
  for (i in 1:num_sub_steps) {
    R1 = distribute_rad_iteratively(R4, FALSE)
    R1$SE = R1$SE + (R1$E + R1$E_)*sub_dt
    R1$SA = R1$SA + (R1$A + R1$A_)*sub_dt
    R2 = update_temperature(R1,sub_dt)
    R3 = update_lw_emission(R2)
    R4 = update_driving(R3)
    dT_step = c(dT_step, max_dT(R1, R2))
  }
  list(R2[,c(1,8,14,15)], dT_step)
}

#
# Main
#

# Update once down and up
R0 = budget_lw()
R1 = R0
R1$F  = R1$E
R1$F_ = R1$E_
R2 = distribute_rad_down(R1)
R3 = distribute_rad_up  (R2)
# Update iteratively
R6 = distribute_rad_iteratively(R1)
# Show calculation steps
round_budget(list(R0, R1, R2, R3, R6))

# Check temperature steps not too large
dt = 180
dT = distribute_lw(R0, dt, 1)[[2]]
# Round temperature step up and choose number of substeps to achieve max dT of 0.5 oC
num_sub_steps = ceiling(2*dT)
num_sub_steps

# Update in minor time steps
final      = distribute_lw(R0, dt, num_sub_steps)[[1]]
final$NetJ = with(final, SA-SE) 
final$dT   = final$NetJ/R0$C
final$T0   = R0$T
final$Tchk = with(final[[1]], T - dT)
round_rad(final)
sum(final$NetJ)/dt

