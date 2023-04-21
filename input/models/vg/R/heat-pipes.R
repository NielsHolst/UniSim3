pipe_density  = 2   # m/m2
pipe_diameter = 30  # mm
area          = 10000 # m2
pipe_length   = pipe_density*area # m
pipe_volume   = pi/4*(pipe_diameter/1000)^2*pipe_length # m3
pipe_volume

transit_time  = function(flow_rate) {
  # flow_rate    : m3/h
  # transit_time : min
  pipe_volume/flow_rate*60
}
transit_time(20)

k     = 0.0063
b     = 1.25
Tair  = 20

M = expand.grid(
  FlowRate = c(10, 20),
  T0       = 30:80
)
M = mutate(M,
  TransitTime = transit_time(FlowRate),
  DT_pipe = (k*(b-1)*TransitTime + (T0 - Tair)^(1-b))^(1/(1-b)),
  E_pipe  = 4184*DT_pipe*pipe_volume/area/TransitTime*1000/60
)

4184*30*pipe_volume/area/transit_time(20)*1000/60


pipe_volume/transit_time(20)

windows(5,6)
ggarrange(
  ggplot(M, aes(T0, DT_pipe, colour=factor(FlowRate))) +
    scale_colour_manual(values=c(blue, red)) +
    guides(color = guide_legend(reverse = TRUE)) + 
    ylim(0, NA) +
    labs(x=NULL,
         y="Drop in pipe temperature (oC)",
         colour="Flow rate\n(m3/h)") +
    geom_line(linewidth=1) +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=8))
    )

  ,
  ggplot(M, aes(T0, E_pipe, colour=factor(FlowRate))) +
    scale_colour_manual(values=c(blue, red)) +
    guides(color = guide_legend(reverse = TRUE)) + 
    ylim(0, NA) +
    labs(x="Pipe inlet temperature (oC)",
         y="Energy emitted (W/m2)",
         colour="Flow rate\n(m3/h)") +
    geom_line(linewidth=1) +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=8))
    )
    ,
    ncol = 1,
    align = "hv",
    common.legend = TRUE,
    legend = "right"
)

