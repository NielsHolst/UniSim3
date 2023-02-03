# Load your sim and S data
sim_data_file = "~/sites/ecolmod3/download/aphid-biocontrol-sim.Rdata"
S_data_file   = "~/sites/ecolmod3/download/aphid-biocontrol-S.Rdata"

# Load standard script
source("~/QDev/UniSim3/input/scripts/begin.R")
source("D:/Documents/QDev/UniSim3/input/scripts/begin-sobol.R")

# Output folder
output_folder = "~/QDev/UniSim3/output"

# Here goes
load(sim_data_file)
load(S_data_file)
dim(sim)
dim(S)

# Prepare analysis
sobol_k = length(unique(S$Input)) - 1
sobol_N = nrow(sim)/(sobol_k+2)
iterationColumn = "iteration"
stepColumn      = "step"

make_plot = function() {

  f = function(combi) { 
    M = sobol_indices( sobol_inputs(combi$ix_output, combi$N) )
    M$Output = output_names()[combi$ix_output]
    M
  }
  g = function(m) {
    data.frame(Effect=sum(m$Effect))
    }

  breaks = 2^(9:log2(sobol_N))
  combinations = expand.grid(ix_output=1:length(output_names()), N=breaks)
  M = adply(combinations, 1, f)[,-1]

  Msum = ddply(M, .(Output, Measure, N), g)
  Msum$Input = "Sum"
  M = rbind(M,Msum)

  totals = subset(M, Measure=="Total" & Output==output_names()[1] & N==max(N))
  alphabetic_ordering = order(totals$Input)
  totals = totals[alphabetic_ordering, ]
  effect_ordering = order(totals$Effect, decreasing=TRUE)
  M$Input = reorder_levels(factor(M$Input), effect_ordering)
  M$Output = factor(M$Output)
  M$Measure = factor(M$Measure)
  # Show only relevant variables 
  M = subset(M, Input=="Sum" & Output %in% c("yieldImprovement","maxPrevalence","maxCadaverPrevalence"))
  M$Output = droplevels(M$Output)
  M$Output = reorder_levels(factor(M$Output), c(2,1,3))
  levels(M$Output) = c("Peak exposed\nprevalence","Peak cadaver\nprevalence","Yield\nimprovement")
  levels(M$Measure) = c("First\norder","Total")

  ggplot(M, aes(x=N, y=Effect, colour=Measure)) +
    geom_line() +
    geom_point() +
    scale_x_continuous(trans="log2", breaks=breaks, labels = scales::trans_format("log2", scales::math_format(2^.x))) +
    labs(x="Sample size (N)", y="Sobol' index") +
    ylim(-0.05,NA) +
    geom_hline(yintercept=0, colour="grey") +
    geom_hline(yintercept=1, colour="grey", size=1) +
    facet_grid(~Output) +
    guides(color=guide_legend(reverse=TRUE)) +
    scale_colour_manual(values = c(blue, red)) + 
    theme_bw() 
}

W = 174
H = 80

# Screen plot
graphics.off()
open_plot_window(mm(W),mm(H))
print(make_plot())

# Write figures
write_figure = function(file_type) {
  file_name_path = paste0(output_folder, "/fig-8-colour.", file_type)
  print(paste("Writing figure to", file_name_path))
  if (file_type == "png")
    png(file_name_path, width=W, height=H, units="mm", res=1200, type="cairo-png")
  else if (file_type == "eps")
    cairo_ps(file_name_path, width=mm(W), height=mm(H))
  else
    stop(paste0("Wrong file type: '", file_type, "'"))
  print(make_plot())
  dev.off()
}

if (!dir.exists(output_folder)) dir.create(output_folder, recursive=TRUE)
write_figure("png")
