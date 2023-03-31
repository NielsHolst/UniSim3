# Prelude
source("2-energy-budget-lw.R")

# Wind-dependent U-value of outer glass surface
Uwind = function(windSpeed) {
  round(2.8 + 1.2*windSpeed^0.8, 1)
}
Uwind(4)

# Energy budget
budget_lw_con = function() {
  # Radiation budget
  layers = budget_lw()
  # Convection/Conduction budget
  layers$U  = 0
  layers$U_ = 0
  layers$H  = 0
  layers$H_ = 0
  # Glass, screen, plant, floor
  ix = c(2,3,5,7)
  layers$U[ix]  = c(Uwind(4), 1.2, 1.2, 1.2)
  layers$U_[ix] = c(Uwind(0), 1.2, 1.2, 0.1)
  
  # Volumes
  volumes = data.frame(
    Volume = c("Outdoors", "Indoors", "Soil"),
    T = c(10, 25, 6),
    C = c(Inf, round(1020*1.19*3.938,0), Inf),
    H = c(0, 0, 0)
  )
  # Total budget
  budget = list(Layers=layers, Volumes=volumes)
  # With driving variables
  update_driving_convection(budget)
}


# Update driving variables
update_driving_convection = function(budget) {
  # Set driving only
  budget$Layers$H  =
  budget$Layers$H_ = 0
  budget$Volumes$H = 0
  # Light convective heat
  budget$Layers$H[4]  = -5
  budget$Layers$H_[4] = -5
  # Heating convective heat
  budget$Layers$H[6]  = -50
  budget$Layers$H_[6] = -50
  # Indoors convective heat
  budget$Volumes$H[2] = -with(budget$Layers, sum(H + H_))
  # Done
  budget
}

# Update convective and conductive heat fluxes
update_convection = function(budget) {
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  # Set temperatures in vol neighbouring each layer
  n = nrow(lay)
  T = data.frame(
    Above   = rep(vol$T[2], n),
    Below   = rep(vol$T[2], n),
    VolAbove = rep(2, n),
    VolBelow = rep(2, n)
  )
  rownames(T) = lay$Layer
  T$Above[1:2] = 
  T$Below[1]   = vol$T[1]
  T$Below[7]   = vol$T[3]

  T$VolAbove[1:2] = 
  T$VolBelow[1]   = 1
  T$VolBelow[7]   = 3
  # Update each layer and neighbouring volumes
  for (i in 1:n) {
    # Compute fluxes from layer
    T_layer = lay$T[i]
    if (is.na(T_layer)) {
      flux_up   = 
      flux_down = 0
    } else {
      flux_up   = lay$U[i] *(T$Above[i] - T_layer)
      flux_down = lay$U_[i]*(T$Below[i] - T_layer)
    }
    # Update layer
    lay$H[i]    = lay$H[i]  + flux_up
    lay$H_[i]   = lay$H_[i] + flux_down
    # Update volumes
    vol$H[T$VolAbove[i]] = vol$H[T$VolAbove[i]] - flux_up
    vol$H[T$VolBelow[i]] = vol$H[T$VolBelow[i]] - flux_down
  }
  # Done
  list(Layers=lay, Volumes=vol)
}

# Update temperature from net convection
update_temperature_by_convection = function(budget, dt) {
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  # Updates layers
  dT     = with(lay, (H + H_)/C*dt)
  lay$T  = lay$T + dT
  lay$H  = 
  lay$H_ = 0
  # Update volumes
  dT    = with(vol, H/C*dt)
  vol$T = vol$T + dT
  vol$H = 0
  # Done
  list(Layers=lay, Volumes=vol)
}

# Distribute convective heat over time step ('dt') divided into 'n' smaller time steps
distribute_lw_convection = function(budget, dt, n) {
  max_dT = function(budget1, budget2) {
    max(abs(c(budget2$Layers$T - budget1$Layers$T, budget2$Volumes$T - budget1$Volumes$T)), na.rm=TRUE)
  }

  sub_dt = dt/n
  B7 = budget 
  dT_step = NULL
  for (i in 1:n) {
    B1         = B7
    B2         = B1
    B2$Layers  = distribute_radiation(B1$Layers)
    B3         = update_convection(B2)

    layersSumNetAH = with(B3$Layers, A+A_+H+H_-E-E_)
    B3$Layers$SumNetAH = if (i==1) layersSumNetAH else B3$Layers$SumNetAH + layersSumNetAH 
    B3$Volumes$SumNetH = if (i==1) B3$Volumes$H   else B3$Volumes$SumNetH + B3$Volumes$H 

    B4         = B3
    B4$Layers  = update_temperature_by_radiation(B3$Layers, sub_dt)
    B5         = update_temperature_by_convection(B4, sub_dt)
    B6         = B5
    B6$Layers  = update_lw_emission(B5$Layers)
    B7         = B6
    B7$Layers  = update_driving_radiation(B6$Layers)
    B7         = update_driving_convection(B7)
    
    dT_step    = c(dT_step, max_dT(B1, B5))
  }
  c(B5, dT_step)
}

#
# Main
#

# Update once
B1         = budget_lw_con()
B2         = B1
B2$Layers  = distribute_radiation(B1$Layers)
B3         = update_convection(B2)
B4         = B3
B4$Layers  = update_temperature_by_radiation(B3$Layers, 180/6)
B5         = update_temperature_by_convection(B4, 180/6)
B6         = B5
B6$Layers  = update_lw_emission(B5$Layers)
B7         = B6
B7$Layers  = update_driving_radiation(B6$Layers)
B7         = update_driving_convection(B7)

llply(list(B1, B2, B3, B4, B5, B6, B7), round_budget)

# Check sum of heat fluxes
layers_H  = sum(B3$Layers$H) + sum(B3$Layers$H_)
volumes_H = sum(B3$Volumes$H)
paste("Layers and volumes H is zero in total", layers_H, "+", volumes_H, "==", round(layers_H+volumes_H, 3))

# Check that one minor step equals B5
distribute_lw_convection(B1, 30, 1)

# Final update
final = distribute_lw_convection(B1,180, 6)
final$Layers$SumNetAH = final$Layers$SumNetAH*dt/n
final$Volumes$SumNetH = final$Volumes$SumNetH*dt/n
final
round_budget(final)


# Check that temperature change matches absorbed energy
B1$Layers$T  + with(final$Layers,  SumNetAH/C)
B1$Volumes$T + with(final$Volumes, SumNetH/C)

list(
  TotalBalanceLayers  = sum(final$Layers$SumNetAH),
  TotalBalanceVolumes = sum(final$Volumes$SumNetH),
  TotalBalance        = sum(final$Layers$SumNetAH) + sum(final$Volumes$SumNetH)
)


