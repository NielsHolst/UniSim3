library(plyr)
u = 26/180*pi
w = 4
h = 3.5
n = 25
length = 100

. = data.frame(
  roofHeight = sin(u)*w/2
)

. = mutate(.,
  roofWidth  = sqrt(.$roofHeight^2 + (w/2)^2),
  roofArea   = roofWidth*length,
  roofArea2  = w*length/2*sqrt(1+sin(u)^2),
  gableArea  = roofHeight*w/2,
  gableArea1 = w^2/4*sin(u),
  side       = length*h,
  end        = w*h + gableArea,
  cover      = 2*side + 2*end + 2*n*roofArea,
  ground     = n*length*w,
  volume     = ground*h + n*gableArea*length,
  volume2    = end*length*n,
  avgHeight  = volume/ground
)

round(t(.), 3)

