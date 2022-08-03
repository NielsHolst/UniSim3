library(robustHD)
replace_col_name = function(old, new) {
  ix = which(colnames(sim)==old)
  colnames(sim)[ix] <<- new
}

# setwd("/users/au152367/Documents/PAPERS/2017/Nansen model")
# load("sim.Rdata")

# sim = sim[-1]
# replace_col_name("density.thresholdGen.end", "CritDensity")
# replace_col_name("frequencies.thresholdGen.end", "CritResistance")
# replace_col_name("coverage.maxValue.end", "CoverageMax")
# replace_col_name("K.maxValue.end", "Kmax")
# replace_col_name("proportion.bounds.logP.end", "ImmigrationRate")
# replace_col_name("q.bounds.logP.end", "ImmigrationRProp")
# replace_col_name("max.Mortality", "MortalityMax")
# replace_col_name("slope.end", "CoverageSlope")
# replace_col_name("resistanceGain.end", "ResistanceGain")
# replace_col_name("fitnessPenalty.end", "FitnessPenalty")
# replace_col_name("R.end", "R")

# sim$CritDensity[sim$CritDensity==0] = 10000 
# sim$CritResistance[sim$CritResistance==0] = 10000 
# sim$CritDensity = log10(sim$CritDensity)
# sim$CritResistance = log10(sim$CritResistance)

# M = melt(sim, id.vars=c("CritDensity","CritResistance"), variable.name="Parameter", value.name="Value")

# sz = 1
# M = mutate(M,
  # CritResistanceQuad = pmin(floor(CritResistance/sz)*sz, 3) + sz/2,
  # CritDensityQuad = pmin(floor(CritDensity/sz)*sz, 3) + sz/2
# )
# summary(M)

# MM = ddply(M, .(Parameter, CritResistanceQuad, CritDensityQuad), summarise, 
  # Median = median(Value),
  # N = length(Value)
# )

# MM = ddply(MM, .(Parameter), mutate,
    # StdMedian = standardize(Median)
# )

# ggplot(subset(MM, Parameter=="R"),  aes(x=CritResistanceQuad, y=CritDensityQuad)) +
  # geom_raster(aes(fill=100*N/1000), interpolate=TRUE) +
  # scale_fill_distiller(type="seq", palette=5, direction=1) + 
  # labs(x = "Log10(number of generations) until resistance threshold is reached") + 
  # labs(y = "Log10(number of generations) until pest density threshold is reached") +
  # guides(fill=guide_legend(title="% of outcomes"))
   
# ggplot(MM, aes(x=CritResistanceQuad, y=CritDensityQuad)) +
  # geom_raster(aes(fill=StdMedian), interpolate=FALSE) +
  # scale_fill_distiller(type="seq", palette=7, direction=1) + 
  # facet_wrap(~Parameter, scales="free", ncol=3) +
  # labs(x = "Log10(number of generations) until resistance threshold is reached") + 
  # labs(y = "Log10(number of generations) until pest density threshold is reached") + 
  # theme(
    # legend.position="none"
  # )
  
  