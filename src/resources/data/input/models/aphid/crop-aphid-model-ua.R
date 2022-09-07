# crop-aphid-model-ua.R
densities = colnames(sim)[3:9]

M = melt(sim, id.vars=c("iteration", "date"), measure.vars=densities, variable.name="Variable", value.name="Value")
open_plot_window(5,10)
P = ggplot(M, aes(date, Value, colour=Variable, group=iteration)) +
  geom_line(alpha=0.3) +
  labs(y="") +
  theme(legend.position="none") +
  facet_wrap(~Variable, ncol=1, scales="free")
print(P)

M = melt(sim, id.vars=c("iteration", "cropGrowthStage"), measure.vars=densities, variable.name="Variable", value.name="Value")
open_plot_window(5,10)
P = ggplot(M, aes(cropGrowthStage, Value, colour=Variable, group=iteration)) +
  geom_line(alpha=0.3) +
  scale_x_continuous(breaks=10*(0:10)) +
  labs(y="") +
  theme(legend.position="none") +
  facet_wrap(~Variable, ncol=1, scales="free")
print(P)
