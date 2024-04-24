rm(list=ls(all=TRUE))
library(psychrolib)
SetUnitSystem("SI")

tIndoor      = 20
rhIndoor     = 0.80
tDewpoint    = 10
flowRate     = 2.5   # m3/s
indoorArea   = 10000 # m2
airDensity   = 1.19  # kg/m3 
airPressure  = 101330
                    
specHumidityIn  = GetSpecificHumFromHumRatio(GetHumRatioFromRelHum(tIndoor,rhIndoor,airPressure))  # kg/kg
specHumidityOut = GetSpecificHumFromHumRatio(GetHumRatioFromRelHum(tDewpoint,1,airPressure))       # kg/kg
                    
absHumidityIn  = airDensity*specHumidityIn  # kg/m3
absHumidityOut = airDensity*specHumidityOut # kg/m3

dehumidificationRate = (absHumidityIn - absHumidityOut)*flowRate # kg/m3 * m3/s = kg/s

paste("Dehumidification rate of heat pump        = ", round(dehumidificationRate,3), "kg/s") |> print()
paste("Dehumidification rate of heat pump        = ", round(dehumidificationRate,3)*3600, "kg/h") |> print()
paste("Dehumidification rate per greenhouse area = ", round(dehumidificationRate/indoorArea*1000*60, 3), "g/m2/min") |> print()
