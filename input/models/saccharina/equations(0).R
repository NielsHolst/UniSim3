rm(list=ls(all=TRUE))

# Concentrations
cSC = 0.21
cSN = 0.02
cCC = 0.47
cCN = 0
cNC = 0.21
cNN = 0.37
cWCmax = 0.35
cWNmax = 0.05

# Eq. 2
vS = cCC*(cNN   -cWNmax) + cNC*(cWNmax-cCN   ) + cWCmax*(cCN-cNN)
vC = cSC*(cWNmax-cNN   ) + cNC*(cSN   -cWNmax) + cWCmax*(cNN-cSN)
vN = cSC*(cCN   -cWNmax) + cCC*(cWNmax-cSN   ) + cWCmax*(cSN-cCN)
k  = cSC*(cCN   -cNN)    + cCC*(cNN   -cSN)    + cNC*   (cSN-cCN)
wS = vS/k
wC = vC/k
wN = vN/k

# Show
list(
  wS = wS,
  wC = wC,
  wN = wN
)

# State
Sdw  = 34.50
Cdw  = 40.0
Ndw  =  3.0
dSdw =  4.0

# Eq. 3
dCdw = wC/wS*(Sdw + dSdw) - Cdw
dNdw = wN/wS*(Sdw + dSdw) - Ndw
 Wdw =  1/wS*(Sdw + dSdw)

# Show
list(
  dCdw = dCdw,
  dNdw = dNdw,
  Wdw  =  Wdw
)

# Check
biomass = c(
  structure      = Sdw + dSdw, 
  carbon_store   = Cdw + dCdw, 
  nitrogen_store = Ndw + dNdw
)
biomass
sum(biomass)
biomass/Wdw
