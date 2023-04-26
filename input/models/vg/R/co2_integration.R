rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(plyr)
library(reshape2)

# Equation for CO2 balance
# 
# dy/dx = a + b*(c-y)
# y: indoors CO2 concentration [g/m3]
# x: time [h]
# a: injection rate [g/m3/h]
# b: ventilation rate [s-h]
# c: outdoors CO2 concentration [g/m3]

# Solve for y using Wolfram Alpha
# integrate 1/(a + b*(c-y)) dy =>
# 	(a*(y-c) + b*log(b + a*(c - y)))/a^2
# integrate 1 dx =>
#	x
# solve (a*(-c + y) + b*log(b + a*(c - y))/a^2 = x for y =>
# 	y = (a^3*c + b*W(-(a^2*exp((a^2*x)/b - a^2))/b) + a^2*b)/a^3
#
# initial condition y=y0 at x=0
# 	y0 = (a^3*c + a^2*b)/a^3 =>
#	y0 = c + b/a

y = function(x,y0,a,b,c) {
	if (b==0) (y0 + a*x) else exp(-b*x)*( (exp(b*x)-1)*(a/b+c)+y0 ) 
}

x = (0:30)/30

M = rbind(
		data.frame(x=x, Ventilation=4,	Init=600,
			Inj4=y(x, 600, -5, 4, 750),
			Inj20=y(x, 600, 20, 4, 750),
			Inj100=y(x, 600, 100, 4, 750),
			Inj500=y(x, 600, 500, 4, 750)
		),
		data.frame(x=x, Ventilation=8, Init=600,
			Inj4=y(x, 600, -5, 8, 750),
			Inj20=y(x, 600, 20, 8, 750),
			Inj100=y(x, 600, 100, 8, 750),
			Inj500=y(x, 600, 500, 8, 750)
		),
		data.frame(x=x, Ventilation=16, Init=600,
			Inj4=y(x, 600, -5, 16, 750),
			Inj20=y(x, 600, 20, 16, 750),
			Inj100=y(x, 600, 100, 16, 750),
			Inj500=y(x, 600, 500, 16, 750)
		),
			data.frame(x=x, Ventilation=4,	Init=900,
			Inj4=y(x, 900, -5, 4, 750),
			Inj20=y(x, 900, 20, 4, 750),
			Inj100=y(x, 900, 100, 4, 750),
			Inj500=y(x, 900, 500, 4, 750)
		),
		data.frame(x=x, Ventilation=8, Init=900,
			Inj4=y(x, 900, -5, 8, 750),
			Inj20=y(x, 900, 20, 8, 750),
			Inj100=y(x, 900, 100, 8, 750),
			Inj500=y(x, 900, 500, 8, 750)
		),
		data.frame(x=x, Ventilation=16, Init=900,
			Inj4=y(x, 900, -5, 16, 750),
			Inj20=y(x, 900, 20, 16, 750),
			Inj100=y(x, 900, 100, 16, 750),
			Inj500=y(x, 900, 500, 16, 750)
		)
)
	
ggplot(data=melt(M, id.vars=c("x","Ventilation", "Init")), aes(x=60*x, y=value, colour=variable)) +
	geom_hline(yintercept=750, size=1.5, colour="darkgrey")  +
	geom_line(size=1.5) +
	xlab("Time (minutes)") +
	ylab("Indoors CO2 concentration (ppm)\n") + 
	facet_grid(Init~Ventilation)
	

	