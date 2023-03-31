# Prelude
source("3-energy-budget-convection.R")

# Advection
delta_vent = function(yin0, yout, v, dt) {
  (yout-yin0)*(1 -exp(-v/3600*dt))
}
dE_in = function(Tin0, Tout, v, dt) {
  delta_vent(Tin0, Tout, v, dt)*4780/dt
}

# Energy budget
budget_lw_con_adv = function(dt) {
  # Radiation and convection budget
  budget = budget_lw_con()
  # Advection
  budget$Volumes$V = 0
  # With driving variables
  update_driving_advection(budget, dt)
}

# Update driving variables
update_driving_advection = function(budget, dt) {
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  # Advection
  v = 3 # ventilation rate per hour
  flux = dE_in(vol$T[2], vol$T[1], v, dt)
  vol$V = c(-flux, flux, 0)
  # Done
  list(Layers=lay, Volumes=vol)
}

# Update temperature from advection
update_temperature_by_advection = function(budget, dt) {
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  # Update indoors volume
  dT    = with(vol, V/C*dt)
  vol$T[2] = vol$T[2] + dT[2]
  # Done
  list(Layers=lay, Volumes=vol)
}

# Distribute convective heat over time step ('dt') divided into 'n' smaller time steps
distribute_lw_conv_adv = function(budget, dt, n, update_driving_advection=NULL) {
  max_dT = function(budget1, budget2) {
    max(abs(c(budget2$Layers$T - budget1$Layers$T, budget2$Volumes$T - budget1$Volumes$T)), na.rm=TRUE)
  }

  sub_dt = dt/n
  B8 = budget 
  dT_step = NULL
  for (i in 1:n) {
    B1         = B8
    B2         = B1
    B2$Layers  = distribute_radiation(B1$Layers)
    B3         = update_convection(B2)

    layersSumNetAH  = with(B3$Layers, A+A_+H+H_-E-E_)
    volumesSumNetHV = with(B3$Volumes, H+V)
    B3$Layers$SumNetAH  = if (i==1) layersSumNetAH  else B3$Layers$SumNetAH + layersSumNetAH 
    B3$Volumes$SumNetHV = if (i==1) volumesSumNetHV else B3$Volumes$SumNetH + volumesSumNetHV

    B4         = B3
    B4$Layers  = update_temperature_by_radiation(B3$Layers, sub_dt)
    B5         = update_temperature_by_convection(B4, sub_dt)
    B6         = update_temperature_by_advection(B5, sub_dt)
    B7         = B6
    B7$Layers  = update_lw_emission(B6$Layers)
    B8         = B7
    B8$Layers  = update_driving_radiation(B7$Layers)
    B8         = update_driving_convection(B8)
    B8         = update_driving_advection(B8, sub_dt)
    
    dT_step    = c(dT_step, max_dT(B1, B6))

  }
  c(B6, dT_step)
}


#
# Main
#

# Update once
sub_dt     = 30
B1         = budget_lw_con_adv(sub_dt)
B2         = B1
B2$Layers  = distribute_radiation(B1$Layers)
B3         = update_convection(B2)
B4         = B3
B4$Layers  = update_temperature_by_radiation(B3$Layers, sub_dt)
B5         = update_temperature_by_convection(B4, sub_dt)
B6         = update_temperature_by_advection(B5, sub_dt)
B7         = B6
B7$Layers  = update_lw_emission(B6$Layers)
B8         = B7
B8$Layers  = update_driving_radiation(B7$Layers)
B8         = update_driving_convection(B8)
B8         = update_driving_advection(B8, sub_dt)

data.frame(Tnew = B1$Layers$T + with(B3$Layers, (A+A_+H+H_-E-E_)*dt/n/C))

llply(list(B1, B2, B3, B4, B5, B6, B7, B8), round_budget)

# Final update
final = distribute_lw_conv_adv(B1,180, 6)
final$Layers$SumNetAH  = final$Layers$SumNetAH*dt/n
final$Volumes$SumNetHV = final$Volumes$SumNetHV*dt/n
final
round_budget(final)


# Check that temperature change matches absorbed energy
B1$Layers$T  + with(final$Layers,  SumNetAH/C)
B1$Volumes$T + with(final$Volumes, SumNetHV/C)

# Check whole budget
list(
  TotalBalanceLayers  = sum(final$Layers$SumNetAH),
  TotalBalanceVolumes = sum(final$Volumes$SumNetHV),
  TotalBalance        = sum(final$Layers$SumNetAH) + sum(final$Volumes$SumNetHV)
)


