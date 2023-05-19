y = function(x,y0,k,v,z) {
  exp(-v*x)*( y0 + (k/v + z)*(exp(v*x)-1) ) 
}

#https://www.omnicalculator.com/physics/absolute-humidity
# Indoors 95% RH, 22C
y0 = 18.45e-3 #kg/m3

# Outdoors 40% RH, 12C
z =  4.264e-3 #kg/m3

# Transpiration
k = 0.0001875/3.8384
k
 
# Ventilation rate
v = 20/3600 # /s

M = expand.grid (
  x = 60*(0:10),
  v = c(20, 40)
)

M$y = with(M, y(x, y0, k, v/3600, z))

windows(5,3)
ggplot(M, aes(x, M$y, colour=factor(v))) +
  geom_line() +
  geom_point(size=2) +
  labs(x="Time (s)", y="Indoors H2O (kg/m3)", colour="Ventilation rate") +
  theme(
    axis.title.x = element_text(margin = margin(t=8)),
    axis.title.y = element_text(margin = margin(r=9))
  )
  
ahFromVp = function(temperature, vp) {
  Mwater = 18.016e-3
  R = 8.314
  T0 = 273.15
  vp*Mwater/(R*(temperature + T0))
}

svp = function(temperature) {
  610.8*exp(17.27*temperature/(temperature+237.3))
}

sah = function(temperature) {
    ahFromVp(temperature, svp(temperature))
}

rhFromAh = function(temperature, ah) {
    pmin(100*ah/sah(temperature), 100.)
}

M$RH = rhFromAh(22, M$y)

windows(5,3)
ggplot(M, aes(x, M$RH, colour=factor(v))) +
  geom_line() +
  geom_point(size=2) +
  labs(x="Time (s)", y="Indoors H2O (%)", colour="Ventilation rate") +
  theme(
    axis.title.x = element_text(margin = margin(t=8)),
    axis.title.y = element_text(margin = margin(r=9))
  )
  
