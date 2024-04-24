sim = read_output(output_file_name)

M = sim[c("iteration", "date", "mussel.biomass", "mussel.abundance", "starfish.biomass", "starfish.abundance")]
M = melt(M, id.vars=c("iteration", "date"), variable.name="Variable", value.name="Value")

G = ggplot(M, aes(x=date, y=Value)) +
  geom_line(aes(group=iteration, colour=Variable), size=1.1, alpha=0.3) +
  labs(x="Date", title="Model uncertainty\n") + 
  facet_wrap(~Variable, scales="free_y") +
  theme_classic() +
  theme(legend.position="none")
windows(8,8)
print(G)

G = ggplot(subset(M, date==max(M$date)), aes(x=Value)) +
  geom_density(aes(fill=Variable, colour=Variable), alpha=0.3) +
  labs(title="Distribution of final densities\n") + 
  facet_wrap(~Variable, scales="free") +
  theme_classic() +
  theme(legend.position="none") +
  theme(
    axis.title.y = element_blank(),
    axis.ticks.y = element_blank(),
    axis.text.y  = element_blank()
  )
windows(8,8)
print(G)

G = ggplot(subset(sim, date==max(sim$date)), aes(x=mussel.biomass, y=starfish.biomass)) +
  labs(title="Final densities\n") + 
  geom_point(size=3, colour="forestgreen", alpha=0.5) +
  theme_classic() 
windows(6,6)
print(G)

G = ggplot(sim, aes(x=mussel.biomass, y=starfish.biomass)) +
  geom_point(aes(group=iteration), size=1.1, alpha=0.3, colour="brown") +
  geom_path(aes(group=iteration), size=1.1, alpha=0.3, colour="forestgreen") +
  theme_classic() 
windows(8,8)
print(G)


