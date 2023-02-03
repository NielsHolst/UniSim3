# Prelude
setwd("~/QDev/UniSim3/input/models/vg")
source("0-energy-budget-functions.R")

# Retrieve short-wave energy budget
source("1-energy-budget-sw.R")

# Constants
sigma = 5.6704e-8
T0    = 273.15

# Wind-dependent U-value of outer glass surface
Uwind = function(windSpeed) {
  round(2.8 + 1.2*windSpeed^0.8, 1)
}
Uwind(4)

# Energy budget
budget_lw_heat = function() {
  # Layers
  layers = read.table(header=TRUE, sep=",", text=
   "Layer,a,r,t,a_,r_,t_,T, C, U, U_
    Sky,0,0,1,1,0,0,-10,NA,0,0
    Glass,0.88,0.12,0,0.88,0.12,0,15,8400,6.4,3.3
    Screen,0.07,0.93,0,0.57,0.43,0,18,2280,1.2,1.2
    Light,0,0,1,0,0,1,NA,NA,0,0
    Plant,0.79,0.01,0.20,0.79,0.01,0.20,24,NA,1.2,1.2
    Heating,0,0,1,0,0,1,NA,NA,0,0
    Floor,0.6,0.4,0,0,0,1,12,42000,1.2,0.1"
  )
  layers$C[is.na(layers$C)] = Inf
  layers = update_lw_emission(layers)
  # Volumes
  volumes = data.frame(
    Volume = c("Outdoors", "Indoors", "Soil"),
    T = c(10, 25, 6),
    C = c(Inf, 5341, Inf),
    H = c(0, 0, 0)
  )
  # Driving variables
  update_driving(list(layers, volumes))
}

# Update long-wave emission by Boltzman's Law
update_lw_emission = function(layers) {
  layers$E  = 0
  layers$E_ = 0
  ix = !is.na(layers$T)
  layers$E [ix] = sigma*layers$a_[ix]*(layers$T[ix] + T0)**4
  layers$E_[ix] = sigma*layers$a [ix]*(layers$T[ix] + T0)**4
  layers
}

# Update driving variables
update_driving = function(budget) {
  # Prelude
  layers     = budget[[1]]
  volumes    = budget[[2]]
  layers$H_  = 
  layers$H   = 0
  # Short-wave absorption (computed earlier)
  layers$A     = final_sw$A
  layers$A_    = final_sw$A_
  # Light long-wave emission
  layers$E[4]  = 15
  layers$E_[4] = 15
  # Heating long-wave emission
  layers$E[6]  = 75
  layers$E_[6] = 75
  # Light convective heat
  layers$H[4]  = -5
  layers$H_[4] = -5
  # Heating convective heat
  layers$H[6]  = -50
  layers$H_[6] = -50
  # Convective heat
  volumes$H[2] = -sum(layers$H) - sum(layers$H_)
  # Done
  list(layers, volumes)
}
# budget_lw_heat()

# Update convective and conductive heat fluxes
update_heat_fluxes = function(budget) {
  # Prelude
  layers  = budget[[1]]
  volumes = budget[[2]]
  # Set temperatures in volumes neighbouring each layer
  n = nrow(layers)
  T = data.frame(
    Above   = rep(volumes$T[2], n),
    Below   = rep(volumes$T[2], n),
    IxAbove = rep(2, n),
    IxBelow = rep(2, n)
  )
  T$Above[1:2] = 
  T$Below[1]   = volumes$T[1]
  T$Below[7]   = volumes$T[3]

  T$IxAbove[1:2] = 
  T$IxBelow[1]   = 1
  T$IxBelow[7]   = 3
  # Update each layer and neighbouring volumes
  for (i in 1:n) {
    T_layer = layers$T[i]
    if (is.na(T_layer)) {
      flux_up   = 
      flux_down = 0
    } else {
      flux_up   = layers$U[i] *(T$Above[i] - T_layer)
      flux_down = layers$U_[i]*(T$Below[i] - T_layer)
    }
    layers$H[i]             = layers$H[i]  + flux_up
    layers$H_[i]            = layers$H_[i] + flux_down
    #
    volumes$H[T$IxAbove[i]] = volumes$H[T$IxAbove[i]] - flux_up
    volumes$H[T$IxBelow[i]] = volumes$H[T$IxBelow[i]] - flux_down
  }
  # Done
  list(layers, volumes)
}

# Update temperature from net radiation absorption
update_temperature = function(budget, dt) {
  # Prelude
  layers  = budget[[1]]
  volumes = budget[[2]]
  # Updates layers
  dT        = with(layers, (A + A_ - E - E_ + H + H_)/C*dt)
  layers$T  = layers$T + dT
  layers$A  = 
  layers$A_ = 
  layers$H  = 
  layers$H_ = 0
  # Update volumes
  dT        = with(volumes, H/C*dt)
  volumes$T = volumes$T + dT
  volumes$H = 0
  # Done
  list(layers, volumes)
}
# update_temperature(budget_lw_heat(), 30)

budget = budget_lw_heat()
dt = 20
num_sub_steps = 1

# Distribute long-wave radiation + convective and conductive heat
# over a total time step ('dt') 
# divided into 'num_sub_steps minor' time steps
distribute_lw_heat = function(budget, dt, num_sub_steps) {

  max_dT = function(budget1, budget2) {
    max(
      c(
        abs(budget1[[1]]$T - budget2[[1]]$T), 
        abs(budget1[[2]]$T - budget2[[2]]$T)
      ),
      na.rm=TRUE
    )
  }

  sub_dt = dt/num_sub_steps
  budget[[1]]$SE = 0
  budget[[1]]$SA = 0
  budget[[1]]$SH = 0
  budget[[2]]$SH = 0
  B5 = budget
  dT_step = NULL
  for (i in 1:num_sub_steps) {
    B1      = B5
    B1[[1]] = distribute_rad_iteratively(B5[[1]], FALSE)
    B2 = update_heat_fluxes(B1)
    
    B2[[1]]$SE = with(B2[[1]], SE + (E + E_)*sub_dt)
    B2[[1]]$SA = with(B2[[1]], SA + (A + A_)*sub_dt)
    B2[[1]]$SH = with(B2[[1]], SH + (H + H_)*sub_dt)
    B2[[2]]$SH = with(B2[[2]], SH + (H     )*sub_dt)
    
    B3 = update_temperature(B2,sub_dt)
    B4 = list( update_lw_emission(B3[[1]]), B3[[2]])
    B5 = update_driving(B4)
    dT_step = c(dT_step, max_dT(B1, B3))
      }
  list(B3, dT_step)
}

#
# Main
#

B0 = budget_lw_heat()

# Check temperature steps not too large
dt = 180
dT = distribute_lw_heat(B0, dt, 1)[[2]]
# Round temperature step up and choose number of substeps to achieve max dT of 0.5 oC
num_sub_steps = ceiling(2*dT)
num_sub_steps

# Update in minor time steps
final = distribute_lw_heat(B0, dt, num_sub_steps)[[1]]
final = list(
  final[[1]][,c(1,8,18:20)],
  final[[2]][,c(1,2,4,5)]
)

final[[1]]$NetJ = with(final[[1]], SA - SE + SH) 
final[[2]]$NetJ = with(final[[2]], SH) 
final[[1]]$dT   = with(final[[1]], NetJ/B0[[1]]$C)
final[[2]]$dT   = with(final[[2]], NetJ/B0[[2]]$C)
final[[1]]$T0   = B0[[1]]$T
final[[2]]$T0   = B0[[2]]$T
final[[1]]$Tchk = with(final[[1]], T - dT)
final[[2]]$Tchk = with(final[[2]], T - dT)
final

(sum(final[[1]]$NetJ) + sum(final[[2]]$NetJ))/dt


