library(lubridate)
setwd("C:/Users/au152367/Documents/QDev/UniSim2/input/projects/ig")

# write.table(sim, "simulation.txt", sep="\t", quote=FALSE)

sim = read.table("simulation.txt", sep="\t", header=TRUE)

sim$Date = date(sim$dateTime)

sim = subset(sim, Date==dmy("10/5/2001"))
sim$Minute = 1:nrow(sim)
head(sim)
tail(sim)

M = data.frame(
  minute = sim$Minute,
  indoorsCo2 = sim$co2.value,
  indoorsRh = sim$rh,
  indoorsTemperature = sim$temperature.value,
  growthLight = sim$powerUsage,
  heating = sim$energyFlux_0 + sim$energyFlux_1,
  photosynthesis = sim$netGrowthRate
  )

write.table(M, "simulation-1440-minutes.txt", sep="\t", quote=FALSE, row.names=FALSE)
