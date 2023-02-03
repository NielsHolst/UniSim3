  # Prelude
setwd("~/QDev/UniSim3/input/models/vg")
source("0-energy-budget-functions.R")

# Short-wave radiation budget
budget_sw = function() {
  M = read.table(header=TRUE, sep=",", text=
   "Layer,a,r,t,a_,r_,t_,E,E_
    Sky,1,0,0,1,0,0,0, 0
    Glass,0.03,0.08,0.89,0.03,0.08,0.89,0,0
    Screen,0.1,0.6,0.3,0.1,0.6,0.3,0,0
    Light,0,0,1,0,0,1,0,0
    Plant,0.71,0.05,0.24,0.71,0.05,0.24,0,0  
    Heating,0,0,1,0,0,1,0,0
    Floor,0.6,0.4,0,1,0,0,0,0"
  )
  update_driving(M)
}

# Update driving variables
update_driving = function(budget) {
  budget$A  = 0
  budget$A_ = 0
  # Downwards short-wave radiation from growth light
  budget$E[4]  = 100
  budget
}

#
# Main
#

# Update once down and up
R0 = budget_sw()
R0$F  = R0$E
R0$F_ = R0$E_
R1 = distribute_rad_down(R0)
R2 = distribute_rad_up  (R1)
llply(list(R0, R1, R2), round_rad)

# Update until solved
final_sw = distribute_rad_iteratively(R0)

# Summarise
final_sw$SE = with(final_sw, E + E_)
final_sw$SA = with(final_sw, A + A_)
round_rad(final_sw[,c(1,12,13)])
sum(final_sw$SA)
