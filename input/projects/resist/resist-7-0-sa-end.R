library(field)
library(ggplot2)
library(gridExtra)
library(reshape2)
library(stringr)

setwd("C:/Users/au152367/Documents/QDev/UniSim2/input/projects/resist")
# save(sim, file="sim.Rdata")
load("sim.Rdata")

M = sim[,-1]
k = length(colnames(M)) - 2
N = max(sim$iteration)/(k+2)
colnames(M) = str_replace(colnames(M), "\\.end", "")



col_outbreak = which(colnames(M) == "density.thresholdPassed")
col_resistance = which(colnames(M) == "frequencies.thresholdPassed")
inputs = -c(col_outbreak, col_resistance)
A = M[1:N, inputs]
B = M[(N+1):(2*N), inputs]
C = M[(2*N+1):((k+2)*N), inputs]
dim(A)
dim(B)
dim(C)


sobol = function(output) {
  yA = M[1:N, output]*1
  yB = M[(N+1):(2*N), output]*1

  yC = function(i) {
    from = (i+1)*N + 1
    to = from + N - 1
    M[from:to, output]*1
  }

  mult = function(x,y) {
    sum(x*y)/N
  } 

  Si = function(i) {
    m2 = mean(yA)^2
    (mult(yA, yC(i)) - m2) / (mult(yA,yA) - m2)
  }

  STi = function(i) {
    m2 = mean(yA)^2
    1 - (mult(yB, yC(i)) - m2) / (mult(yA,yA) - m2)
  }
  
  reorder_levels = function(the_factor, new_order) {
    factor(the_factor,levels(the_factor)[new_order])
  }

  si = adply(1:9, 1, Si)[,2]
  sti = adply(1:9, 1, STi)[,2]

  Sobol = data.frame(Parameter = colnames(M)[inputs], Si = si, STi = sti)
  colnames(Sobol) = c("Parameter", "FirstOrderEffect", "TotalEffect")

  ix = order(Sobol$TotalEffect)
  Sobol = Sobol[rev(ix),]

  Sobol = rbind(Sobol, c(NA, sum(Sobol[,2]), sum(Sobol[,3])))
  Sobol$Parameter = as.character(Sobol$Parameter)
  Sobol$Parameter[k+1] = "SUM"
  Sobol$Parameter = factor(Sobol$Parameter)
  Sobol[,2:3] = round(Sobol[,2:3], 3)
  
  Sobol
}

Sobol_resistance = sobol(col_resistance)
Sobol_outbreak = sobol(col_outbreak)
Sobol_resistance
Sobol_outbreak

# https://stackoverflow.com/questions/14423325/confidence-intervals-for-predictions-from-logistic-regression
# https://www.fromthebottomoftheheap.net/2017/05/01/glm-prediction-intervals-i/

plot_quantiles = function(x) {
  obs = data.frame(M[x], Resistance=M$frequencies.thresholdPassed*1)
  head(obs)
  qmodel_low  = qsreg(obs[,x], obs$Resistance, alpha=0.25)
  qmodel_mid  = qsreg(obs[,x], obs$Resistance, alpha=0.5)
  qmodel_high = qsreg(obs[,x], obs$Resistance, alpha=0.75)

  x_range = (100:900)/1000
  QP = data.frame(
    X=x_range, 
    Low = predict(qmodel_low, x_range),
    Resistance = predict(qmodel_mid, x_range),
    High = predict(qmodel_high, x_range)
  )
  colnames(QP)[1] = x

  ggplot(QP, aes_string(x=x)) +
    geom_ribbon(aes(ymin=Low, ymax=High),  fill = "steelblue2", alpha = 0.2) +
    geom_line(aes(y=Resistance)) +
    labs(y="Resistance risk")   
}

grid.arrange(
  plot_quantiles(as.character(Sobol_resistance$Parameter[1]))
  ,
  plot_quantiles(as.character(Sobol_resistance$Parameter[2]))
  ,
  plot_quantiles(as.character(Sobol_resistance$Parameter[3]))
  ,
  plot_quantiles(as.character(Sobol_resistance$Parameter[4])),
  ,
  ncol=2
)




  

