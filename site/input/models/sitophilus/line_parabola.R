rm(list=ls(all=TRUE))
library("reshape2")
source("~/Rdev/setup.R")


# x1 = 15
# x2 = 25
# alpha = 0.4
# beta = 1

x1 = 30
x2 = 37
alpha = 0.00199
beta = -0.0239


A = sqrt((beta + 2*alpha*x1 - alpha*x2)*(beta + alpha*x2))
B = x1^2 - 2*x1*x2 + x2^2
C = sqrt(beta^2 + 2*alpha*beta*x1 + 2*alpha^2*x1*x2 - alpha^2*x2^2)
pm = c(-1,1)

a  = (-beta - alpha*x1 - pm*A) / (2*B)
b  = (beta*x1 + alpha*x1^2 + pm*x1*A) / B
c_ = (-2*beta*x1*x2 - 2*alpha*x1^2*x2 + beta*x2^2 + alpha*x1*x2^2 - 2*x1*x2*pm*A + x2^2*pm*A) / (2*B)
x0 = (-beta + pm*C)/alpha
list(a,b,c_,x0)

# list(
  # a  = c(-0.00627719, -0.0637228),
  # b  = c(0.188316, 1.91168),
  # c_ = c(-0.784648, -7.96535), 
  # x0 = c(-16.8614, 11.8614)
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

# Pick parabola with minimum distance from culmination to line
pick = which.min(c(f(x1) - g1(x1), f(x1) - g2(x1)))
pick = paste0("g", pick, "x")

x = seq(10, 40, by=0.01)
M = data.frame(
  x = x,
  fx = f(x),
  g1x = g1(x),
  g2x = g2(x)
)

M = melt(M, id.vars="x")
ggplot(M, aes(x, value, colour=variable)) +
  geom_vline(xintercept=c(x1,x2)) +
  geom_vline(xintercept=x0, colour=brown) +
  geom_hline(yintercept=0) +
  geom_line(linewidth=1) +
  xlim(10, NA) +
  ylim(0, NA) +
  labs(x="", y="", subtitle=paste("Use parabola culminating closest to line: ", pick))

# Pick parabola that culminates closest to the line
