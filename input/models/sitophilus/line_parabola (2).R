rm(list=ls(all=TRUE))
source("~/Rdev/setup.R")

update_geom_defaults("line", list(linewidth=1))
update_geom_defaults("vline", list(linewidth=1))
update_geom_defaults("hline", list(linewidth=1))

alpha = 0.1
beta = -10
x1 = 12
x2 = 25

k = sqrt((beta + 2*alpha*x1 - alpha*x2)*(beta + alpha*x2))
(beta + 2*alpha*x1 - alpha*x2)
(beta + alpha*x2)
x0 = min( c((-beta + k)/alpha, (-beta - k)/alpha) )
a  = -(beta - k) / (2*(x1-x2)^2)
b  = x1*(beta - k + alpha*x1) / (x1-x2)^2
c_ = (2*x1 - x2)*x2*(beta - k + alpha*x1) / (2*(x1-x2)^2)
D = b^2 - 4*a*c_
list(
  x0=x0, a=a, b=b, c_=c_, D=D
)
(-b +sqrt(D))/2/a


f = function(x) {
  alpha*x + beta
}

g = function(x) {
  a*x^2 + b*x + c_
}

x = (0:400)/10
x = 0:40
M = data.frame(
  x = x,
  fx = f(x),
  gx = g(x)
)

windows(7,4)
ggplot(M, aes(x, fx)) +
  geom_vline(xintercept=c(x0, x1, x2), colour=brown) +
  geom_line(colour=blue) +
  geom_line(colour=red, aes(y=gx))
  
  # geom_vline(xintercept=c(min(M$x), max(M$x)), colour=orange) +
  # geom_hline(yintercept=fg(x1), colour=violet) +
  scale_x_continuous(breaks = 0:50)  

