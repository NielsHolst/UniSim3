cSC = 0.21
cSN = 0.02
cCC = 0.47
cCN = 0
cNC = 0.21
cNN = 0.37
cWC = 0.25
cWN = 0.05

# Eq. 2
vS = cCC*(cNN   -cWN) + cNC*(cWN-cCN   ) + cWC*(cCN-cNN)
vC = cSC*(cWN-cNN   ) + cNC*(cSN   -cWN) + cWC*(cNN-cSN)
vN = cSC*(cCN   -cWN) + cCC*(cWN-cSN   ) + cWC*(cSN-cCN)
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
