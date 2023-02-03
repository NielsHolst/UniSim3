# Prelude
setwd("~/QDev/UniSim3/input/models/vg")
source("0-energy-budget-functions.R")

# Retrieve short-wave energy budget
source("1-energy-budget-sw.R")
sw_budget = R1

# Constants
sigma = 5.6704e-8
T0    = 273.15

# Parameters
ventilation_rate = 2

update_driving = function(budget) {
  # Distinguish budgets
  layers  = budget[[1]]
  volumes = budget[[2]]
  # Short-wave absorption
  layers$A     = sw_budget$A
  layers$A_    = sw_budget$A_
  # Done
  list(layers, volumes)
}

update_lw_emission = function(budget) {
  # Update only layers
  layers  = budget[[1]]
  # Update only layers with temperature
  ix = !is.na(layers$T)
  layers$E [ix] = sigma*layers$a_[ix]*(layers$T[ix] + T0)**4
  layers$E_[ix] = sigma*layers$a [ix]*(layers$T[ix] + T0)**4
  # Assign flows to emissions
  layers$F      = layers$E
  layers$F_     = layers$E_
  # Done
  list(layers, budget[[2]])
}

update_heat_fluxes = function(budget) {
  layers  = budget[[1]]
  volumes = budget[[2]]
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
    volumes$H[T$IxAbove[i]] = volumes$H[T$IxAbove[i]] - flux_up
    volumes$H[T$IxBelow[i]] = volumes$H[T$IxBelow[i]] - flux_down
  }
  list(layers, volumes)
}

# Energy budget

Uwind = function(windSpeed) {
  round(2.8 + 1.2*windSpeed^0.8, 1)
}
Uwind(4)

create_budget = function() {
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

  layers$H_  = layers$H = 
  layers$A_  = layers$A = 
  layers$F_  = layers$F = 
  layers$E_  = layers$E = 0
  
  # Volumes
  volumes = data.frame(
    Volume = c("Outdoors", "Indoors", "Soil"),
    T = c(10, 25, 6),
    C = c(Inf, 5341, Inf),
    H = c(0, 0, 0),
    A = c(0, 0, 0)
  )

  # Done
  list(layers, volumes)
}
round_budget(create_budget())

update_advection = function(budget, dt) {
  # Update only volumes
  volumes  = budget[[2]]
  # Compute advection 
  h  = 4.4
  vr = 1 - exp(-ventilation_rate/3600*dt)
  Vr = h*vr
  T_out = volumes$T[1]
  T_in  = volumes$T[2]
  indoors_dT   = Vr*(T_out - T_in)
  volumes$A[2] = indoors_dT*1124
  # Done
  list(budget[[1]], volumes)
}

update_temperature = function(budget, dt) {
  # Distinguish budgets
  layers  = budget[[1]]
  volumes = budget[[2]]
  # Updates layers
  dT     = (layers$A + layers$A_ - layers$E - layers$E_ + layers$H + layers$H_)/layers$C*dt
  layers$T = layers$T + dT
  layers$A = layers$A_ =
  layers$H = layers$H_ = 0
  # Update volumes
  dT          = volumes$H/volumes$C*dt + volumes$A/volumes$C
  volumes$T = volumes$T + dT
  volumes$H =
  volumes$A = 0
  # Done
  list(layers, volumes)
}
update_temperature(create_budget(), 30)


max_dT = function(budget1, budget2) {
  max(
    c(
      abs(budget1[[1]]$T - budget2[[1]]$T), 
      abs(budget1[[2]]$T - budget2[[2]]$T)
    ),
    na.rm=TRUE
  )
}

budget = create_budget()
dt = 30
sub_step = 30

resolve_budget = function(budget, dt, sub_step) {
  steps  = ceiling(dt/sub_step)
  sub_dt = dt/steps
  sky_balance         = 
  plant_balance       = 
  outdoors_balance    = 
  soil_balance        = 
  indoors_ventilation = 0
  B6 = budget
  for (i in 1:steps) {
    B1 = update_driving(B6)
    B2 = update_lw_emission(B1)
    B3 = list(distribute_rad_iteratively(B2[[1]]), B2[[2]])
    B4 = update_heat_fluxes(B3)
    B5 = update_advection(B4, sub_dt)
    B6 = update_temperature(B5, sub_dt)

    layers = B3[[1]]
    sky_balance        = sky_balance    + (layers$A_[1] - layers$E[1])*sub_dt
    plant_balance      = plant_balance  + (layers$A[5]  + layers$A_[5] - layers$E[5] - layers$E_[5])*sub_dt

    layers  = B4[[1]]
    volumes = B4[[2]]
    plant_balance      = plant_balance    + (layers$H[5] + layers$H_[5])*sub_dt
    outdoors_balance   = outdoors_balance + volumes$H[1]*sub_dt
    soil_balance       = soil_balance     + volumes$H[3]*sub_dt

    volumes = B5[[2]]
    outdoors_balance   = outdoors_balance - volumes$A[2]
  }
  list(B6, sky_balance, plant_balance, outdoors_balance, soil_balance)
}

B0 = create_budget()
B = resolve_budget(create_budget(), 180, 30)
round_budget(B0)
round_budget(B[[1]])

. = data.frame( 
  glass_dT        = B[[1]][[1]]$T[2] - B0[[1]]$T[2],
  screen_dT       = B[[1]][[1]]$T[3] - B0[[1]]$T[3],
  floor_dT        = B[[1]][[1]]$T[7] - B0[[1]]$T[7],
  indoors_dT      = B[[1]][[2]]$T[2] - B0[[2]]$T[2]
)                  
.$watt_influx       = sum(c(sw_budget$A, sw_budget$A_)) 
.$total_influx      = .$watt_influx*180
                   
.$sky_absorbed      = B[[2]]
.$glass_absorbed    = .$glass_dT  *B0[[1]]$C[2]
.$screen_absorbed   = .$screen_dT *B0[[1]]$C[3]
.$plant_absorbed    = B[[3]]
.$floor_absorbed    = .$floor_dT  *B0[[1]]$C[7]
                   
.$indoors_absorbed  = .$indoors_dT*B0[[2]]$C[2]
.$outdoors_absorbed = B[[4]]
.$soil_absorbed     = B[[5]]
.$total_absorbed    = sum(.[,7:14])
round(t(.),1)

