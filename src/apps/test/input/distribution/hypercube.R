rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(gridExtra)

n = 10	# number of strata
x_values = 0:30
min_frac = 0.01
max_frac = 0.99

plot_strata = function(FUN, INV) {
	dy = 1/n
	y = c(min_frac, (1:(n-1))*dy, max_frac)
	L1 = data.frame(
		x=rep(0,n+1),
		y=y,
		xend=INV(y),
		yend=y
	)
	L2 = data.frame(
		x=INV(y),
		y=y,
		xend=INV(y),
		yend=0
	)
	L = rbind(L1,L2)
	print(L)
	
	ggplot(data.frame(x=x_values, y=FUN(x_values)), aes(x=x,y=y)) +
		geom_segment(data=L, aes(x=x,y=y,xend=xend,yend=yend), colour="red", linetype=2) +
		geom_point(data=L[1:(n+1),], aes(x=x,y=y), colour="red") +
		geom_point(data=L[1:(n+1),], aes(x=xend,y=x), colour="red") +
		geom_line(size=2, colour="blue") +
		ylim(0,1)
		
}

even = function(x) {
	x/max(x_values)
}

inv_even = function(y) {
	y*max(x_values)
}

normal = function(x) {
	pnorm(x_values, 15, 5)
}

inv_normal = function(y) {
	qnorm(y, 15, 5)
}

#windows(10,6)   #Windows only
grid.arrange(
	ncol=2
	,
	plot_strata(even, inv_even)
	,
	plot_strata(normal, inv_normal)
)

