# Prelude
source("6-water-budget.R")

# Energy budget
budget_complete = function(dt) {
  # Radiation and convection budget
  budget = budget_lw_con_adv_plant(dt)
  # Add latent heat
  budget$Layers$Cn = 0
  # Add relative humidity
  budget$Volumes$RH=c(60,90,NA)
  # Done
  budget
}

update_humidity_by_trans_cond = function(budget, dt) {
  # Prelude
  lay   = budget$Layers
  vol   = budget$Volumes
  water = budget$Water
  # Transpiration 
  A_rad_plant = with(lay, (A[5] + A_[5] - E[5] - E_[5]))
  trans = transpiration(A_rad_plant, vol$RH[2], vol$T[2])
  water$Tr[2] = trans*dt
  # Condensation
  Cai = 1.241
  c = 2e-6
  Lhe = 2454e3
  Tglass = lay$T[2]
  Tin = vol$T[2]
  RHin = vol$RH[2]
  vp_in = RHin/100*svp(Tin)
  abs_hum_in = abs_humidity(vp_in, Tin)
  vp_sat_glass = svp(Tglass)
  abs_hum_sat_glass = abs_humidity(vp_sat_glass, Tglass)
  cond = Cai*c*max(abs_hum_in - abs_hum_sat_glass, 0)
  water$Cn[2] = -cond*dt
  lay$Cn[2] = Lhe*cond*dt
  # Update indoors RH
  R = 8.314
  T0 = 273.15
  Mw = 18e-3
  abs_hum_in
  abs_hum_in = abs_hum_in + (trans - cond)*dt
  abs_hum_in
  vp_in = abs_hum_in*R*(Tin+T0)/Mw
  rh_in = min(100*vp_in/svp(Tin), 100)
  vol$RH[2] = rh_in
  # Done
  list(Layers=lay, Volumes=vol, Water=water)
}

update_humidity_by_ventilation = function(budget, dt) {
  ah = function(rh, T) {
    vp = rh/100*svp(T)
    abs_humidity(vp, T)
  }
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  water = budget$Water
  v = 3
  # Absolute humidity
  AHin  = ah(vol$RH[2], vol$T[2])
  AHout = ah(vol$RH[1], vol$T[1])
  change_AHin = delta_vent(AHin, AHout, v, dt)
  AHin  = AHin + change_AHin
  water$Mv[1] = -change_AHin
  water$Mv[2] =  change_AHin
  # Relative humidity
  R = 8.314
  T0 = 273.15
  Mw = 18e-3
  Tin = vol$T[2]
  vp_in = AHin*R*(Tin+T0)/Mw
  rh_in = min(100*vp_in/svp(Tin), 100)
  vol$RH[2] = rh_in
  # Done
  list(Layers=lay, Volumes=vol, Water=water)
}

update_temperature_by_latent_heat = function(budget, dt) {
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  # Update glass by latent heat (only glass can have Cn>0)
  dT     = with(lay, Cn/C*dt)
  lay$T  = lay$T + dT
  lay$Cn = 0
  # Done
  list(Layers=lay, Volumes=vol)
}

water_budget = function() {
  data.frame(
    Volume = c("Outdoors", "Indoors"),
    Tr = c(0,0),
    Cn = c(0,0),
    Mv = c(0,0)
  )
}

incl_water = function(budget) {
  append(budget, list(Water = water_budget()))
}

sum_water = function(water1, water2) {
  cbind(Volume=water1[,1], water1[,2:4]+water2[,2:4])  
}

# Distribute convective heat over time step ('dt') divided into 'n' smaller time steps
# distribute_final = function(budget, dt, n) {
  # max_dT = function(budget1, budget2) {
    # max(abs(c(budget2$Layers$T - budget1$Layers$T, budget2$Volumes$T - budget1$Volumes$T)), na.rm=TRUE)
  # }
  
  # sub_dt = dt/n
  # B12 = budget 
  # water = water_budget()
  # dT_step = NULL
  # for (i in 1:n) {
    # B1         = B12
    # B2         = B1
    # B2$Layers  = distribute_radiation(B1$Layers)
    # B3         = update_convection(B2)
    # B4         = update_humidity_by_trans_cond(incl_water(B3), sub_dt)
    # water      = sum_water(water, B4$Water)

    # layersSumNetAH      = with(B4$Layers, A+A_+H+H_-E-E_)
    # volumesSumNetHV     = with(B4$Volumes, H+V)
    # B4$Layers$SumNetAH  = if (i==1) layersSumNetAH  else B4$Layers$SumNetAH + layersSumNetAH 
    # B4$Layers$SumCn     = if (i==1) B4$Layers$Cn    else B4$Layers$SumCn    + B4$Layers$Cn 
    # B4$Volumes$SumNetHV = if (i==1) volumesSumNetHV else B4$Volumes$SumNetH + volumesSumNetHV

    # B5         = update_plant_temperature(B4)
    # B6         = B5
    # B6$Layers  = update_temperature_by_radiation(B5$Layers, sub_dt)
    # B7         = update_temperature_by_convection(B6, sub_dt)
    # B8         = update_temperature_by_advection(B7, sub_dt)
    # B9         = update_temperature_by_latent_heat(B8, sub_dt)

    # B10        = update_humidity_by_ventilation(incl_water(B9), sub_dt)
    # water      = sum_water(water, B10$Water)

    # B11         = B10
    # B11$Layers  = update_lw_emission(B10$Layers)
    # B12         = B11
    # B12$Layers  = update_driving_radiation(B11$Layers)
    # B12         = update_driving_convection(B12)
    # B12         = update_driving_advection(B12, sub_dt)
    
    # dT_step    = c(dT_step, max_dT(B1, B10))

  # }
  # B10$Water = water
  # c(B10, dT_step)
# }

distribute_final = function(budget, dt, n) {
  max_dT = function(budget1, budget2) {
    max(abs(c(budget2$Layers$T - budget1$Layers$T, budget2$Volumes$T - budget1$Volumes$T)), na.rm=TRUE)
  }

  sub_dt = dt/n
  B8 = budget 
  dT_step = NULL
  for (i in 1:n) {
    B1         = B8
    B2         = B1
    B2$Layers  = distribute_radiation(B1$Layers, FALSE)
    B3         = update_convection(B2)

    layersSumNetAH  = with(B3$Layers, A+A_+H+H_-E-E_)
    volumesSumNetHV = with(B3$Volumes, H+V)
    B3$Layers$SumNetAH  = if (i==1) layersSumNetAH  else B3$Layers$SumNetAH + layersSumNetAH 
    B3$Volumes$SumNetHV = if (i==1) volumesSumNetHV else B3$Volumes$SumNetH + volumesSumNetHV

    B4         = update_humidity_by_trans_cond(incl_water(B3), sub_dt)
    B4$Layers$SumCn = if (i==1) B4$Layers$Cn    else B4$Layers$SumCn    + B4$Layers$Cn 
    B5         = update_humidity_by_ventilation(incl_water(B4), sub_dt)
    water      = sum_water(water, B5$Water)


    B6         = update_temperature(B5, sub_dt)
    B7         = B6
    B7$Layers  = update_lw_emission(B6$Layers)
    B8         = B7
    B8$Layers  = update_driving_radiation(B7$Layers)
    B8         = update_driving_convection(B8)
    B8         = update_driving_advection(B8, sub_dt)
        
    dT_step    = c(dT_step, max_dT(B1, B6))
  }
  B6$Water = water
  c(B6, dT_step)
}

#
# Main
#

# Set up
sub_dt     = 30
water      = water_budget()
B1         = budget_complete(sub_dt)

# Final update
final = distribute_final(B1,180, 6)
final$Layers$SumNetAH  = final$Layers$SumNetAH*dt/n
final$Volumes$SumNetHV = final$Volumes$SumNetHV*dt/n
final
round_budget(final)

# Check whole budget
Lhe = 2454e3
list(
  DeltaTglass         = final$Layers$SumCn[2]/final$Layers$C[2],
  EscapedLatentHear   = final$Water$Mv[1]*Lhe,
  TotalBalanceLayers  = sum(final$Layers$SumNetAH),
  CnBalanceGlass      = final$Layers$SumCn[2],
  TotalBalanceVolumes = sum(final$Volumes$SumNetHV),
  GlassLatentHeat     = -final$Water$Cn[2]*Lhe,
  TotalBalance        = sum(final$Layers$SumNetAH) + sum(final$Volumes$SumNetHV)
)

budget = B1
dt = 180
n = 6
i = 1
