# Prelude
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
  M$E  = 0
  M$E_ = 0
  M$F  = 0
  M$F_ = 0
  M$A  = 0
  M$A_ = 0
  M$C[is.na(M$C)] = Inf
  M = update_lw_emission(M)
  M = update_driving_radiation(M)
  M
}

# Update long-wave emission by Boltzman's Law
update_lw_emission = function(budget) {
  ix = !is.na(budget$T)
  budget$E [ix] = sigma*budget$a_[ix]*(budget$T[ix] + T0)**4
  budget$E_[ix] = sigma*budget$a [ix]*(budget$T[ix] + T0)**4
  budget
}

# Update driving variables
update_driving_radiation = function(budget) {
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
update_temperature_by_radiation = function(budget, dt) {
  dT        = with(budget, (A + A_ - E - E_)/C*dt)
  budget$T  = budget$T + dT
  budget$A  = 
  budget$A_ = 0
  budget
}

# Distribute long-wave radiation over time step ('dt') divided into 'n' smaller time steps
distribute_lw = function(rad, dt, n) {
  max_dT = function(budget1, budget2) {
    max(abs(budget1$T - budget2$T), na.rm=TRUE)
  }
  sub_dt = dt/n
  R4 = rad 
  dT_step = NULL
  for (i in 1:n) {
    R1 = distribute_radiation(R4)
    sumNetA = R1$A + R1$A_ - R1$E - R1$E_
    R1$SumNetA = if (i==1) sumNetA else R1$SumNetA + sumNetA 
    R2 = update_temperature_by_radiation(R1,sub_dt)
    R3 = update_lw_emission(R2)
    R4 = update_driving_radiation(R3)
    dT_step = c(dT_step, max_dT(R1, R2))
  }
  list(R2, dT_step)
}

r1 = function(x) round(x,1) 

#
# Main
#

# Update once
R1 = budget_lw()
R2 = distribute_radiation(R1)
R3 = update_temperature_by_radiation(R2,180/6)
R4 = update_lw_emission(R3)
R5 = update_driving_radiation(R4)

# Check step 2 
paste("Absorbed before  :", r1(sum(R1$A) + sum(R1$A_)))
paste("Longwave emission:", r1(sum(R1$E) + sum(R1$E_)))
paste("Longwave after   :", r1(sum(R1$A) + sum(R1$A_) + sum(R1$E) + sum(R1$E_)), "==" , r1(sum(R2$A) + sum(R2$A_)))

# Show calculation steps
llply(list(R1, R2, R3, R4, R5), round_lay)

# Find number of minor time steps (n)
dt = 180
dT = distribute_lw(R1, dt, 1)[[2]]
# Round temperature step up and choose number of substeps to achieve max dT of 0.5 oC
n = ceiling(dT/0.5)
n

# Update in minor time steps
final = distribute_lw(R1, dt, n)[[1]]
final$SumNetA = final$SumNetA*dt/n
round_lay(final)
round_lay(final[,-ncol(final)])

ix = c(2,3,7)
absorbed237           = (final$T[ix] - R1$T[ix])*final$C[ix]
emitted_light_heating = (sum(R1$E[c(4,6)]) + sum(R1$E_[c(4,6)]))*dt

list(
  Absorbed237   = absorbed237, 
  Totalbalance = sum(absorbed237) + final$SumNetA[1] + final$SumNetA[5] - emitted_light_heating
)


