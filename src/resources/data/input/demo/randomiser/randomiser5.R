# randomiser5.R
M = data.frame(
  Larva = dmy("30/04/2008") + sim$larva.at,
  Pupa  = dmy("30/04/2008") + sim$pupa.at,
  Adult = dmy("30/04/2008") + sim$adult.at
)
M = melt(M, variable.name="LifeStage", value.name="Date")
P = ggplot(M, aes(Date, colour=LifeStage, fill=LifeStage))  +
  geom_density(alpha=0.3) +
  scale_colour_manual(values=c(blue, green, violet)) +
  scale_fill_manual(  values=c(blue, green, violet)) +
  labs(x = "Date of maximum occurence", y="Relative density")
print(P)