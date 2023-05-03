source("greenhouse-geometry.R")

a_end  = 0.15
r_end  = 0.35
t_end  = 0.60

a_other = 0.10
r_other = 0.10
t_other = 0.80

# Radiative parameters
weighted_average = function(w_end, w_side1) {
  A_side1 = A_side2 = M$A_side
  Aw = with(M, w_end*A_end + w_side1*A_side1 + A_side2 + A_roof)
  a = with(M, w_end*A_end*a_end + w_side1*A_side1*a_other + (A_side2 + A_roof)*a_other)/Aw
  r = with(M, w_end*A_end*r_end + w_side1*A_side1*r_other + (A_side2 + A_roof)*r_other)/Aw
  t = 1 - a - r
  avg = c(a=a, r=r, t=t)
  round(avg,3)
}

weighted_average = function(w_end, w_side1) {
  A_side1 = A_side2 = M$A_side
  Aw = with(M, w_end*A_end + w_side1*A_side1 + A_side2 + A_roof)
  a0 = with(M, w_end*A_end*a_end + w_side1*A_side1*a_other + (A_side2 + A_roof)*a_other)/Aw
  r0 = with(M, w_end*A_end*r_end + w_side1*A_side1*r_other + (A_side2 + A_roof)*r_other)/Aw
  t0 = with(M, w_end*A_end*t_end + w_side1*A_side1*t_other + (A_side2 + A_roof)*t_other)/Aw
  a = a0/(a0+r0+t0)
  r = r0/(a0+r0+t0)
  t = t0/(a0+r0+t0)
  avg = c(a=a, r=r, t=t)
  round(avg,3)
}

weighted_average(0.25, 0.20)
weighted_average(1,0.20)

# Heat capacity
C_cover = with(M,(A_end*18000 + (A_side + A_roof)*8400)/A_gh)
C_cover

C_cover = with(M,(A_end*8400 + (A_side + A_roof)*8400)/A_gh)
C_cover

# U-value
U_cover = with(M,(A_end*0.3 + (A_side + A_roof)*1.2)/A_gh)
U_cover
