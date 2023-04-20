k = 0.05
b = 1.25
Tin = 20
volume = 7.5
flow_rate = c(10, 20)
Agh = 10000

dTpipe = function(Tinlet, flow_rate) {
  dtpipe = volume/flow_rate
  print(dtpipe)
  data.frame(
    DTpipe = (Tinlet - Tin) - (k*(b-1)*dtpipe + (Tinlet - Tin)^(1-b))^(1/(1-b))
  )
}

E = function(dT, dt) {
  4184*dT*volume/Agh/dt*1000/3600
}

dTpipe(60, flow_rate)

E(10,0.4)

M = expand.grid(
  Tinlet   = 30:80,
  FlowRate = c(5,10)
)
M = ddply(M, .(Tinlet, FlowRate), function(x) dTpipe(x$Tinlet, x$FlowRate))

ggplot(M, aes(Tinlet, DTpipe, colour=factor(FlowRate))) +
  geom_line() +
  labs(
    x = "Pipe inlet temperature (oC)",
    y = "Drop in pipe temperature (oC)",
    colour = "Flow rate\n(m3/h)"
  ) +
  theme(
    axis.title.x = element_text(margin = margin(t=8)),
    axis.title.y = element_text(margin = margin(r=9))
  )

ggplot(M, aes(Tinlet, DTpipe, colour=factor(FlowRate))) +
  geom_line() +
  labs(
    x = "Pipe inlet temperature (oC)",
    y = "Energy emitted (W/m2)",
    colour = "Flow rate\n(m3/h)"
  ) +
  theme(
    axis.title.x = element_text(margin = margin(t=8)),
    axis.title.y = element_text(margin = margin(r=9))
  )
