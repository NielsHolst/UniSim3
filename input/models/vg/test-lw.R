R0 = budget_lw()
# R0$A = R0$A_ = 0
R1 = distribute_rad_down(R0)

round_rad(rbind(R0, rep(9999,ncol(R0)), R1))
sum(R0$F)
dF = R1$F_ - R0$F_
sum(R1$A) + sum(R1$A_) + sum(dF)

R2 = distribute_rad_up(R1)
round_rad(rbind(R1, rep(9999,ncol(R0)), R2))
sum(R1$F_)
dF = R2$F - R1$F
sum(R2$A) + sum(R2$A_) + sum(dF) - sum(R1$A) - sum(R1$A_)

R6 = distribute_rad_iteratively(R0)
round_rad(rbind(R0, rep(9999,ncol(R0)), R6))
sum(R0$F) + sum(R0$F_)
sum(R6$A) + sum(R6$A_)
