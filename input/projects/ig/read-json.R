rm(list=ls(all=TRUE))
graphics.off()

library(ggplot2)
library(gridExtra)
library(lubridate)
library(plyr)
library(reshape2)
library(rjson)

setwd("/Users/au152367/Documents/QDev/UniSim2/input/projects/ig")
json = fromJSON(file="UnisimInput_2019-08-01_2019-08-31.json")


print(paste("Number of samples:", length(json)))
print(paste("Items per sample :", length(json[[1]])))
names(json[[1]])

obs = json[[1]]
constr = obs$Construction

date_time = function(day, hour) {
  x = dmy("31/12/2018")
  h = floor(hour)
  m = floor(60*(hour - h))
  s = floor(3600*(hour - h) - 60*m)
  x + day + hours(h) + minutes(m) + seconds(s)
}

get_screens = function(screens) {

  get_effect = function(screen) {
    screen$Effect$Value/100
  }
  get_net_effect = function(screen) {
    (1 - screen$Material$TransmissivityLight) * screen$Effect$Value/100
  }

  M1 = data.frame(t(ldply(screens, get_effect)))
  colnames(M1) = paste0("ScreenEffect", 1:length(screens))
  M2 = data.frame(t(ldply(screens, get_net_effect)))
  colnames(M2) = paste0("ScreenNetEffect", 1:length(screens))
  M = cbind(M1, M2)
  rownames(M) = {}
  M
}

get_vents = function(vents) {

  get_vent_effect = function(vent) {
    vent$Opening$Value
  }

  M = data.frame(t(ldply(vents, get_vent_effect)))
  colnames(M) = paste0("VentEffect", 1:length(vents))
  M
}

get_data = function(obs) {
  area = obs$Construction$Length * obs$Construction$SpanWidth * obs$Construction$SpanCount 
  indoors = obs$IndoorClimate
  outdoors = obs$OutdoorClimate
  growthLights = obs$GrowthLights
  if (length(growthLights) > 1) stop("More than one element in growthLights")
  growthLight = growthLights[[1]]
  M = data.frame(
    DateTime = NA,
    Date = NA,
    Time = NA,
    Day = obs$TimeStamp$DayOfYear,
    Hour = obs$TimeStamp$TimeOfDay,
    IndoorsTemperature = indoors$Temperature$Value,
    IndoorsLightIntensity = indoors$LightIntensity$Value*7.9,  # Convert from klux to W/m2 (sunlight spectrum)
    IndoorsRH = indoors$RelativeHumidity$Value,
    IndoorsCo2 = indoors$Co2$Value,
    OutdoorsTemperature = outdoors$Temperature$Value,
    OutdoorsIrradiation = outdoors$Irradiation$Value,
    OutdoorsCo2 = outdoors$Co2$Value,
    OutdoorsWindSpeed = outdoors$WindSpeed$Value,
    OutdoorsWindDirection = outdoors$WindDirection$Value,
    GrowthLightPowerUsage = growthLight$PowerUsage,
    GrowthLightIntensity = growthLight$LightIntensity
  ) 
  x = date_time(M$Day, M$Hour)
  M$DateTime = x
  M$Date = paste(day(x),month(x),year(x),sep="/")
  M$Time = paste(hour(x),minute(x),second(x),sep=":")
  cbind(M, get_screens(obs$Screens), get_vents(obs$Vents))
}

# M0 = ldply(json, get_data)
# save(M0, file="M0.Rdata")
# M0[M0$Hour<0.37, ]
load("M0.Rdata")
M0 = M0[1:1440,]
write.table(M0[, c(2,3,6,8,9,10,11,13,16:22)], "ig-sensor-records.txt", quote=FALSE, row.names=FALSE, sep="\t")

plot_sensors = function() {
  M = melt(M0[, c(1, 4:12)], id.vars="DateTime", value.name="Value", variable.name="Variable")
  windows(12,6)
  ggplot(M, aes(x=DateTime, y=Value, colour=Variable)) +
    geom_line() +
    ylab("") + 
    theme_classic() +
    facet_wrap(~Variable, scales="free")
}

plot_actuators = function() {
  M = melt(M0[,c(1, 13:22)], id.vars="DateTime", value.name="Value", variable.name="Variable")
  windows(12,6)
  ggplot(M, aes(x=DateTime, y=Value, colour=Variable)) +
    geom_line() +
    ylab("") + 
    theme_classic() +
    facet_wrap(~Variable, scales="free")
}

plot_outdoors_radiation = function() {
  windows(12,6)
  ggplot(M0, aes(x=OutdoorsIrradiation)) +
    geom_density(fill="green", alpha=0.3) 
}

plot_sensors()
plot_actuators()

# model = lm(IndoorsLightIntensity ~ OutdoorsIrradiation + OutdoorsIrradiation:ScreenEffect1 + OutdoorsIrradiation:ScreenEffect2, 
           # data=subset(M0, IndoorsLightIntensity>100 & GrowthLightIntensity==0))
# summary(model)
# plot(model, 1)

# ggplot(M0, aes(x=OutdoorsIrradiation, y=predict(model), colour=Hour)) +
  # geom_point() +
  # theme_minimal()
