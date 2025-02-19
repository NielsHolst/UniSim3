# Prelude
source("4-energy-budget-advection.R")

ri = function(RH, CO2) {
  g0 = 0.1
  g1 = 1.64
  Pn = 2
  (g0 + g1*RH/100*Pn/CO2)^-1
}

re = function(Tplant, Tin, u=0.1, L=0.05) {
  1174*sqrt(L)/(L*abs(Tplant-Tin) + 207*u^2)^0.25
}

svp = function(T) {
  610.8*exp(17.27*T/(T+237.3))
}
svp(25) # == 3168. OK. Check Table 2.3 in https://www.fao.org/3/x0490e/x0490e0j.htm#annex%202.%20meteorological%20tables 

svp_slope = function(T) {
  4098*svp(T)/(T+237.3)^2
}
svp_slope(25) # == 189. OK. Check Table 2.4 in https://www.fao.org/3/x0490e/x0490e0j.htm#annex%202.%20meteorological%20tables 

vp = function(rh, T) {
  rh/100*svp(T)
}

plot_svp = function() {
  M = data.frame(
    T = 10:35
  )
  M$svp = with(M, svp(T))

  windows(5,3)
  ggplot(M, aes(T, svp)) +
    geom_line(size=1, colour=red) +
    labs(
      x="Temperature (oC)", 
      y="Saturated water vapour pressure\n(Pa)" 
    ) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

plot_svp_slope = function() {
  M = data.frame(
    T = 10:35
  )
  M$svp_slope = with(M, svp_slope(T))

  windows(5,3)
  ggplot(M, aes(T, svp_slope)) +
    geom_line(size=1, colour=red) +
    labs(
      x="Temperature (oC)", 
      y="Saturated water vapour pressure\nslope (Pa/K)" 
    ) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

Tplant = function(Arad, rh, Tin=25, ri=9.5, re=200) {
  Lai = 1.9
  rhoa = 1.19
  Ca = 1020
  gamma = 67.71
  
  a = (ri+re)/2/Lai/rhoa/Ca*Arad - 1/gamma*(svp(Tin) - vp(rh, Tin))
  b = 1 + svp_slope(Tin)/gamma + ri/re
  Tin + a/b
}

transpiration = function(Arad, rh, Tin=25) {
  co2 = 900
  re = 218
  Lai = 1.9
  rhoa = 1.19
  Ca = 1020
  gamma = 67.71
  lambda = 2454e3 # J/kg
  
  a = svp_slope(Tin)/gamma*Arad + 2*Lai*rhoa*Ca/gamma/re*(svp(Tin) - vp(rh, Tin))
  b = lambda*(1 + svp_slope(Tin)/gamma + ri(rh, co2)/re)
  a/b
}

plot_transpiration = function() {
  M = expand.grid(
    Arad = 5*(0:50),
    RH = c(60, 90)
  )
  M$transpiration = with(M, transpiration(Arad, RH))*1000*60

  windows(5,3)
  ggplot(M, aes(Arad, transpiration, colour=factor(RH))) +
    geom_line(size=1) +
    labs(
      x="Plant canopy net absorbed radiation (W/m2)", 
      y="Transpiration (g/m2/min)", 
      colour="RH (%)"
    ) +
    ylim(0,NA) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

plot_latent_heat = function() {
  M = expand.grid(
    Arad = 5*(0:50),
    RH = c(60, 90)
  )
  M$transpiration = with(M, transpiration(Arad, RH))
  M$LE = M$transpiration*2454e3 

  windows(5,5)
  ggplot(M, aes(Arad, LE, colour=factor(RH))) +
    geom_abline(slope=1, intercept=0, size=1, linetype="dashed", alpha=0.5) +
    geom_line(size=1) +
    labs(
      x="Plant canopy net absorbed radiation (W/m2)", 
      y="Latent heat lost (W/m2)", 
      colour="RH (%)"
    ) +
    xlim(0, 250) +
    ylim(0, 250) +
    coord_equal()+
    annotate("text", x=180, y= 80, label="Canopy warmer\nthan the air", alpha=0.5, size=6) +
    annotate("text", x= 75, y=160, label="Canopy cooler\nthan the air", alpha=0.5, size=6) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

plot_Tplant = function() {
  M = expand.grid(
    Arad = 5*(0:40),
    RH = c(60, 90)
  )
  M$Tplant = with(M, Tplant(Arad, RH))

  windows(5,3)
  ggplot(M, aes(Arad, Tplant, colour=factor(RH))) +
    geom_hline(yintercept=25, size=1) +
    geom_line(size=1) +
    labs(
      x="Plant canopy net absorbed radiation (W/m2)", 
      y="Plant canopy temperature (oC)", 
      colour="RH (%)"
    ) +
    scale_y_continuous(breaks=20:26) +
    guides(colour=guide_legend(reverse=TRUE)) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

plot_ri = function() {
  M = expand.grid(
    RH = 50:100,
    CO2 = c(400, 900, 1500)
  )
  M$ri = with(M, ri(RH, CO2)) 

  windows(5,3)
  ggplot(M, aes(RH, ri, colour=factor(CO2))) +
    geom_line(size=1) +
    labs(
      x="Relative humidity (%)", 
      y="Internal resistance (s/m)", 
      colour="CO2 (ppm)"
    ) +
    guides(colour=guide_legend(reverse=TRUE)) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

plot_re = function() {
  M = expand.grid(
    Tplant = c(24, 27, 30),
    Tin    = 24,
    u      = (0:60)/100,
    L      = c(0.05, 0.2)
  )
  M$re = with(M, re(Tplant, Tin, u, L))

  windows(5,3)
  ggplot(M, aes(u, re, colour=factor(Tplant), linetype=factor(L))) +
    geom_line(size=1) +
    scale_linetype_manual(values=c("solid", "dashed"))+
    ylim(0,700) +
    labs(
      x="Leaf wind speed (m/s)", 
      y="External resistance (s/m)", 
      colour="Leaf\ntemperature", 
      linetype="Leaf\ndimension"
    ) +
    guides(linetype=guide_legend(reverse=TRUE)) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

# Energy budget
budget_lw_con_adv_plant = function(dt) {
  # Radiation and convection budget
  budget = budget_lw_con_adv(dt)
  # U for plant layer is zero
  budget$Layers$U [5] = 0
  budget$Layers$U_[5] = 0
  # With driving variables
  update_driving_advection(budget, dt)
}

# Update plant temperature from net radiation absorption
update_plant_temperature = function(budget) {
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  # Get plant net radiation
  Arad = with(lay, A[5] + A_[5] - E[5] - E_[5])
  lay$A [5] = 
  lay$A_[5] = 0
  # Set plant temperature
  rh = 90
  co2 = 900
  # Tpl = lay$T[5]
  Tin = vol$T[2]
  lay$T[5] = Tplant(Arad, rh, Tin, ri(rh, co2), 218) #re(Tpl, Tin))
  # Done
  list(Layers=lay, Volumes=vol)
}

update_temperature = function (budget, sub_dt) {
  # Update plant
  budget = update_plant_temperature(budget)
  # Prelude
  lay = budget$Layers
  vol = budget$Volumes
  # Update other than plant
  ix = c(2:3, 7)
  heat_flux = with(lay, A + A_ - E - E_ + H + H_)
  lay$T[ix] = lay$T[ix] + heat_flux[ix] / lay$C[ix] * sub_dt 
  # Update indoors
  vol$T = vol$T + with(vol, (H+V)/C*sub_dt)
  # Done
  list(Layers=lay, Volumes=vol)
}

# Distribute convective heat over time step ('dt') divided into 'n' smaller time steps
distribute_lw_conv_adv_plant = function(budget, dt, n) {
  max_dT = function(budget1, budget2) {
    max(abs(c(budget2$Layers$T - budget1$Layers$T, budget2$Volumes$T - budget1$Volumes$T)), na.rm=TRUE)
  }

  sub_dt = dt/n
  B6 = budget 
  dT_step = NULL
  for (i in 1:n) {
    B1         = B6
    B2         = B1
    B2$Layers  = distribute_radiation(B1$Layers, FALSE)
    B3         = update_convection(B2)

    layersSumNetAH  = with(B3$Layers, A+A_+H+H_-E-E_)
    volumesSumNetHV = with(B3$Volumes, H+V)
    B3$Layers$SumNetAH  = if (i==1) layersSumNetAH  else B3$Layers$SumNetAH + layersSumNetAH 
    B3$Volumes$SumNetHV = if (i==1) volumesSumNetHV else B3$Volumes$SumNetH + volumesSumNetHV

    B4         = update_temperature(B3, sub_dt)
    B5         = B4
    B5$Layers  = update_lw_emission(B4$Layers)
    B6         = B5
    B6$Layers  = update_driving_radiation(B5$Layers)
    B6         = update_driving_convection(B6)
    B6         = update_driving_advection(B6, sub_dt)
        
    dT_step    = c(dT_step, max_dT(B1, B4))
  }
  c(B4, dT_step)
}

#
# Main
#

# Show figures
# plot_svp()
# plot_svp_slope()
# plot_ri()
# plot_re()
# plot_Tplant()
# plot_transpiration()
# plot_latent_heat()


# Set up
sub_dt     = 30
B1         = budget_lw_con_adv_plant(sub_dt)

# Final update
final = distribute_lw_conv_adv_plant(B1, 180, 6)

final$Layers$SumNetAH  = final$Layers$SumNetAH*dt/n
final$Volumes$SumNetHV = final$Volumes$SumNetHV*dt/n
final
round_budget(final)

# Check whole budget
list(
  TotalBalanceLayers  = sum(final$Layers$SumNetAH),
  TotalBalanceVolumes = sum(final$Volumes$SumNetHV),
  TotalBalance        = sum(final$Layers$SumNetAH) + sum(final$Volumes$SumNetHV)
)
