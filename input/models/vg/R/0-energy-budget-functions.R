# Prelude
# rm(list=ls(all=TRUE))
# graphics.off()
library(plyr)
library(reshape2)

distribute_rad_down = function(rad) {
  n = nrow(rad)
  for (i in 1:(n-1)) {
    # Correct absorption and transmission of this layer and layer below
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

distribute_radiation = function(rad, verbose=TRUE) {
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
  R1
}

round_lay = function(x, include_rad_par=TRUE, include_F=FALSE) {
  if (!include_F) {
	ix = which(colnames(x)=="F" | colnames(x)=="F_")
	x = x[,-ix]
  }
  layer   = x[,1]
  rad_par = x[,2:7]
  rest    = x[,8:ncol(x)] 
  if (include_rad_par) {
	M = cbind(layer, round(rad_par,2), round(rest,1))
    colnames(M) = colnames(x)
  } else {
	M = cbind(layer, round(rest,1))
	colnames(M) = colnames(x)[c(1, 8:ncol(x))]
  }
  M
}

round_vol = function(x) {
  volume  = x[,1]
  rest    = x[,2:ncol(x)]
  M = cbind(volume, round(rest, 1))
  colnames(M) = colnames(x)
  M
}

round_water = function(x) {
  volume  = x[,1]
  rest    = x[,2:ncol(x)]*1000 # from kg to g
  M = cbind(volume, round(rest, 3))
  colnames(M) = colnames(x)
  M
}

round_budget = function(x) {
  R = list(
    Layers  = round_lay(x$Layers, FALSE), 
    Volumes = round_vol(x$Volumes)
  )
  if ("Water" %in% names(x)) c(R, list(Water = round_water(x$Water))) else R
}
