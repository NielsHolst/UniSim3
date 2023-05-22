library(ggplot2)
library(plyr)
library(reshape2)

y = function(x, y0, T, cn, C, v, V) {
  indoorsAh0 = y0
  indoorsAh1 = (-exp(x*(-(cn + v)))*(cn*C - y0*(cn + v) + T + v*V) + cn*C + T + v*V)/(cn + v)
  avgAh      = (indoorsAh0 + indoorsAh1)/2
  data.frame (
    Seconds       = x,
    IndoorsAh     = indoorsAh1,
    DeltaAH       = indoorsAh1 - indoorsAh0,
    Transpiration = T*x,
    Condensation  = -cn*(avgAh - C)*x,
    Ventilation   = -v*(avgAh-V)*x
  )
}

averageHeight     = 3.9383711467890774
transpiration     = 2.1861570864786665e-05/averageHeight
condensation      = 1.8322423371401192e-05/averageHeight
coverSah          = 0.00640180964538232
indoorsAh         = 0.013785533838367395
outdoorsAh        = 0.0084417480337549977
ventilation       = 0.00017187500000000004

# Check y0
y(0, indoorsAh, transpiration, condensation, coverSah, ventilation, outdoorsAh)

# Plot 12 hours
x = 12*3600*(0:30)/30
M = adply(x, 1, function(x) y(x, indoorsAh, transpiration, condensation, coverSah, ventilation, outdoorsAh))[,-1]
M$Hours = M$Seconds/3600
ggplot(M, aes(Hours, IndoorsAh)) +
  geom_line()

# Plot 5 minutes partions
x = 60*(0:30)
M = adply(x, 1, function(x) y(x, indoorsAh, transpiration, condensation, coverSah, ventilation, outdoorsAh))[,-1]
M$Error = with(M, Transpiration + Condensation + Ventilation - DeltaAH)
P = melt(M[,-2], id.vars="Seconds")

ggplot(P, aes(Seconds, value, colour=variable)) +
  geom_line(size=1) +
  theme_bw()
  
  