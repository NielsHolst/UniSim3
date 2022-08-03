rm(list=ls(all=TRUE) )

library(plyr)

setwd("C:/Users/au152367/documents/QDev/UniSim2/output")

# Inputs
p = read.table("food_web_parameters.txt", TRUE, "\t")
m = nrow(p)/3 - 1
n = ncol(p)-3
y_col = 3:(ncol(p)-1)
Y  = matrix(unlist(p[1, y_col]), nrow=1, ncol=n)
D  = matrix(unlist(p[2, y_col]), nrow=1, ncol=n)
a  = as.matrix(p[(3+0*m):(2+1*m) , y_col])
g  = as.matrix(p[(3+1*m):(2+2*m), y_col])
X  = matrix(p[(3+2*m):(2+3*m), ncol(p)])
Dt = unlist(p[(3+3*m), ncol(p)])
d  = D/Y*Dt

# Outputs
sij  = Sij = mDXIj = matrix(rep(NA,m*n), nrow=m, ncol=n)
mXi = matrix(rep(NA,m), nrow=m, ncol=1)compute_sij = function() {
  si_  = matrix(rep(NA,m), nrow=m, ncol=1)
  for (i in 1:m) {
    aYDt = a[i,]*Y*Dt
    sum_aYDt = sum(aYDt)
    si_ = 1- exp(-sum_aYDt)
    sij[i,] <<- aYDt/sum_aYDt*si_
  }
  sij
}

compute_Sij = function() {
  S_j  = matrix(rep(NA,n), nrow=1, ncol=n)
  for (j in 1:n) {
    gsX = g[,j]*sij[,j]*c(X)
    sum_gsX = sum(gsX)
    S_j = if (D[j]>0) D[j]*(1 - exp(-sum_gsX/D[j])) else 0
    Sij[,j] <<- gsX/sum_gsX*S_j
  }
  Sij
}

compute_mDXij = function() {
  mDXij <<- Sij/g
  mDXij
}

compute_mXi = function() {
  for (i in 1:m) {
    mXi[i] <<- sum(mDXij[i,])/X[i]
  }
  mXi
}

compute_sij()
compute_Sij()
compute_mDXij()
compute_mXi()
