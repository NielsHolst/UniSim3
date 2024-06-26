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

M = sim
M$Biocontrol = "Unsuccesful"
M$Biocontrol[M$yieldImprovement>yieldThreshold] = "Successful"
M$Biocontrol = factor(M$Biocontrol)
M$Biocontrol = reorder_levels(M$Biocontrol, c(2:1))


make_plot = function() {
  ggarrange(
    ggplot(M, aes(percentageCadaversGs43, fill=Biocontrol)) +
      scale_fill_manual(values=grey_scale2) +
      geom_density(alpha=0.8, size=0.3) +
      xlim(0,4) +
      labs(x="Cadaver prevalence (%) in GS 43", y="") + 
      theme1 +
      theme(
        legend.position = "none"
      )
    ,
    ggplot(M, aes(percentageCadaversGs61, fill=Biocontrol)) +
      scale_fill_manual(values=grey_scale2) +
      geom_density(alpha=0.8, size=0.3) +
      xlim(0,80) +
      labs(x="Cadaver prevalence (%) in GS 61", y="Probability density") +
      theme1 +
      theme(
        legend.position = "none"
      )
    ,
    ggplot(M, aes(percentageCadaversGs73, fill=Biocontrol)) +
      scale_fill_manual(values=grey_scale2) +
      geom_density(alpha=0.8, size=0.3) +
      xlim(0,80) +
      labs(x="Cadaver prevalence (%) in GS 73", y="") + 
      theme1 +
      theme(
        legend.position = "none"
      )
    ,
    ncol=1, align="hv"
  )
}

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

# Hook for batch command
figures = function(df) {
  Pages = list(Page = list(Grob=make_plot(), Width=mm(W), Height=mm(H)))
}
