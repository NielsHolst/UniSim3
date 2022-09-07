# Load your sim data
sim_data_file = "~/sites/ecolmod3/code/biocontrol-model-sa_0000.Rdata"

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

# Common yield threshold
yieldThreshold   = unname(quantile(sim$yieldImprovement,0.90))
thresholdLabel = paste0(round(yieldThreshold,1), "%-points")
print(paste("90% fractile of yield improvement =", thresholdLabel))

# Weather file names 1 to 40
file_names = c(
  "Aarnes_2004.txt",
  "Aarnes_2005.txt",
  "Aarnes_2006.txt",
  "Aarnes_2012.txt",
  "Aarnes_2013.txt",
  "Aarnes_2014.txt",
  "Aarnes_2015.txt",
  "Aarnes_2016.txt",
  "Aarnes_2017.txt",
  "Aarnes_2018.txt",
  "Ilseng_2004.txt",
  "Ilseng_2005.txt",
  "Ilseng_2006.txt",
  "Ilseng_2012.txt",
  "Ilseng_2013.txt",
  "Ilseng_2014.txt",
  "Ilseng_2015.txt",
  "Ilseng_2016.txt",
  "Ilseng_2017.txt",
  "Ilseng_2018.txt",
  "Ramnes_2004.txt",
  "Ramnes_2005.txt",
  "Ramnes_2006.txt",
  "Ramnes_2012.txt",
  "Ramnes_2013.txt",
  "Ramnes_2014.txt",
  "Ramnes_2015.txt",
  "Ramnes_2016.txt",
  "Ramnes_2017.txt",
  "Ramnes_2018.txt",
  "Rygge_2004.txt",
  "Rygge_2005.txt",
  "Rygge_2006.txt",
  "Rygge_2012.txt",
  "Rygge_2013.txt",
  "Rygge_2014.txt",
  "Rygge_2015.txt",
  "Rygge_2016.txt",
  "Rygge_2017.txt",
  "Rygge_2018.txt"
)

# Load files names into a data frame
f = function(x) {
  data.frame( 
    Location = x[1],
    Year = substr(x[2],3,4)
  )
}
M = ldply(str_split(file_names, "_"), f)
FileNames = cbind(fileNumber=1:40, M)

M = sim

M$Biocontrol = "Unsuccessful"
M$Biocontrol[M$yieldImprovement>yieldThreshold] = "Successful"
M$Biocontrol = factor(M$Biocontrol)

Counts = as.data.frame.matrix(t(table(M$Biocontrol, M$fileNumber)))
Counts$fileNumber = 1:40
Counts = merge(Counts, FileNames, by="fileNumber")
Counts$Success = 100*with(Counts, Successful/(Successful+Unsuccessful))
# Check: Average==10?
100*sum(Counts[,2])/sum(Counts[,2:3]) 

breaks = quantile(Counts$Success, c(0.25, 0.75))
breaks
Counts$Group = 2
Counts$Group[Counts$Success <= breaks[1]] = 1
Counts$Group[Counts$Success >  breaks[2]] = 3
Counts$Group = factor(Counts$Group)

Counts$Label = round(Counts$Success,0)
Counts$Year = factor(Counts$Year)
levels(Counts$Year) = 2000 + c(4:6, 12:18)

grey_scale3  = c("white", "grey94", "darkgrey")

make_plot = function() {

  ggplot(Counts, aes(Location, Year)) +
    geom_tile(aes(fill=Group), colour="black") +
    geom_text(aes(label=Label), size=3) +
    scale_fill_manual(values = grey_scale3) +
    labs(x="", y="") +
    theme1 +
    theme(
      axis.line = element_blank(),
      axis.ticks = element_blank(),
      legend.position = "none"
    )  
  
}

# Dimensions
W = 84
H = 80

# Screen plot
graphics.off()
open_plot_window(mm(W),mm(H))
print(make_plot())

# Write figures
write_figure = function(file_type) {
  file_name_path = paste0(output_folder, "/fig-8-bw.", file_type)
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

