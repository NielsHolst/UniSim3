# Load your sim data
parent = "C:/MyDocuments/QDev/UniSim3"
sim_data_file = paste0(parent, "/site/download/aphid-biocontrol-sim.Rdata")
S_data_file = paste0(parent, "/site/download/aphid-biocontrol-S.Rdata")

# Load standard scripts
source(paste0(parent, "/input/scripts/begin.R"))
source(paste0(parent, "/input/scripts/begin-sobol.R"))

# Load your data
load(sim_data_file)
load(S_data_file)
dim(sim)
dim(S)

# Prepare analysis
sobol_k = length(unique(S$Input)) - 1
sobol_N = nrow(sim)/(sobol_k+2)
iterationColumn = "iteration"
stepColumn      = "step"

# Plot trends
open_plot_window(24,14)
P1 = plot_facetted(
  df   = sim[1:min(nrow(sim),1000),], 
  x    = input_names(), 
  y    = output_names(), 
  ncol = 1, 
  dir  = 'h') + geom_smooth(colour='yellow')

# Plot convergences
P2 = plot_sobol_convergence()

# Plot indices
P3 = ggarrange(
  plotlist=plot_sobol_indices(S)
)

# Show on screen
open_plot_window(24,14)
print(P1)

open_plot_window(24,14)
print(P2)

open_plot_window(24,14)
print(P3)

# Hook for batch command
figures = function(df) {
  Pages = 
    list(
      list(Grob=P1, Width=24, Height=14),
      list(Grob=P2, Width=24, Height=14),
      list(Grob=P3, Width=24, Height=14)
    )
}
