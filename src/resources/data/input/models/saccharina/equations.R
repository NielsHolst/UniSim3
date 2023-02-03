rm(list=ls(all=TRUE))

# Fixed concentrations in compartments
cSC = 0.21
cCC = 0.47
cSN = 0.02
cNN = 0.37
cPP = 0.33

# Optimal proportions of compartments
wS = 0.1861
wC = 0.6615
wN = 0.1251
wP = 0.02727

# State
Sdw  = 34.50
Cdw  = 40.0
Ndw  =  3.0
Pdw  =  1.0
dSdw =  4.0

# Example: Only structural demand gets fulfilled
Wdw = Sdw + Cdw + Ndw + Pdw
list(
  Wdw,
  100*(Sdw+dSdw)/(Wdw + dSdw),
  100*(Cdw     )/(Wdw + dSdw),
  100*(Ndw     )/(Wdw + dSdw),
  100*(Pdw     )/(Wdw + dSdw)
)

# Demands
dCdw = wC/wS*(Sdw + dSdw) - Cdw
dNdw = wN/wS*(Sdw + dSdw) - Ndw
dPdw = wP/wS*(Sdw + dSdw) - Pdw

# Show
list(
  dCdw,
  dNdw,
  dPdw,
  Wdw
)

# Check
biomass = c(
  Sdw + dSdw, 
  Cdw + dCdw, 
  Ndw + dNdw,
  Pdw + dPdw
)
biomass
100*biomass/sum(biomass)

Wdw =1/wS*(Sdw + dSdw)
Wdw
sum(biomass)
