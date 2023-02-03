# Prelude
rm(list=ls(all=TRUE))
library(ggplot2)
library(lubridate)

# Set folder
setwd("~/QDev/UniSim3/input/models/saccharina/data")

# Read obs
# obs = read.table("obs-2019.txt", sep="\t", header=TRUE)
obs = read.table("obs-2019-soe3-top.txt", sep="\t", header=TRUE)

obs$Date = dmy(obs$Date)
obs$Day  = as.numeric(obs$Date - obs$Date[1])
n = nrow(obs)

# Compute slopes
dx = as.numeric(diff(obs$Date))
dy = diff(log(obs$Wtotal))
slope = signif(dy/dx, 2)

# Slope labels
Label = data.frame(
  Date   = obs$Date[1:(n-1)]   + diff(obs$Date)/2,
  Wtotal = 10^(log10(obs$Wtotal[1:(n-1)]) + diff(log10(obs$Wtotal))/2),
  Slope  = slope
)

# Regress stable growth points
model = lm(log(Wtotal) ~ Day, data = obs[2:(n-1),])
co = coef(model)
Model = data.frame(
   Date   = obs$Date[1],
   xend   = obs$Date[1] + obs$Day[n],
   Wtotal = exp(co[1]),
   yend   = exp((co[1] + co[2]*obs$Day[n]))
)
Model


# Show plot
ggplot(obs, aes(Date,Wtotal)) +
  geom_point(size=3, colour="red") +
  geom_line(size=1, colour="red") +
  # geom_segment(aes(xend=xend, yend=yend), colour="blue", linetype="dashed", data=Model) +
  geom_label(aes(label=Slope), size=2, data=Label) +
  # annotate("label", x=Model$Date, y=Model$Wtotal, label=signif(Model$Wtotal,3), size=2) +
  # annotate("label", x=Model$xend, y=Model$yend, label=signif(Model$yend,3), size=2) +
  scale_x_date(date_breaks="months", date_labels="%m", name="Month") +
  scale_y_continuous(trans='log10', labels=scales::comma, name="Biomass (kg/m)") +
  theme_minimal()

