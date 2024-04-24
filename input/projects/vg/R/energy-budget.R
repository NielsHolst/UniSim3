
ports = function(name) {
  str_detect(colnames(sim), name) |> which()
}

port_names = function(name) {
  full_names = colnames(sim)[ports(name)]
  periods = str_locate(full_names, "\\.")
  res = {}
  for (i in 1:nrow(periods)) {
    res = c(res, str_sub(full_names[i], 1, periods[[i]]-1))
  }
  res
}

port_values = function(name) {
  sim[nrow(sim), ports(name)] |> as.numeric()
}

port_names("netConvection")
port_values("netConvection")

#
# Temperatures
#

port_names("temperature")
T = port_values("temperature")
TL = T[4:length(T)]
TV = T[1:3]

#
# Balances
#

L = data.frame(
  swEmissionTop    = -port_values("swEmissionTop"),
  swEmissionBottom = -port_values("swEmissionBottom"),
  swAbsorbedTop    =  port_values("swAbsorbedTop"),
  swAbsorbedBottom =  port_values("swAbsorbedBottom"),
  lwEmissionTop    = -port_values("lwEmissionTop"),
  lwEmissionBottom = -port_values("lwEmissionBottom"),
  lwAbsorbedTop    =  port_values("lwAbsorbedTop"),
  lwAbsorbedBottom =  port_values("lwAbsorbedBottom"),
  convectionTop    =  port_values("convectionTop"),
  convectionBottom =  port_values("convectionBottom"),
  condensation     =  port_values("condensation")
)
L$Total = rowSums(L)
L = rbind(L, colSums(L))
L$Temperature =  c(TL,NA)
rownames(L) = c(port_names("swEmissionTop"), "Total")

V = data.frame(
  heatInflux = port_values("heatInflux")
)
V$Total = rowSums(V)
V = rbind(V, colSums(V))
V$Temperature =  c(TV,NA)
rownames(V) = c(port_names("heatInflux"), "Total")

B = data.frame(
  balanceVent = sim$balanceVent[nrow(sim)],
  balanceConv = sim$balanceConv[nrow(sim)],
  balanceLw   = sim$balanceLw[nrow(sim)],
  balanceSw   = sim$balanceSw[nrow(sim)]
)

sim$dateTime[nrow(sim)] |> print()
round(L,2) |> print()
round(V,2) |> print()
round(B,2) |> print()

