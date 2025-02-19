# crop-aphid-model-ua.R
graphics.off()
densities = colnames(sim)[6:9]

M = melt(sim, id.vars=c("iteration", "date"), measure.vars=densities, variable.name="Variable", value.name="Value")
graphics.off()
open_plot_window(12,7)
P = ggplot(M, aes(date, Value, colour=Variable, group=iteration)) +
  geom_line(alpha=0.3, linewidth=2) +
  labs(x="", y="") +
  theme(legend.position="none") +
  facet_wrap(~Variable, ncol=2, scales="free")
print(P + ggplot_theme(24))
msgWindow("maximize")


