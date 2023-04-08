rm(list=ls(all=TRUE))
graphics.off()

source("~/QDev/UniSim3/input/scripts/begin.R")

v = 3.0/60

y = function(yin0, yout, t) {
  yout - (yout-yin0)*exp(-v*t)
}

x = 0:60

M = expand.grid(
  Time = x,
  Yin0 = c(15,25)
)
M$Yin = with(M, y(Yin0, 20, Time))
M$Yin0 = factor(M$Yin0)

windows(4,3)
ggplot(M, aes(Time, Yin, colour=Yin0)) +
  geom_line(size=1) +
  geom_hline(yintercept=20, size=1) +
  scale_x_continuous(breaks=15*(0:4)) +
  labs(x="Minutes", y="Indoors temperature") +
  theme_bw() +
  theme(
    legend.position="none",
    axis.title.y = element_text(margin = margin(t = 0, r = 10, b = 0, l = 0)))
  )

DT = y(c(15,25), 20, 2) - c(15,25)
DT
Cair = 1020*1.19
Cair
DT*Cair/120*3.938


