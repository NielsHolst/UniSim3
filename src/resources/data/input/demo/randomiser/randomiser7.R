# randomiser7.R
bins = 20
ggarrange(
  ggplot(sim, aes(monteCarlo.x,  monteCarlo.y)) +
    geom_point(colour=red)
  ,
  ggplot(sim, aes(stratified.x,  stratified.y)) +
    geom_point(colour=blue)
  ,
  ggplot(sim, aes(sobolSequence.x,  sobolSequence.y)) +
    geom_point(colour=green)
  ,
  ggplot(sim, aes(monteCarlo.x)) +
    geom_histogram(colour=red, fill=red, alpha=0.3, bins=bins)
  ,
  ggplot(sim, aes(stratified.x)) +
    geom_histogram(colour=blue, fill=blue, alpha=0.3, bins=bins)
  ,
  ggplot(sim, aes(sobolSequence.x)) +
    geom_histogram(colour=green, fill=green, alpha=0.3, bins=bins)
  ,
  nrow=2, ncol=3
)


