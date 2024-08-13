M = melt(subset(sim, totalPar>0), id.vars=c("Season", "dateTime"), measure.vars=c("temperature", "rh", "totalPar"), variable.name="Variable", value.name="Value")
open_plot_window(8,3)
P1 = ggplot(M, aes(Value)) +
  geom_density(aes(colour=Season, fill=Season), alpha=0.3) +
  labs(title="Model verification 1", x="", y="") +
  facet_wrap(~Variable, scales="free") +
  theme(
    axis.text.y = element_blank(),
    axis.ticks  = element_blank(),
    panel.grid  = element_blank()
  )
print(P1)
  
  