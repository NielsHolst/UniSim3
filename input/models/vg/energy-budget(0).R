# Prelude
rm(list=ls(all=TRUE))
library(plyr)

# Short-wave radiation budget
P = function(row_num, direction) {
  p = read.table(header=TRUE, sep=",", text=
   "Layer,a,r,t,a_,r_,t_,E,E_
    Sky,1,0,0,1,0,0,0, 0
    Glass,0.03,0.08,0.89,0.03,0.08,0.89,0,0
    Screen,0.1,0.6,0.3,0.1,0.6,0.3,0,0
    Light,0,0,1,0,0,1,0,0
    Plant,0.47,0.48,0.95,0.7,0.1,0.2,0,0
    Floor,0.6,0.4,0,1,0,0,0,0
    Soil,1,0,0,1,0,0,0,0"
  )
  col_num = if (direction=="down") 8 else 9
  p[row_num, col_num] = 100
  p
}

distribute_radiation = function(p) {
  budget = init_budget(p)
  iter = 0
  precision = 1e-4
  while (sum(c(budget$F, budget$F_)) > precision) {
    iter = iter + 1
    budget = distribute_radiation_down(p, budget)
    budget = distribute_radiation_up  (p, budget)
  }
  # print(paste("Energy budget resolves in", iter, "steps"))
  M = cbind(p, budget)
  M$Sum = sum(budget)
  M$Iter = iter
  M
}

init_budget = function(p) {
  budget = data.frame(
    F  = p$E,
    F_ = p$E_,
    A  = rep(0, nrow(p)),
    A_  = rep(0, nrow(p))
  )
  rownames(budget) = p$Layer
  budget
}

distribute_radiation_down = function(p, b) {
  n = nrow(p)
  for (i in 1:(n-1)) {
    # Correct absorption and transmission of this layer and layer below
    # for reflections ad infinitum between the two layers
    k   = 1 - p$r[i+1]*p$r_[i]
    ah  = p$a[i+1]/k
    th  = p$t[i+1]/k
    ah_ = p$a_[i] /k * p$r[i+1]
    th_ = p$t_[i] /k * p$r[i+1]

    # Absorbed by layer below
    absorbed    = ah*b$F[i]
    # Transmitted through layer below
    transmitted = th*b$F[i]

    # Absorbed by this layer by reflection from below
    absorbed_    = ah_*b$F[i]
    # Transmitted through this layer by reflection from below
    transmitted_ = th_*b$F[i]
    sum(absorbed, transmitted, absorbed_, transmitted_)

    # Update layer below
    b$A [i+1] = b$A[i+1] + absorbed
    b$F [i+1] = b$F[i+1] + transmitted

    # Update this layer
    b$A_[i]   = b$A_[i] + absorbed_
    b$F_[i]   = b$F_[i] + transmitted_
    sum(b[,2:4]) + b$F[i+1]
  }
  b$F = 0
  b
}

distribute_radiation_up = function(p, b) {
  n = nrow(p)
  for (i in n:2) {
    # Correct absorption and transmission of this layer and layer above
    # for reflections ad infinitum between the two layers
    k   = 1 - p$r_[i-1]*p$r[i]
    ah_ = p$a_[i-1]/k
    th_ = p$t_[i-1]/k
    ah  = p$a[i] /k * p$r_[i-1]
    th  = p$t[i] /k * p$r_[i-1]
    # f = c(ah_, th_, ah ,th)
    # print(f)
    # print(paste("Check unity =", sum(f)))

    # Absorbed by layer above
    absorbed_    = ah_*b$F_[i];
    # Transmitted through layer above
    transmitted_ = th_*b$F_[i];
    # Absorbed by this layer by reflection from above
    absorbed     = ah*b$F_[i]
    # Transmitted through this layer by reflection from above
    transmitted  = th*b$F_[i]
    # print(rownames(b)[i])
    # print(c(b$F_[i], absorbed_, transmitted_, absorbed, transmitted))

    # Update layer above
    b$A_[i-1] = b$A_[i-1] + absorbed_
    b$F_[i-1] = b$F_[i-1] + transmitted_

    # Update this layer
    b$A [i]   = b$A [i] + absorbed
    b$F [i]   = b$F [i] + transmitted

  }
  b$F_ = 0
  b
}

test_func = function(func, p) {
  result = func(p, init_budget(p))
  result$Sum = sum(result)
  result
}

# test_func(distribute_radiation_up, P(3, "up"))
# adply(1:6, 1, function(x) test_func(distribute_radiation_down, P(x, "down")))
# adply(1:6, 1, function(x) test_func(distribute_radiation_up,   P(x, "up")))

distribute_radiation(P(1, "down"))
# adply(1:7, 1, function(x) distribute_radiation(P(x, "down")))
# adply(1:6, 1, function(x) distribute_radiation(P(x, "up")))

# Conductive heat budget  
P = function(windSpeed) {
  p = read.table(header=TRUE, sep=",", text=
   "Layer,T, U, U_, Cp, Volume, Volume_, Eps, Eps_
    Sky,-10,0,0,-999,1,1,1,1
    Glass,15,3.3, 0,8400,2,1,0.8,0.8
    Screen,18, 1.2,1.2,2280,2,2,0.92,0.06
    Light,0,0,0,-999,2,2,0,0
    Plant,24,0,0,-999,2,2,0.8,0.8
    Heating, 60, 2, 2, -999, 2, 2, 1, 1
    Floor,12, 0.1,7.5,42000,3,2,0,0.6"
  )
  p$U_[2] = Uwind(windSpeed)
  p[p==(-999)] = Inf
  p
}

volumes = data.frame (
  Name    = c("Outside", "Inside", "Soil"),
  T       = c(-2, 21, 4),
  Cp      = c(Inf, 1020, Inf),
  Density = c(Inf, 4.44*1.19, Inf)
)

Uwind = function(windSpeed) {
  round(2.8 + 1.2*windSpeed^0.8, 1)
}

P(8)
volumes

conduct_heat = function(L, V, dt) {
  L$A   = 0
  L$A_  = 0
  V$A  = 0
  n = nrow(L)
  for (i in 1:n) {
    vol_above = L$Volume_[i]
    vol_below = L$Volume[i]
    L$A_[i] = (V$T[vol_above] - L$T[i])*L$U_[i]*dt
    L$A[i]  = (V$T[vol_below] - L$T[i])*L$U[i]*dt
    V$A[vol_above] = V$A[vol_above] - L$A_[i]
    V$A[vol_below] = V$A[vol_below] - L$A[i]
  }
  V$dT = V$A/V$Cp/V$Density
  L$dT  = (L$A + L$A_)/L$Cp
  list(L, V)
}

i = 0
both = conduct_heat(P(8), volumes, 120)
while (i < 200 & sum(c(abs(both[[1]]$dT), abs(both[[1]]$dT))) > 1) {
  i = i + 1
  both[[1]]$T = both[[1]]$T + both[[1]]$dT
  both[[2]]$T = both[[2]]$T + both[[2]]$dT
  both = conduct_heat(both[[1]], both[[2]], 120)
}
print(i)
print(both)

# Radiative heat budget  

L = P(8)

K = function(celcius) {
  celcius + 273.15
}

distribute_radiation = function(L) {
  sigma = 5.6704e-8
  n = nrow(L)
  i = 1
  L$Next = -1
  while (i < n) {
    j = i
    i = i+1
    while (i <= n && L$Eps_[i] == 0) i = i+1 
    if (L$Eps_[i] > 0) L$Next[j] = i
  }

  L$A = 0
  L$A_ = 0
  m = sum(L$Next > 0)
  for (i in 1:m) {
    # Flux between this layer and the next layer below
    i_below = L$Next[i]
    eps = 1/(1/L$Eps[i] + 1/L$Eps_[i_below] - 1)
    # Absorbed from layer below
    absorbed = sigma*eps*(K(L$T[i_below])^4 - K(L$T[i])^4)
    # print(absorbed)
  }
}
