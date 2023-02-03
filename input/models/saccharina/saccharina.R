library(KeyboardSimulator)
sim$date = date(sim$date)
obs = read.table(paste0(box_script_folder, "/data/obs-2019.txt"), sep="\t", header=TRUE)
obs = obs[c("Date", "Wtotal", "Cconc", "Nconc", "Pconc")]
obs$Date = dmy(obs$Date)

com = sim[c("date", "whole.dw", "concC", "concN", "concP")]
common_names = c("Date", "Biomass", "ConcC", "ConcN", "ConcP")
colnames(obs) = common_names
colnames(com) = common_names

env = sim[c("date", "T", "E0", "N", "S", "P")]
colnames(env) = c("Date","Temperature","PAR","WaterN", "Salinity", "WaterP")

# sd = sim[c("date", "phiT", "carbon.sdRatio", "scalingNitrogen", "scalingPhosphorus", "phiSal")]
# colnames(sd) = c("Date", "TempScaling", "CarbonSD", "Nscaling", "Pscaling", "SalinityScaling")

sd = sim[c("date", "propIabsorbed", "carbon.sdRatio", "nitrogen.sdRatio", "phosphorus.sdRatio")]
colnames(sd) = c("Date", "propIabsorbed", "CarbonSD", "NitrogenSD", "PhosphorusSD" )


obsM = melt(obs, id.vars="Date", variable.name="Variable", value.name="Value")
simM = melt(com, id.vars="Date", variable.name="Variable", value.name="Value")
envM = melt(env, id.vars="Date", variable.name="Variable", value.name="Value")
sdM  = melt(sd,  id.vars="Date", variable.name="Variable", value.name="Value")
# sd2M = melt(sd2, id.vars="Date", variable.name="Variable", value.name="Value")

envM$Variable = reorder_levels(envM$Variable, c(1:3,5,4))

scale_x = scale_x_date(breaks = date_breaks('2 months'), labels = date_format('%m'))
line_alpha = 0.5

P = ggplot(obsM, aes(Date, Value, colour=Variable)) +
  geom_point(size=2) +
  geom_line(data = simM, alpha=line_alpha) +
  scale_x +
  labs(x="") +
  facet_wrap(~Variable, ncol=1, scales="free_y") +
  theme(legend.position="none")
Q = ggplot(obsM, aes(Date, log10(Value), colour=Variable)) +
  geom_point(size=2) +
  geom_line(data = simM, alpha=line_alpha) +
  scale_x +
  labs(x="Month") +
  facet_wrap(~Variable, ncol=1, scales="free_y") +
  theme(legend.position="none")
R = ggplot(envM, aes(Date, Value, colour=Variable)) +
  geom_line() +
  scale_x +
  labs(x="") +
  facet_wrap(~Variable, ncol=1, scales="free_y") +
  theme(legend.position="none")
S = ggplot(sdM, aes(Date, Value, colour=Variable)) +
  geom_line() +
  scale_x +
  labs(x="") +
  facet_wrap(~Variable, ncol=1, scales="free_y") +
  theme(legend.position="none")
# T = ggplot(sd2M, aes(Date, Value, colour=Variable)) +
  # geom_line() +
  # scale_x +
  # labs(x="") +
  # facet_wrap(~Variable, ncol=1, scales="free_y") +
  # theme(legend.position="none")
graphics.off()
windows(10,7)
print(ggarrange(P,Q,R,S,nrow=1))


