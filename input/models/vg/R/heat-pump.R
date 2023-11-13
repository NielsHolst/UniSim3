rm(list=ls(all=TRUE))
graphics.off()
source("~/RDev/setup.R")

# Parameters
groundArea = 10000 # m2
volume = 44384 # m3
indoorsTemperature = 28 # oC
indoorsRh = 95 # %
state = 1 # [0;1]
number = 1
maxCoolingLoad = 78.4 # kW
coolingEfficiency = 0.69 # [0;1]
maxFlowRate = 2.5 # m3/s
parasiticLoad = 2.2 # kW  
coolingTemperature = 10 # oC
LHe = 2454e3 # J/kg

# Physical functions

ahFromVp = function(temperature, vp) {
  Mwater = 18.016e-3
  R = 8.314
  T0 = 273.15
  vp*Mwater/(R*(temperature + T0))
}

svp = function(temperature) {
  610.8*exp(17.27*temperature/(temperature+237.3))
}

sah = function(temperature) {
  ahFromVp(temperature, svp(temperature))
}

ahFromRh = function(temperature, rh) {
  rh/100*sah(temperature)
}

rhFromAh = function(temperature, ah) {
  min(100*ah/sah(temperature), 100.)
}

# The heat pump
heat_pump = function() {
  # Compute for one unit
  coolingLoad = maxCoolingLoad/coolingEfficiency*state # kW
  beforeAh = ahFromRh(indoorsTemperature, indoorsRh) # kg/m3
  afterAh = ahFromRh(coolingTemperature, 100.) # kg/m3

  condensationRate = max(beforeAh - afterAh, 0.)*maxFlowRate*state # kg/s
  powerUseCooling = coolingLoad;
  powerUserParasitic = parasiticLoad*state;
  powerUse =  powerUseCooling + powerUserParasitic;
  cooling = coolingLoad + LHe*condensationRate*1e-3; # J/kg * kg/s * kW/(1000*W) = kW

  # Correct for number of units and area;
  # also convert kW -> W
  k = 1000.*number/groundArea;
  data.frame(
    condensationRate = condensationRate,
    powerUseCooling = powerUseCooling*k,
    powerUserParasitic = powerUserParasitic*k,
    powerUse = powerUse*k,
    cooling = cooling*k
  )
}
t(heat_pump())

time_step = 60 #s
T = 28
RH = 100
minutes = 120

result = RH
for (i in 1:minutes) {
  water_mass = volume*ahFromRh(T, RH) # m3 * kg/m3 = kg
  dehum_rate = heat_pump()$condensationRate # kg/s
  water_mass = water_mass - dehum_rate*time_step
  AH = water_mass/volume # kg/m3
  RH = rhFromAh(T, AH)
  result = c(result, RH)
}
M = data.frame(
  Minute = 0:minutes,
  RH = result
)
ggplot(M, aes(Minute, RH)) +
  geom_line(colour=blue, linewidth=1)
  