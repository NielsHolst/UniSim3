source("~/Rdev/setup.R")

my_plot = function(M) {
  ggplot(M, aes(x, fx)) +
    geom_line(colour=blue, linewidth=1) +
    geom_point(colour=red, size=2)
}

g = function(x, slope, x0, x1) {
  a = slope/2/(x0-x1)
  b = slope*(1 - x0/(x0-x1))
  2*a*x + b
}

g(10, 1, 10, 15)
g(15, 1, 10, 15)


fc = function(x, slope, x0, x1, c) {
  a = slope/2/(x0-x1)
  b = slope*(1 - x0/(x0-x1))
  a*x^2 + b*x + c
}

x  = 0:40
x0 = 10
x1 = 15
c_ = 20
slope = 2
M = data.frame(
  x = x,
  fx = fc(x, slope, x0, x1, c_)
)

windows(7,4)
my_plot(M) + 
  scale_x_continuous(breaks = 5*(0:10)) +
  ylim(-5, 80) +
  geom_hline(yintercept=c(0, fc(x1, slope, x0, x1, c_))) + 
  geom_vline(xintercept=c(x0, x1)) +
  geom_abline(slope=2, intercept= fc(x0, slope, x0, x1, c_) - slope*x0, colour=violet)
    

x = 33
x2 = 33

f = function(x, slope, x0, x1, x2) {
  a = slope/2/(x0-x1)
  b = slope*(1 - x0/(x0-x1))
  c_ = -a*x2^2 -b*x2
  c_
  a*x^2 + b*x + c_
}

x  = 0:40
x0 = 10
x1 = 15
x2 = 25
slope = 2
M = data.frame(
  x = x,
  fx = f(x, slope, x0, x1, x2)
)

windows(7,4)
my_plot(M) + 
  scale_x_continuous(breaks = 5*(0:10)) +
  ylim(-5, 80) +
  geom_hline(yintercept=c(0, f(x1, slope, x0, x1, x2))) + 
  geom_vline(xintercept=c(x0, x1, x2)) +
  geom_abline(slope=2, intercept= f(x0, slope, x0, x1, x2) - slope*x0, colour=violet)
