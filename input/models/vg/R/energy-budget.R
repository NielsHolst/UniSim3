
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
# Short-wave balance
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
rownames(L) = c(port_names("swEmissionTop"), "Total")

V = data.frame(
  heatInflux = port_values("heatInflux")
)
V$Total = rowSums(V)
V = rbind(V, colSums(V))
rownames(V) = c(port_names("heatInflux"), "Total")

round(L,2)
round(V,2)
round(sim$ventilationHeatLoss[nrow(sim)],2)

# tail(sim[,c(1,157:184)])

