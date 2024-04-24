graphics.off()

M = data.frame(
  Time  = 0:6,
  Value = 0:6
)

Points = data.frame(
  Time  = c(1,2,5),
  Value = c(1,2,5)
)

Segments = data.frame(
  Time = c(1,2,5),
  Xend = c(1,2,5),
  Value = c(-1,-1,-1),
  Yend = c(1,2,5)
)

windows(4,3)
ggplot(M, aes(Time, Value)) +
  geom_segment(aes(xend=Xend, yend=Yend), data=Segments) + 
  geom_line(size=1, colour=red) +
  geom_point(size=3, colour=blue, data=Points) +
  scale_x_continuous(name="Time",         breaks = c(1,2,5), limits=c(-1,7), labels=c("t-1", "t", "t+n*dt")) +
  scale_y_continuous(name="Sensed value", breaks = c(1,2,5), limits=c(-1,7), expand=c(0,0)) +
  annotate("text", x=c(1.5, 3.5), y=0, label=c("dt", "n*dt")) +
  theme(
    panel.grid = element_blank(),
    axis.text.y = element_blank(),
    axis.ticks.y = element_blank(),
    axis.title.x = element_text(margin = margin(t=8)),
    axis.title.y = element_text(margin = margin(r=9))
  )
