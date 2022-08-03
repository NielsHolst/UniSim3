rm(list=ls(all=TRUE) )

library(plyr)
library(stringr)

extend_eqs = function(v, i) {
  y = {}
  for (j in 1:length(v))
    y = c(y, paste0(v[j],"*p[",i,"]"), paste0(v[j],"*q[",i,"]"))
  y
}

eval_eqs = function(p) {
  q = 1-p
  y = 1
  for (i in 1:length(p))
    y = extend_eqs(y, i)
  data.frame (
    Eq = y,
    P = aaply(y, 1, function(x) eval(parse(text=x)))
  )
}

compute_ps = function(p) {
  M = eval_eqs(p)
  ps = str_locate_all(M$Eq, "p")
  ps = llply(ps, function(x) (x[,1] + 2)/5)

  n = length(p)
  proportions = ldply(ps, 
    function(x) {
      y = {}
      for (i in 1:n)
        y = c(y, if (i %in% x) 1/length(x) else 0)
      y = data.frame(t(y))
      colnames(y) = paste0("p",1:n)
      y
    }
  )
  cbind(M, proportions)
}


compute_final = function(p) {
  P = {}
  for (i in 3:ncol(ps)) {
    P = c(P, sum(ps$P*ps[,i]))
  }
  P
}

p = c(0.4, 0.1, 0.6, 0.8)
p = rep(0.01,8)

p = c(0.2,0.6)
# q = 1 - p

# p[1]*q[2]
# q[1]*p[2]
# p[1]*p[2]
# q[1]*q[2]

# p[1]*q[2] + p[1]*p[2]/2
# q[1]*p[2] + p[1]*p[2]/2
# p = c(0.2,0.6,0.5)


ps = compute_ps(p)
P  = compute_final(p)
notP = tail(ps$P,1)
P
notP
sum(c(P, tail(ps$P,1)))

