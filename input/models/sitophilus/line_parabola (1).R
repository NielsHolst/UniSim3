rm(list=ls(all=TRUE))
source("~/Rdev/setup.R")

update_geom_defaults("line", list(linewidth=1))
update_geom_defaults("vline", list(linewidth=1))
update_geom_defaults("hline", list(linewidth=1))


f = function(x) {
  alpha*x + beta
}

g = function(x) {
  a*x^2 + b*x + c_
}

fg = function(x) {
  # Split x range
  y = c(x[x<x0] |> f(), x[x>=x0] |> g())
  # Restrict to positive y
  i = y>0
  x = x[i] 
  y = y[i] 
  # Right root
  x2 = max( (-b + c(-1,1)*sqrt(b^2 - 4*a*c_))/(2*a)  )
  data.frame(
    x = c(-beta/alpha, x, x2),
    y = c(0, y, 0)
  )
}

alpha = 2
beta = -10
x0 = 10
x1 = 10.5

a = alpha/2/(x0-x1)
b = alpha*(1 - x0/(x0-x1))
c_ = -a*x0^2 + (alpha-b)*x0 + beta

windows(7,4)
M = fg((0:400)/10)
ggplot(M, aes(x, y)) +
  # geom_vline(xintercept=c(x0, x1), colour=brown) +
  # geom_vline(xintercept=c(min(M$x), max(M$x)), colour=orange) +
  # geom_hline(yintercept=fg(x1), colour=violet) +
  geom_line(colour=blue) +
  scale_x_continuous(breaks = 0:50)  

