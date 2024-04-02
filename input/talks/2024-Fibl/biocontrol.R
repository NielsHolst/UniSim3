# biocontrol.R
n = max(sim$iteration)

if (n<100) {
  M = melt(sim, id.vars=c("iteration", "date"), measure.vars=2:10, variable.name="Variable", value.name="Value")
  alpha = if (length(unique(sim$iteration))==1) 1 else 0.3

  open_plot_window(6.5,6.5)
  P = ggplot(M, aes(date, Value, colour=Variable, group=iteration)) +
    geom_line(alpha=alpha) +
    labs(y="") +
    theme(legend.position="none") +
    facet_wrap(~Variable, ncol=3, scales="free")
} else {
  M = melt(subset(sim, step==max(sim$step)), id.vars=c("iteration"), measure.vars=2:10, variable.name="Variable", value.name="Value")
  P = ggplot(M, aes(Value, colour=Variable, fill=Variable)) +
    geom_histogram(alpha=0.4, bins=n/10) +
    labs(x="") +
    theme(legend.position="none") +
    facet_wrap(~Variable, ncol=3, scales="free")
}
print(P)
