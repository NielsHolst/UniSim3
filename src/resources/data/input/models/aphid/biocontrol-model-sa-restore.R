# biocontrol-model-sa-restore.R
rm(list=ls(all=TRUE))

# Load standard scripts
source("~/QDev/UniSim3/input/scripts/begin.R")
source("~/QDev/UniSim3/input/scripts/begin-sobol.R")

# Load your data
setwd("~/sites/ecolmod3/code")
load("biocontrol-model-sa_0023.Rdata")
load("biocontrol-model-sa_0023-S.Rdata")

# Prepare analysis
sobol_k = length(unique(S$Input)) - 1
sobol_N = nrow(sim)/(sobol_k+2)
iterationColumn = "iteration"
stepColumn      = "step"

# Plot trends
plot_facetted(
  df   = sim[1:min(nrow(sim),1000),], 
  x    = input_names(), 
  y    = output_names(), 
  ncol = 1, 
  dir  = 'h') + geom_smooth(colour='yellow')

# Plot convergences
plot_sobol_convergence()

# Plot indices
ggarrange(
  plotlist=plot_sobol_indices(S)
)