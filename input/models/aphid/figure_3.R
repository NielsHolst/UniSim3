# Load your sim data
do_load = !exists("sim_data_file")
parent = "C:/MyDocuments/QDev/UniSim3"
sim_data_file = paste0(parent, "/site/download/aphid-biocontrol-sim.Rdata")

# Load standard script
source(paste0(parent, "/input/scripts/begin.R"))

# Output folder
output_folder = paste0(parent, "/output")

# Here goes
if (do_load) load(sim_data_file)
dim(sim)

# Common theme
theme1 = theme_classic() + theme(
  axis.title = element_text(size=9),
  axis.title.y = element_text(margin = margin(r=4, unit="points")),
  axis.title.y.right = element_text(margin = margin(l=5, unit="points")),
  axis.text  = element_text(size=8, colour="black"),
  legend.title = element_text(size=8),
  legend.text  = element_text(size=8),
  legend.key.height = unit(12, "points"),
  legend.key.width = unit(12, "points"),
  legend.position = "bottom",
  legend.direction = "vertical",
  legend.spacing.y = unit(2, "points"),
  legend.box.spacing = unit(0, "points"),
  plot.margin = margin(10,10,10,10)
)

# Common scales
bw = c("white", "grey", "black")
grey_scale2  = c("lightgrey", "darkgrey")

# Common yield threshold
yieldThreshold   = unname(quantile(sim$yieldImprovement,0.90))
thresholdLabel = paste0(round(yieldThreshold,1), "%-points")
print(paste("90% fractile of yield improvement =", thresholdLabel))

yield_lab = "Yield improvement (%-points)"
density_lab = "Probability density\n"

successful =  "More successful"
unsuccesful = "Less successful"
sim$Outcome = unsuccesful
sim$Outcome[sim$yieldImprovement > yieldThreshold] = successful
sim$Outcome = factor(sim$Outcome)

make_plot = function(size1, size2) {
  dens = density(sim$yieldImprovement)
  M = data.frame(
    yieldImprovement = dens$x, 
    Density = dens$y
  )
  M$Success = M$yieldImprovement>yieldThreshold
  yend = M$Density[M$Success][1]
  arr = arrow(type="closed", length=unit(2,"mm"))
  ggplot(M, aes(yieldImprovement, Density)) + 
    geom_ribbon(ymin=0, aes(ymax=Density), fill="grey", colour="black") +
    geom_ribbon(ymin=0, aes(ymax=Density, fill=Success), colour="black", size=size1) +
    scale_discrete_manual("fill", values=grey_scale2) +
    annotate("segment", x=yieldThreshold, xend=yieldThreshold, y=0, yend=1.01*yend, size=size2) +
    annotate("text",x=2.5, y=0.05, label="90%", size=3) +
    annotate("text",x=9.0, y=0.11, label=thresholdLabel, size=3) +
    annotate("segment", x=9.0, y=0.095, xend= 1.01*yieldThreshold, yend=1.1*yend, arrow=arr, size=size1) +
    scale_x_continuous(limits=c(0,12)) +
    labs(x=yield_lab, y=density_lab) +
    theme1 +
    theme(
      legend.position = "none"
    )
}

# Dimensions
W = 84
H = 50

# Screen plot
graphics.off()
open_plot_window(mm(W),mm(H))
print(make_plot(0.3, 0.3))

# Write figures
write_figure = function(file_type) {
  file_name_path = paste0(output_folder, "/fig-3-bw.", file_type)
  print(paste("Writing figure to", file_name_path))
  if (file_type == "png")
    png(file_name_path, width=W, height=H, units="mm", res=1200, type="cairo-png")
  else if (file_type == "eps")
    cairo_ps(file_name_path, width=mm(W), height=mm(H))
  else
    stop(paste0("Wrong file type: '", file_type, "'"))
  print(make_plot(0.3, 0.3))
  dev.off()
}

if (!dir.exists(output_folder)) dir.create(output_folder, recursive=TRUE)
write_figure("png")
write_figure("eps")

# Hook for batch command
figures = function(df) {
  Pages = list(Page = list(Grob=make_plot(0.3,0.3), Width=mm(W), Height=mm(H)))
}
