source("greenhouse-geometry.R")

faces = function(...) {
  y = c(...)
  names(y) = c("roof1","roof2","side1","side2","end1", "end2" )
  y
}

A = with(M, faces(rep(A_roof/2,2), rep(A_side/2,2), rep(A_end/2,2)))


a = faces(rep(0.10,4), rep(0.15,2))
r = faces(rep(0.10,4), rep(0.35,2))
t = faces(rep(0.80,4), rep(0.60,2))
w = faces(rep(1,6))
s = faces(rep(1,6))

# Radiative parameters
weighted_average = function(w, s) {
  a = sum(s*w*A*a)/sum(s*w*A)
  r = sum(s*w*A*r)/sum(s*w*A)
  t = 1 - a - r
  avg = c(a=a, r=r, t=t)
  round(avg,3)
}

w1 = faces(rep(1,3), 0.20, rep(0.25,2))
w1

# Verify cover scenarios
weighted_average(w,s)
weighted_average(w1,s)

# Screens on roof only
a = faces(rep(0.10,2), rep(0,4))
r = faces(rep(0.10,2), rep(0,4))
t = faces(rep(0.80,2), rep(1,4))

# Check is weighting in no screen changes result
w2 = faces(rep(1,4), rep(0.25,2))
w
w1
w2
weighted_average(w,s)
weighted_average(w1,s)
weighted_average(w2,s)

