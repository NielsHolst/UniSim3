graphics.off()
summary(sim) |> print()

sim = sim |>
mutate(
  DamagePct = 100*seedsDamaged/seedsTotal,
  BeetleDensity = adult/size
)

{
  ggplot(sim, aes(step, adult)) +
    geom_line(aes(group=iteration), alpha=0.3, colour=green) +
    labs(x="Day")
} |> print()

windows()
{
  ggplot(sim, aes(step, outflowTotal)) +
    geom_line(aes(group=iteration), alpha=0.3, colour=red) +
    labs(x="Day", y="Adults accumulated")
} |> print()

windows()
{
  ggplot(sim, aes(step, numEggsLaid)) +
    geom_line(aes(group=iteration), alpha=0.3, colour=green) +
    labs(x="Day")
} |> print()

windows()
{
  ggplot(sim, aes(step, DamagePct)) +
    geom_line(aes(group=iteration), alpha=0.3, colour=red) +
    labs(x="Day")
} |> print()
