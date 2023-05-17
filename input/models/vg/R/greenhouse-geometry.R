library(plyr)

n = 25
w = 4
L = 100
h = 3.5
u = 26/180*pi

M = data.frame(
  A_side_ = L*h
)

M = mutate(M,
  A_end_  = w*h + w^2/4*sin(u),
  A_roof_ = w*L/2*sqrt(1 + sin(u)^2),
  A_side  = 2*A_side_,
  A_end   = 2*n*A_end_,
  A_roof  = 2*n*A_roof_,
  A_cover = A_side + A_end + A_roof,
  A_gh    = n*L*w,
  V_gh    = n*L*A_end_,
  cover_per_ground = A_cover/A_gh,
  avg_height = V_gh/A_gh,
  C_gh = 1.19*1020*avg_height
)

round(t(M), 3)
