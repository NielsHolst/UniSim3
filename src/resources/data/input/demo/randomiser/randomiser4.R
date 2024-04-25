# randomiser4.R
M = melt(sim, id.vars=c("iteration", "date"), measure.vars=2:5, variable.name="LifeStage", value.name="PopulationSize")
P = ggplot(M, aes(date, PopulationSize, colour=LifeStage, group=iteration)) +
  geom_line(alpha=0.3) +
  labs(x="", y="") +
  theme(legend.position="none") +
  facet_wrap(~LifeStage, scales="free")
print(P)

# Hook for batch command
figures = function(df) {
  Pages = list(Page = list(Grob=P, Width=7.0, Height=4.0))
}

