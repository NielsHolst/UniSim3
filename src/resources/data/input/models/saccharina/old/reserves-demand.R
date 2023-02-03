rm(list=ls(all=TRUE))

biomassTotal = 1157.81
massCStruct = 180.166
massCRes = 132.987
massNStruct = 17.1586
massNRes = 6.26303
propCRes = 0.47
propNRes = 0.37
propCStruct = 0.21
optCConc = 0.36
optNConc = 0.052
demandStructGrowthC = 5.41848
demandStructGrowthN = 0.516046

C = (massCStruct + massCRes)/biomassTotal
N = (massNStruct + massNRes)/biomassTotal
C
N


fC = function(Dcarbon) {
  Dnitrogen = 220
  biomassStructGrowth = demandStructGrowthC/propCStruct;
  total = biomassTotal + biomassStructGrowth + Dcarbon/propCRes + Dnitrogen/propNRes
  (massCStruct + massCRes + Dcarbon)  /total
}

x = 20*(0:50)
M = data.frame(
  Dcarbon = x,
  Cconc = fC(x)
)
ggplot(M, aes(Dcarbon, Cconc)) +
  geom_line(colour="red")
  


biomassStructGrowth = demandStructGrowthC/propCStruct;
a = optCConc*(propNRes*(biomassTotal+biomassStructGrowth) - massNStruct - massNRes - demandStructGrowthN);

d = optNConc*(propCRes*(biomassTotal+biomassStructGrowth) - massCStruct - massCRes - demandStructGrowthC);
b = (optNConc - propNRes)*(massCStruct + massCRes + demandStructGrowthC);

e = (optCConc - propCRes)*(massNStruct + massNRes + demandStructGrowthN);
c = propCRes*(propNRes - optNConc) - propNRes*optCConc;

f = propNRes*(propCRes - optCConc) - propCRes*optNConc;
Dcarbon   = propCRes*(a+b)/c;
Dnitrogen = propNRes*(d+e)/f;

Dcarbon
Dnitrogen

total = biomassTotal + biomassStructGrowth + Dcarbon/propCRes + Dnitrogen/propNRes
(massCStruct + massCRes + Dcarbon)  /total


