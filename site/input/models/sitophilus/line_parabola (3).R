rm(list=ls(all=TRUE))
library("reshape2")
source("~/Rdev/setup.R")


x1 = 15
x2 = 25

p = function(x, c_) {
  A = -c_*(x - x2)*(x - 2*x1 + x2)
  B = x2*(-2*x1 + x2)
  A/B
}

alpha = 0.6
beta = -3

f = function(x) {
  alpha*x + beta
}

x = 0:40
M = data.frame(
  x = x,
  p1 = p(x, -1),
  p2 = p(x, -2),
  p3 = p(x, -3),
  L = f(x)
)
M = melt(M, id.vars="x")
ggplot(M, aes(x, value, colour=variable)) +
  geom_vline(xintercept=c(x1,x2)) +
  geom_hline(yintercept=0) +
  geom_line(linewidth=1) 

x0 = c(5., -120.)
c_ = c(0.714286, -0.517241)
b  = c(-0.171429, 0.124138)
a  = c(0.00571429, -0.00413793)


# a   =  -b/(2*x1)

# b   = -(2*c_*x1) / ((2*x1 - x_2)*x_2)

# x0 = 

# (  sqrt(x2^2*(x2 - 2*x1)^2*(4*beta^2 + alpha^2*x2^4 - 4*alpha^2*x2^2 + 4*alpha^2*x1^2*x2^2 + 4*alpha*x1*(2*beta - alpha*x2^3 + 2*alpha*x2))) + 
   # x1*(4*alpha*x2^3 - 4*beta*x2) -
   # alpha*x2^4 - 
   # 4*alpha*x1^2*x2^2 +
   # 2*beta*x2^2
# )/
# (
  # 2*alpha*(2*x1 - x2)*x2
# )



f = function(x) {
  alpha*x + beta
}

g1 = function(x) {
  a[1]*x^2 + b[1]*x + c_[1]
}


g2 = function(x) {
  a[2]*x^2 + b[2]*x + c_[2]
}


x = -120:40
M = data.frame(
  x = x,
  fx = f(x),
  g1x = g1(x),
  g2x = g2(x)
)

M = melt(M, id.vars="x")
windows()
ggplot(M, aes(x, value, colour=variable)) +
  geom_line() 
