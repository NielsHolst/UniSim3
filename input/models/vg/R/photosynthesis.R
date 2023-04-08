# All functions below are from the plantecophys package (except for the example plots)
# The leaf_photosynthesis function has been abbreviated and boundary layer conductance added
# The canopy-photosynthesis functions was invented by me
# niels.holst@agro.au.dk

.Rgas <- function()8.314
Tk <- function(x)x+273.15

RHtoVPD <- function(RH, TdegC, Pa=101){
	esatval <- esat(TdegC, Pa)
	e <- (RH/100) * esatval
	VPD <- (esatval - e)/1000
return(VPD)
}

esat <- function(TdegC, Pa=101){  
  a <- 611.21
  b <- 17.502
  c <- 240.97
  f <- 1.0007 + 3.46 * 10^-8 * Pa * 1000
  esatval <- f * a * (exp(b * TdegC/(c + TdegC)))
  return(esatval)
}

# Arrhenius
arrh <- function(Tleaf, Ea){
  exp((Ea * (Tk(Tleaf) - 298.15)) / (298.15 * .Rgas() * Tk(Tleaf))) 
}

TKm <- function(Tleaf, Patm=101.325,
                Oi = 210,      # O2 concentration (mmol mol-1)
                Ec = 79430.0,  # activation energy for Kc 
                Eo = 36380.0,  # activation energy for Ko
                Kc25 = 404.9,  # Kc at 25C
                Ko25 = 278.4  # Ko at 25C
                ){
  
  Oi <- Oi * Patm / 100
  
  Ko <- Ko25*arrh(Tleaf, Eo)
  Kc <- Kc25*arrh(Tleaf, Ec)
  Km <- Kc * (1.0 + Oi / Ko)
  
return(Km)
}

# Vcmax temperature response (Arrhenius)
TVcmax <- function(Tleaf, EaV, delsC, EdVC){
  
  if(EdVC > 0){
    V1 <- 1+exp((delsC*(25 + 273.15)-EdVC)/(.Rgas()*(25 + 273.15)))
    V2 <- 1+exp((delsC*(Tleaf+273.15)-EdVC)/(.Rgas()*(Tk(Tleaf))))
    f <- V1/V2
  } else f <- 1
  
  exp((Tleaf-25)*EaV/(.Rgas()*Tk(Tleaf)*Tk(25))) * f
}

# Jmax temperature response (Arrhenius)
TJmax <- function(Tleaf, EaJ, delsJ, EdVJ){
  J1 <- 1+exp((298.15*delsJ-EdVJ)/.Rgas()/298.15)
  J2 <- 1+exp((Tk(Tleaf)*delsJ-EdVJ)/.Rgas()/Tk(Tleaf))
  exp(EaJ/.Rgas()*(1/298.15 - 1/Tk(Tleaf)))*J1/J2
}

# Non-rectangular hyperbola
Jfun <- function(PPFD, alpha, Jmax, theta){
  (alpha*PPFD + Jmax - 
     sqrt((alpha*PPFD + Jmax)^2 - 4*alpha*theta*PPFD*Jmax))/(2*theta)
}

# - larger root
QUADP <- function(A,B,C){
  
  if((B^2 - 4*A*C) < 0){
    warning("IMAGINARY ROOTS IN QUADRATIC")
    return(0)
  }
  
  if(identical(A,0)){
    if(identical(B,0)){
      return(0)
    } else {
      return(-C/B)
    }
  } else {
   return((- B + sqrt(B^2 - 4*A*C)) / (2*A))
  }
  
}

# - smaller root
QUADM <- function(A,B,C){
  
  if((B^2 - 4*A*C) < 0){
    warning("IMAGINARY ROOTS IN QUADRATIC")
    return(0)
  }
  
  if(identical(A,0)){
    if(identical(B,0)){
      return(0)
    } else {
      return(-C/B)
    }
  } else {
    return((- B - sqrt(B^2 - 4*A*C)) / (2*A))
  }
  
}

leaf_photosynthesis = function(
    # Enviromental variables
      Ca, 
    PPFD,
    RH,
    Tleaf,
    GS,
    GB,     # Added: boundary layer conductance
    # Species-specific parameters:
    # Obligatory
    Jmax, 
    Vcmax, 
    GammaStar,
    Km,
    Rd0,
    # Optional
    alpha = 0.24, 
    theta = 0.85, 
    EaJ   = 29680,
    EdVJ  = 200000,
    delsJ = 631.88,
    EaV   = 58550,
    EdVC  = 200000,
    delsC = 629.26,
    Q10   = 1.92
  ) 
{
  # Added: Total conductance
  GT = GS*GB/(GS + GB)
  
  #---- Constants
  Rgas <- .Rgas()
  GCtoGW <- 1.57     # conversion from conductance to CO2 to H2O
  
  VPD <- RHtoVPD(RH, Tleaf)
    
  # Leaf respiration
  Rd <- Rd0*Q10^((Tleaf-25)/10)
    
  #-- Vcmax, Jmax T responses
  Vcmax <- Vcmax * TVcmax(Tleaf,EaV, delsC, EdVC)
  Jmax <- Jmax * TJmax(Tleaf, EaJ, delsJ, EdVJ)
  
  # Electron transport rate
  J <- Jfun(PPFD, alpha, Jmax, theta)
  VJ <- J/4
        
  # Solution when Rubisco activity is limiting
  # Changes: Use total conductance in stead of stomatal conductance
  GC <- GT / GCtoGW
  A <- 1./GC
  B <- (Rd - Vcmax)/GC - Ca - Km
  C <- Vcmax * (Ca - GammaStar) - Rd * (Ca + Km)
  Ac <- QUADM(A,B,C)
  
  # Photosynthesis when electron transport is limiting
  B <- (Rd - VJ)/GC - Ca - 2*GammaStar
  C <- VJ * (Ca - GammaStar) - Rd * (Ca + 2*GammaStar)
  Aj <- QUADM(A,B,C)
  
  # NOTE: the solution above gives net photosynthesis, add Rd
  # to get gross rates (to be consistent with other solutions).
  Ac <- Ac + Rd
  Aj <- Aj + Rd

  # Hyperbolic minimum.
  Am <- -QUADP(A=1 - 1E-04, B=Ac+Aj, C=Ac*Aj)
  
  # Net photosynthesis
  Am <- Am - Rd
  
  # Calculate Ci 
  Ci <- Ca - Am/GC
  
  # For zero GC:
  Ci[!is.finite(Ci)] <- Ca
  # Stomata fully shut; Ci is not really Ca but that's 
  # how we like to think about it.

  data.frame( 
    ALEAF=Am,
    Ac=Ac,
    Aj=Aj,
    Rd=Rd,
    GT=GT,
    Ci=Ci
  )
}

canopy_photosynthesis = function(
    # Variables needed for canopy model
    Aground, 
    lai, 
    abs_leaf, 
    dt,
    # Environmental variables
    Ca, 
    RH,
    Tleaf,
    GS,
    GB,     
    # Species-specific parameters:
    # Obligatory
    Jmax, 
    Vcmax, 
    GammaStar,
    Km,
    Rd0,
    # Optional
    alpha = 0.24, 
    theta = 0.85, 
    EaJ   = 29680,
    EdVJ  = 200000,
    delsJ = 631.88,
    EaV   = 58550,
    EdVC  = 200000,
    delsC = 629.26,
    Q10   = 1.92
  ) 
{
  Aleaf = Aground/lai
  Fleaf = Aleaf/abs_leaf/dt
  Pleaf = leaf_photosynthesis(
    Ca        = Ca, 
    PPFD      = Fleaf,
    RH        = RH,
    Tleaf     = Tleaf,
    GS        = GS,
    GB        = GB,     
    Jmax      = Jmax, 
    Vcmax     = Vcmax, 
    GammaStar = GammaStar,
    Km        = Km,
    Rd0       = Rd0,
    alpha     = alpha, 
    theta     = theta, 
    EaJ       = EaJ,
    EdVJ      = EdVJ,
    delsJ     = delsJ,
    EaV       = EaV,
    EdVC      = EdVC,
    delsC     = delsC,
    Q10       = Q10
  )
  data.frame(
    Pn_ground = lai*Pleaf$ALEAF,
    R_ground  = lai*Pleaf$Rd
  )
}


#
# Main
#

# Test
leaf_photosynthesis(
  Ca        = 800,
  PPFD      = 1000,
  RH        = 80,
  Tleaf     = 25,
  GS        = 0.1,
  GB        = 0.01,
  Jmax      = 194,
  Vcmax     = 112,
  GammaStar = 41,
  Km        = 600,
  Rd0       = 0.49
)

# Canopy equal to leaf
canopy_photosynthesis(
  Aground   = 1000*60,
  dt        = 60,
  lai       = 1,
  abs_leaf  = 1,
  Ca        = 800,
  RH        = 80,
  Tleaf     = 25,
  GS        = 0.1,
  GB        = 0.01,
  Jmax      = 194,
  Vcmax     = 112,
  GammaStar = 41,
  Km        = 600,
  Rd0       = 0.49
)

# Canopy with LAI>1 and leaf absorptivity<1
canopy_photosynthesis(
  Aground   = 1000*60,
  dt        = 60,
  lai       = 3,
  abs_leaf  = 0.9,
  Ca        = 800,
  RH        = 80,
  Tleaf     = 25,
  GS        = 0.1,
  GB        = 0.01,
  Jmax      = 194,
  Vcmax     = 112,
  GammaStar = 41,
  Km        = 600,
  Rd0       = 0.49
)
  
plot_leaf_photo_synthesis = function() {
  M = expand.grid(
    Ca    = c(600, 1200),
    PPFD  = 10*(0:100),
    Tleaf = c(15, 30)
  )

  M = ddply(M, .(Ca, PPFD, Tleaf), 
    function(x) {
      leaf_photosynthesis(
      Ca        = x$Ca,
      PPFD      = x$PPFD,
      RH        = 80,
      GS        = 0.1,
      GB        = 0.01,
      Tleaf     = x$Tleaf,
      Jmax      = 194,
      Vcmax     = 112,
      GammaStar = 41,
      Km        = 600,
      Rd0       = 0.49
      )
    }
  )
  M$Ca    = factor(M$Ca)
  M$Tleaf = factor(M$Tleaf)

  windows(5,3)
  ggplot(M, aes(PPFD, ALEAF, colour=Ca)) +
    geom_line(size=1, aes(linetype=Tleaf)) +
    labs(
      x="Light intensity (micromol PAR/m2 leaf/s)",
      y="Net photosynthesis\n(micromol CO2/m2 leaf/s)",
      colour="CO2 (ppm)",
      linetype="Tleaf (oC)"
    ) +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}
  
plot_canopy_photo_synthesis = function() {
  M = expand.grid(
    Ca      = c(600, 1200),
    Aground = 10*(0:100),
    Tleaf   = c(15, 30)
  )

  M = ddply(M, .(Ca, Aground, Tleaf), 
    function(x) {
      canopy_photosynthesis(
      Aground   = x$Aground*60,
      dt        = 60,
      lai       = 3,
      abs_leaf  = 0.9,
      Ca        = x$Ca,
      RH        = 80,
      GS        = 0.1,
      GB        = 0.01,
      Tleaf     = x$Tleaf,
      Jmax      = 194,
      Vcmax     = 112,
      GammaStar = 41,
      Km        = 600,
      Rd0       = 0.49
      )
    }
  )
  M$Ca    = factor(M$Ca)
  M$Tleaf = factor(M$Tleaf)

  windows(5,3)
  ggplot(M, aes(Aground, Pn_ground, colour=Ca)) +
    geom_line(size=1, aes(linetype=Tleaf)) +
    labs(
      x="Light intensity (micromol PAR/m2 ground/s)",
      y="Net photosynthesis\n(micromol CO2/m2 ground/s)",
      colour="CO2 (ppm)",
      linetype="Tleaf (oC)"
    ) +
    theme(
      axis.title.x = element_text(margin = margin(t=8)),
      axis.title.y = element_text(margin = margin(r=9))
    )
}

# setwd("~/sites/ecolmod3/media/models/vg")
plot_leaf_photo_synthesis()
plot_canopy_photo_synthesis()
