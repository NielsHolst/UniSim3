# Prelude
source("5-energy-budget-plant.R")

abs_humidity = function(press_humidity, T) {
  R = 8.314
  T0 = 273.15
  Mw = 18e-3
  Mw*press_humidity/R/(T+T0)
}

plot_abs_humidity = function() {
  M = expand.grid(
    T  = 10:30,
    RH = c(60,90)
  )
  M$VP = with(M, RH/100*svp(T))
  M$AH = with(M, abs_humidity(VP, T))

  windows(5,3)
  ggplot(M, aes(T, AH, colour=factor(RH))) +
    geom_line(size=1) +
    labs(
      x="Temperature (oC)", 
      y="Absolute humidity (kg/m3)",
      colour="RH (%)"
    ) +
    guides(colour=guide_legend(reverse=TRUE)) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

condensation = function(RHin, Tglass, g=2e-3) {
  Tin = 24
  # At glass
  svp_glass = svp(Tglass)
  sat_abs_hum_glass = abs_humidity(svp_glass, Tglass)
  # In air
  vp_in = RHin/100*svp(Tin)
  abs_hum_in = abs_humidity(vp_in, Tin)
  pmax(g*(abs_hum_in - sat_abs_hum_glass), 0)
}

plot_condensation_estimation = function() {
  M = expand.grid(
    Tglass = c(13, 15, 17),
    RHin = 30:100
  )
  M$Condensation = with(M, condensation(RHin, Tglass))

  Lacroix = data.frame(
    RHin = c(70,80,85,90),
    Condensation = c(4.2,7.9,9.7,12.4)/1000/1000,
    Tglass = 15
  )

  windows(5,3)
  ggplot(M, aes(RHin, 1000*Condensation, colour=factor(Tglass))) +
    geom_line(size=1) +
    geom_point(data=Lacroix, colour="black", size=2.5) +
    scale_colour_manual(values=c(orange, brown, violet)) +
    labs(
      x="Indoors RH (%)", 
      y="Condensation rate\n(g/m2 glass/s)",
      colour="Glass\ntemperature\n(oC)"
    ) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

plot_condensation = function() {
  M = expand.grid(
    Tglass = 0:30,
    RHin = c(60,90)
  )
  M$Condensation = with(M, condensation(RHin, Tglass))

  windows(5,3)
  ggplot(M, aes(Tglass, 1000*Condensation, colour=factor(RHin))) +
    geom_vline(xintercept=24, size=1, linetype="dashed", alpha=0.5) +
    geom_line(size=1) +
    labs(
      x="Glass temperature (oC)", 
      y="Condensation rate\n(g/m2 ground/s)",
      colour="Indoors\nRH (%)"
    ) +
    guides(colour=guide_legend(reverse=TRUE)) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

abs_hum_flux = function(RHin0, RHout) {
  Tin = 24
  Tout = 10
  v = 3
  dt = 120
  h = 3.94
  vp_in_0 = RHin0/100*svp(Tin)
  vp_out = RHout/100*svp(Tout)
  abs_hum_in_0 = abs_humidity(vp_in_0, Tin)
  abs_hum_out = abs_humidity(vp_out, Tout)
  (abs_hum_out - abs_hum_in_0)*(1 -exp(-v/3600*dt))*h/dt
}

latent_heat = function(RHin0, RHout) {
  lambda = 2454e3
  -lambda*abs_hum_flux(RHin0, RHout)
}

plot_latent_heat = function() {
  M = expand.grid(
    RHin0 = c(60,90),
    RHout = 30:100
  )
  M$LatentHeat = with(M, latent_heat(RHin0, RHout))

  windows(5,3)
  ggplot(M, aes(RHout, LatentHeat, colour=factor(RHin0))) +
    geom_line(size=1) +
    labs(
      x="Outdoors RH (%)", 
      y="Latent heat flux (W/m2)",
      colour="Indoors\nRH (%)"
    ) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
  windows(5,3)
  ggplot(M, aes(RHout, LatentHeat, colour=factor(RHin0))) +
    geom_line(size=1) +
    labs(
      x="Outdoors RH (%)", 
      y="Change in glass temperature (K/s)",
      colour="Indoors\nRH (%)"
    ) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}


plot_abs_hum_flux = function() {
  M = expand.grid(
    RHin0 = c(60,90),
    RHout = 30:100
  )
  M$AbsHumFlux = with(M, abs_hum_flux(RHin0, RHout))

  windows(5,3)
  ggplot(M, aes(RHout, 1000*AbsHumFlux, colour=factor(RHin0))) +
    geom_line(size=1) +
    labs(
      x="Outdoors RH (%)", 
      y="Mass movement of water vapour\n(g/m2/s)",
      colour="Indoors\nRH (%)"
    ) +
    theme_bw() +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

#
# Main
#

# setwd("~/sites/ecolmod3/media/models/vg")
# plot_abs_humidity()
# plot_condensation_estimation()
# plot_condensation()
# plot_abs_hum_flux()

