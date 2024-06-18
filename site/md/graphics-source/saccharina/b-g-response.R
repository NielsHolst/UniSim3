graphics.off()
source("~/QDev/UniSim3/input/scripts/begin.R")

M = data.frame(
  x = 0:100
)
D = 15
a = 0.9
M = mutate(M,
  y = D*(1 - exp(-a*x/D))
)

windows(4,3)
ggplot(M, aes(x,y)) +
  annotate("segment", x=0, y=D, xend=100, yend=D, colour=blue, size=1) +
  annotate("segment", x=0, y=0, xend=10, yend=a*12, colour=blue, size=1) +
  geom_line(colour=red) +
  scale_x_continuous(expand=c(0, 0)) +
  scale_y_continuous(expand=c(0, 0), limits=c(0,1.2*D)) +
  theme_classic() +
  theme(
    axis.text  = element_blank(),
    axis.ticks = element_blank(),
    axis.title = element_blank()
  )
  