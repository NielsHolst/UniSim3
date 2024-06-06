source("C:/MyDocuments/QDev/UniSim3/input/scripts/begin.R")

eq7 = function(T, a) {
  if ( a<=4 ) {
    0
  } else {
    a_opt = 34.92
    f1 = 0.005494
    f2 = 0.2359
    f3 = 26.67
    k = (a - a_opt)/f3
    f1*exp(f2*T)*exp(-exp(-k) - k + 1)
  }
}

p = expand.grid(
  T = c(20, 25, 27, 30),
  a = 0:150
)
M = adply(p, 1, function(x) data.frame(fecundity = eq7(x$T, x$a)))
M = ddply(M, .(T), mutate, Ra = cumsum(fecundity))
head(M)

R0 = ddply(M, .(T), summarize, R0 = sum(fecundity))
R0
M = merge(M, R0, by="T", type="left") 
M$RaProp = M$Ra/M$R0

windows(7,10)
ggarrange(
  ggplot(M, aes(a, fecundity, colour=factor(T))) +
    geom_line() +
    guides(colour=guide_legend(reverse=TRUE)) +
    labs(
      x = "Age (days)",
      y = "Daily fecundity",
      colour = "Temperature",
      title = "Figure 8"
    )
  ,
  ggplot(M, aes(a, Ra, colour=factor(T))) +
    geom_line() +
    guides(colour=guide_legend(reverse=TRUE)) +
    labs(
      x = "Age (days)",
      y = "Accumulated fecundity",
      colour = "Temperature",
      title = "Figure 8, accumulated"
    )
  ,
  ggplot(M, aes(a, RaProp, colour=factor(T))) +
    geom_line() +
    guides(colour=guide_legend(reverse=TRUE)) +
    labs(
      x = "Age (days)",
      y = "Accumulated fecundity proportion",
      colour = "Temperature",
      title = "Figure 8, accumulated proportion"
    )
  ,
  ncol = 1
)

subset(M, T==30 & a > 40 & a < 50)


model = lm(R0 ~ T + I(T^2), R0)
model

x = 20:30
Fit = data.frame(
  T = x,
  R0 = predict(model, data.frame(T=x))
)

windows(7,4)
ggplot(R0, aes(T, R0)) +
  geom_line(colour=blue, data=Fit) +
  geom_point(colour=red, size=2)
  
