# Parameters
cCstr   = 0.21
cCCres  = 0.47
cCNres  = 0.21

cNstr   = 0.02
cNCres  = 0
cNNres  = 0.37

cCtotMax = 0.35
cNtotMax = 0.05

# Primary state variables
Wstr    = NA
WCres   = NA
WNres   = NA

# Derived state variables
Cstr    = function() Wstr *cCstr
CCres   = function() WCres*cCCres
CNres   = function() WNres*cCNres
Ctot    = function() Cstr() + CCres() + CNres()

Nstr    = function() Wstr *cNstr
NCres   = function() WCres*cNCres
NNres   = function() WNres*cNNres
Ntot    = function() Nstr() + NCres() + NNres()

Wtot    = function() Wstr + WCres + WNres
cCtot   = function() Ctot()/Wtot()
cNtot   = function() Ntot()/Wtot()

# Find optimal size of compartments
w1 = function(cCtot, cNtot) cCCres*(cNNres - cNtot) + cCNres*(cNtot - cNCres) + cCtot*(cNCres - cNNres)
w2 = function(cCtot, cNtot) cCCres*(cNNres - cNtot) + cCNres*(cNtot - cNCres) + cCtot*(cNCres - cNNres)
