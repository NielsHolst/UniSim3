M = sim[c("maxPrevalence", "maxCadaverPrevalence","yieldImprovement")]
M = melt(M, measure.vars=1:3, variable.name="Parameter", value.name="Value")
ggplot(M, aes(Value, colour=Parameter, fill=Parameter)) +
  geom_histogram(alpha=0.5) +
  theme(legend.position='none') +
  facet_wrap(~Parameter, scales="free")
  