rm(list=ls(all=TRUE))
library(plyr)

distribute_rad_down = function(rad) {
  n = nrow(rad)
  for (i in 1:(n-1)) {
    # Correct absorption and transmission of this layer and layer radelow
    # for reflections ad infinitum between the two layers
    k   = 1 - rad$r[i+1]*rad$r_[i]
    ah  = rad$a[i+1]/k
    th  = rad$t[i+1]/k
    ah_ = rad$a_[i] /k * rad$r[i+1]
    th_ = rad$t_[i] /k * rad$r[i+1]

    # Absorbed by layer below
    absorbed    = ah*rad$F[i]
    # Transmitted through layer below
    transmitted = th*rad$F[i]

    # Absorbed by this layer by reflection from below
    absorbed_    = ah_*rad$F[i]
    # Transmitted through this layer by reflection from below
    transmitted_ = th_*rad$F[i]

    # Update layer below
    rad$A [i+1] = rad$A[i+1] + absorbed
    rad$F [i+1] = rad$F[i+1] + transmitted

    # Update this layer
    rad$A_[i]   = rad$A_[i] + absorbed_
    rad$F_[i]   = rad$F_[i] + transmitted_
    sum(rad[,2:4]) + rad$F[i+1]
  }
  rad$F = 0
  rad
}

distribute_rad_up = function(rad) {
  n = nrow(rad)
  for (i in n:2) {
    # Correct absorption and transmission of this layer and layer above
    # for reflections ad infinitum between the two layers
    k   = 1 - rad$r_[i-1]*rad$r[i]
    ah_ = rad$a_[i-1]/k
    th_ = rad$t_[i-1]/k
    ah  = rad$a[i] /k * rad$r_[i-1]
    th  = rad$t[i] /k * rad$r_[i-1]

    # Absorbed by layer above
    absorbed_    = ah_*rad$F_[i];
    # Transmitted through layer above
    transmitted_ = th_*rad$F_[i];
    # Absorbed by this layer by reflection from above
    absorbed     = ah*rad$F_[i]
    # Transmitted through this layer by reflection from above
    transmitted  = th*rad$F_[i]

    # Update layer above
    rad$A_[i-1] = rad$A_[i-1] + absorbed_
    rad$F_[i-1] = rad$F_[i-1] + transmitted_

    # Update this layer
    rad$A [i]   = rad$A [i] + absorbed
    rad$F [i]   = rad$F [i] + transmitted

  }
  rad$F_ = 0
  rad
}

distribute_rad_iteratively = function(rad, verbose=TRUE) {
  iter = 0
  precision = 1e-6
  R1 = rad
  R1$F  = R1$E
  R1$F_ = R1$E_
  while (sum(c(R1$F, R1$F_)) > precision) {
    iter = iter + 1
    R0 = distribute_rad_down(R1)
    R1 = distribute_rad_up  (R0)
  }
  if (verbose) print(paste(iter, "iterations needed"))
  m = ncol(R1)
  R1[,1:(m-2)]
}

round_rad = function(x) {
  M = cbind(x[,1], round(x[,-1],2))
  colnames(M)[1] = colnames(x)[1]
  M
}

round_budget = function(x) {
  n = length(x)
  L = vector(mode="list", length=n)
  for (i in 1:length(x)) {
    if (class(x[[i]]) == "data.frame") {
      L[[i]] = round_rad(x[[i]])
    } else {
      L[[i]] = x[[i]]
    }
  }
  names(L) = names(x)
  L
}
