# Load standard script
source("~/QDev/UniSim3/input/scripts/begin.R")

# Output folder
output_folder = "~/QDev/UniSim3/output"

# Here goes

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

M = data.frame(
  Preserve       = c(0, 0.27, 0.9)/100,
  Photosynthesis = c(0, 1.0, 1.0) 
)

make_plot = function() {
  ggplot(M, aes(Preserve, Photosynthesis)) +
    geom_point(data=M[c(1,3),], size=2, colour=orange) +
    geom_line(colour=orange) +
    scale_x_continuous(breaks=M$Preserve, l) +
    labs(x="Plant phosphorus concentration", y="Relative\nphotosynthesis") +
    theme1
}

# Dimensions
W = 84
H = 50

# Screen plot
# graphics.off()
open_plot_window(mm(W),mm(H))
print(make_plot())

# Write figure
write_figure = function(output_folder, file_name, a_ggplot, W, H) {
  if (!dir.exists(output_folder)) 
    dir.create(output_folder, recursive=TRUE)
  file_name_path = paste(output_folder, file_name, sep="/")
  print(paste("Writing figure to", file_name_path))

  if (str_detect(file_name, "\\.png$"))
    png(file_name_path, width=W, height=H, units="mm", res=1200, type="cairo-png")
  else if (str_detect(file_name, "\\.eps$"))
    cairo_ps(file_name_path, width=mm(W), height=mm(H))
  else
    stop(paste0("File has wrong type: '", file_name, "'"))
  print(a_ggplot)
  dev.off()
}

write_figure(output_folder, "P-response.png", make_plot(), W, H)
