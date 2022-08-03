sim = read_output(output_file_name)
sim = sim[,-1]
s = colnames(sim)
colnames(sim) = substr(s, 1, nchar(s)-4)
colnames(sim) = c(
"Generations", 
"Kmax", "coverage", "ImmMean", "ImmResistanceFreq", "MaxInsecticideMort", 
"ResponseShape", "FitnessGain", "ReproductiveRate", "FitnessPenalty")
M = melt(sim, id.vars="Generations")

ggplot(subset(M, Generations<10000), aes(x=value, y=log10(Generations), colour=variable)) +
  geom_point(shape=21) +
  geom_smooth(size=2, se=FALSE) +
  labs(x="Parameter value", y="Number of generations; log10(G)") +
  theme(legend.position="none") +
  facet_wrap(~variable, ncol=3, scales="free")

windows()
  
ggplot(subset(M, Generations==10000), aes(x=value, colour=variable, fill=variable)) +
  geom_density(alpha=0.4) +
  theme(legend.position="none") +
  labs(x="Parameter value", y="Proportion of runs > 10,000 generations") +
  theme(
    axis.text.y = element_blank(),
    axis.ticks.y = element_blank()
  ) +
  facet_wrap(~variable, ncol=3, scales="free")
  
# combi = function(var, xlim) {
  # V = subset(M, variable==var)
  # grid.arrange(
  # ggplot(subset(V, Generations<10000), aes(x=value, y=log10(Generations), colour=variable)) +
    # geom_point(shape=21) +
    # geom_smooth(size=2, se=FALSE) +
    # xlim(xlim) +
    # theme(legend.position="none") +
    # facet_wrap(~variable, ncol=3, scales="free"),
  # ggplot(subset(V, Generations==10000), aes(x=value, colour=variable, fill=variable)) +
    # geom_density(alpha=0.4) +
    # theme(legend.position="none") +
    # xlim(xlim) +
    # facet_wrap(~variable, ncol=3, scales="free"),
    # ncol=1
  # )
# }
# combi("coverage.maxValue", c(0.1,0.9))

