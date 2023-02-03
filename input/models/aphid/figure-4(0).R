# Load your sim data
sim_data_file = "~/sites/ecolmod3/download/aphid-biocontrol-sim.Rdata"

# Load standard script
source("~/QDev/UniSim3/input/scripts/begin.R")

# Output folder
output_folder = "~/QDev/UniSim3/output"

# Here goes
load(sim_data_file)
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

density_lab = "Probability density\n"
prevalence_lab = "Peak exposed prevalence (%)"
cadaver_lab = "Peak cadaver prevalence (%)"

successful =  "More successful"
unsuccessful = "Less successful"
sim$Outcome = unsuccessful
sim$Outcome[sim$yieldImprovement > yieldThreshold] = successful
sim$Outcome = factor(sim$Outcome)

make_density = function(col) {
  dens = density(sim[sim$Outcome==successful, col])
  M1 = data.frame(
    Outcome = successful,
    MaxPrevalence = dens$x, 
    Density = dens$y
  )
  dens = density(sim[sim$Outcome==unsuccessful, col])
  M2 = data.frame(
    Outcome = unsuccessful,
    MaxPrevalence = dens$x, 
    Density = dens$y
  )
  rbind(M1, M2)
}

make_plot = function() {
  a = 0.8
  
  M1 = make_density("maxPrevalence")
  max(M1$Density)
  M1$Density[M1$Density > 0.12] = 0.12
  M2 = make_density("maxCadaverPrevalence")
  
  ggarrange(
    ggplot(M1,aes(MaxPrevalence)) +
      geom_ribbon(ymin=0, aes(ymax=Density, fill=Outcome), colour="black", alpha=a, size=0.3) +
      scale_discrete_manual(c("colour", "fill"), values=grey_scale2, name="Yield\nimprovement") +
      scale_x_continuous(breaks=25*(0:4), limits=c(0,100)) +
      labs(x=prevalence_lab, y=density_lab) +
      theme1 +
      theme(
        legend.position = "none"
      )
    ,
    ggplot(M2,aes(MaxPrevalence)) +
      geom_ribbon(ymin=0, aes(ymax=Density, fill=Outcome), colour="black", alpha=a, size=0.3) +
      scale_discrete_manual(c("colour", "fill"), values=grey_scale2, name="Yield\nimprovement") +
      scale_x_continuous(breaks=25*(0:4), limits=c(0,100)) +
      labs(x=cadaver_lab, y=density_lab) +
      theme1 +
      theme(
        legend.position = "none"
      )
    ,
    ncol=1, align="hv", labels="auto"
  )
}

quantiles = c(0.25, 0.50, 0.75)
quantile(sim$maxPrevalence[sim$Outcome==unsuccessful],        quantiles)
quantile(sim$maxPrevalence[sim$Outcome==successful],          quantiles)
quantile(sim$maxCadaverPrevalence[sim$Outcome==unsuccessful], quantiles)
quantile(sim$maxCadaverPrevalence[sim$Outcome==successful],   quantiles)

# Dimensions
W = 84
H = 110

# Screen plot
graphics.off()
open_plot_window(mm(W),mm(H))
print(make_plot())

# Write figures
write_figure = function(file_type) {
  file_name_path = paste0(output_folder, "/fig-4-bw.", file_type)
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
write_figure("eps")


