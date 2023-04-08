library(plyr)

n = 25
w = 4
L = 100
h = 3.5
u = 26/180*pi

. = data.frame(
  A_side = L*h
)

. = mutate(.,
  A_end = w*h + w^2/4*sin(u),
  A_roof = w*L/2*sqrt(1 + sin(u)^2),
  A_cover = 2*(A_side + n*A_end + n*A_roof),
  A_gh = n*L*w,
  V_gh = n*L*A_end,
  cover_per_ground = A_cover/A_gh,
  avg_height = V_gh/A_gh
)

round(t(.), 3)

# Alternative (same result)
. = data.frame(
  roofHeight = sin(u)*w/2
)

. = mutate(.,
  roofWidth  = sqrt(.$roofHeight^2 + (w/2)^2),
  roofArea   = roofWidth*L,
  roofArea2  = w*L/2*sqrt(1+sin(u)^2),
  gableArea  = roofHeight*w/2,
  gableArea1 = w^2/4*sin(u),
  side       = L*h,
  end        = w*h + gableArea,
  cover      = 2*side + 2*n*end + 2*n*roofArea,
  ground     = n*L*w,
  volume     = ground*h + n*gableArea*L,
  volume2    = end*L*n,
  avgHeight  = volume/ground
)

round(t(.), 3)
