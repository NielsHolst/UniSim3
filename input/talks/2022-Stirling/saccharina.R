sim$date = date(sim$date)
obs = read.table(paste0(box_script_folder, "/data/obs-2019.txt"), sep="\t", header=TRUE)
obs = obs[c("Date", "Wtotal", "Cconc", "Nconc", "Pconc")]
obs$Date = dmy(obs$Date)

com = sim[c("date", "whole.dw", "concC", "concN", "concP")]
common_names = c("Date", "Biomass", "ConcC", "ConcN", "ConcP")
colnames(obs) = common_names
colnames(com) = common_names

obsM = melt(obs, id.vars="Date", variable.name="Variable", value.name="Value")
simM = melt(com, id.vars="Date", variable.name="Variable", value.name="Value")

P = ggplot(obsM, aes(Date, Value, colour=Variable)) +
  geom_point(size=2) +
  geom_line(data = simM) +
  facet_wrap(~Variable, ncol=1, scales="free_y") +
  theme(legend.position="none")
Q = ggplot(obsM, aes(Date, log10(Value), colour=Variable)) +
  geom_point(size=2) +
  geom_line(data = simM) +
  facet_wrap(~Variable, ncol=1, scales="free_y") +
  theme(legend.position="none")
print(ggarrange(P,Q))

# biomass = sim[c("date", "whole.dw", "structure.dw", "storeC.dw", "storeN.dw")]
# colnames(biomass) = c("Date", "Whole", "Structure", "StoreC", "StoreN")
# biomassM = melt(biomass, id.vars="Date", variable.name="Variable", value.name="Value")

# Q = ggplot(
  # biomassM, aes(Date, Value, colour=Variable)) +
  # geom_line() +
  # facet_wrap(~Variable, ncol=1, scales="free_y") +
  # theme(legend.position="none")

