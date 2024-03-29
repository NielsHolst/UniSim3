101325/8.314/293.15*44*1e-6*39384*500/10000
101325/8.314/293.15*44*1e-6*39384*500/39384

1.29*3*3600/1000
1.29*3*3600/1000*10000/39384


y = function(x,y0,k,v,z) {
  exp(-v*x)*( y0 + (k/v + z)*(exp(v*x)-1) ) 
}

Agh = 10e3

M = expand.grid (
  x = 0:12,
  k = c(0, 0.5),
  v = c(0.5, 1)
)
y0 = 500*1.829e-3
z  = 400*1.829e-3

M$CO2_abs = with(M, y(x, y0, k, v, z))
M$CO2 = M$CO2_abs/1.829e-3

windows(5,3)
ggplot(M, aes(x, M$CO2, colour=factor(k))) +
  geom_line(aes(linetype=factor(v))) +
  geom_point(size=2) +
  scale_x_continuous(breaks=0:12) +
  scale_y_continuous(breaks=100*(0:20)) +
  labs(x="Hours", y="Indoors CO2 (ppm)", colour="CO2 net rate", linetype="Ventilation rate") +
  theme(
    axis.title.x = element_text(margin = margin(t=8)),
    axis.title.y = element_text(margin = margin(r=9))
  )

# Case
indoors_ppm  = 4782
indoorsCo2  = 1.829e-3*indoors_ppm
outdoorsCo2 = 1.829e-3*400
co2Injection = 0
averageHeight = 3.938
injection   = co2Injection/averageHeight/3600
Pn = -0.2626
fixation    = Pn*44.01e-6/averageHeight
c           = injection - fixation
ventilationRate = 0.8
v           = ventilationRate/3600

indoorsCo2
new_indoorsCo2 = y(90, indoorsCo2, c, v, outdoorsCo2)
new_indoorsCo2
new_indoors_ppm = new_indoorsCo2/1.829e-3
new_indoors_ppm
