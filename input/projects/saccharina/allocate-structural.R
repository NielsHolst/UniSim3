rm(list=ls(all=TRUE))

M1C   = 100
DCstr = 200
lstr  = 0.12
cCstr = 0.21
eps   = 0.26

phi_strC = M1C/DCstr/(1 + lstr/cCstr + eps)
lstr/cCstr
phi_strC

# Scenario: Carbon is most limiting
Cstr      = phi_strC*DCstr
Cstr_cost = phi_strC*DCstr*lstr/cCstr
Cstr_ex   = phi_strC*DCstr*eps

Cstr     
Cstr_cost
Cstr_ex  
Cstr + Cstr_cost + Cstr_ex
