rm(list=ls(all=TRUE))
graphics.off()

# double temperatureScaling(
  # double T,
  # double Tref,
  # double Ea,
  # double Ed,
  # double dels
# ) 
# {
  # double
    # a = exp(Ea*(T - Tref)) / R/(T+T0)/(Tref+T0),
    # b = 1. + exp( (dels*(Tref+T0)-Ed)/R/(Tref+T0) ),
    # c = 1. + exp( (dels*(T   +T0)-Ed)/R/(T   +T0) );
    # return a*b/c;
# }



# LeafPhotosynthesisAc
# .EaV = 82620.9
# .EdVC = 0.0
# .Cdels = 645.101

# LeafPhotosynthesisAj
# .EaJ = 39676.9
# .EdVJ = 200000.0
# .Jdels = 641.361

.Rgas <- function()8.314
Tk <- function(x)x+273.15

TJmax <- function(Tleaf, EaJ, delsJ, EdVJ){
  J1 <- 1+exp((298.15*delsJ-EdVJ)/.Rgas()/298.15)
  J2 <- 1+exp((Tk(Tleaf)*delsJ-EdVJ)/.Rgas()/Tk(Tleaf))
  exp(EaJ/.Rgas()*(1/298.15 - 1/Tk(Tleaf)))*J1/J2
}

T0 = 273.15
R = 8.314
Tref = 25

TJmax2 <- function(Tleaf, EaJ, delsJ, EdVJ){
  a = exp(EaJ/R*(1/(T0+Tref) - 1/(T0+Tleaf)))
  b =  1+exp(((T0+Tref) *delsJ-EdVJ)/R/(T0+Tref))
  d = 1+exp(((T0+Tleaf)*delsJ-EdVJ)/R/(T0+Tleaf))
  a*b/d
}

TVcmax <- function(Tleaf, EaV, delsC, EdVC){
  if(EdVC > 0){
    V1 <- 1+exp( (delsC*(25 + 273.15)-EdVC)  / (.Rgas()*(25 + 273.15)) )
    V2 <- 1+exp( (delsC*(Tleaf+273.15)-EdVC) / (.Rgas()*(Tk(Tleaf))) )
    f <- V1/V2
  } else f <- 1
  exp((Tleaf-25)*EaV/(.Rgas()*Tk(Tleaf)*Tk(25))) * f
}

TVcmax2 <- function(Tleaf, EaV, delsC, EdVC){
  a = exp((Tleaf-Tref)*EaV/(R*(T0+Tref)*(T0+Tref)))
  b = 1 + exp( (delsC*(T0+Tref) -EdVC)  / (R*(T0+Tref)) )
  d = 1 + exp( (delsC*(T0+Tleaf)-EdVC) / (R*((T0+Tleaf))) )
  a*b/d
}

x = 0:40
data.frame(
  Tleaf = x,
  TVcmax  = TVcmax (x, 82620.9, 645.101, 0.0),
  TVcmax2 = TVcmax2(x, 82620.9, 645.101, 0.0),
  TJmax   = TJmax  (x, 39676.89, 641.3615, 2e5),
  TJmax2  = TJmax2 (x, 39676.89, 641.3615, 2e5)
)

