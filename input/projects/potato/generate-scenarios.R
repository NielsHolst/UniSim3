setwd("/Users/au152367/Documents/QDev/UniSim2/input/projects/potato")
M = expand.grid(
  Year = c(2009,2018),
  CropDevRate = c(0.008, 0.009),
  PestFecundity = c(98, 76, 71),
  Parasitoids = c(0,1,5)
)
M$Scenario = rownames(M)
ix2018 = (M$Year==2018)
M$WeatherFile = "\"../../book/weather/flakkebjerg 2009.txt\""
M$WeatherFile[ix2018] = "\"../../book/weather/flakkebjerg 2018.txt\""
M$StartDate = "1/1/2009"
M$StartDate[ix2018] = "1/1/2018"
write.table(M, "scenarios.txt", quote=FALSE, sep="\t", row.names=FALSE)
M

