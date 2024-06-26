# Load library for gam regressions
library(mgcv)

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

meltxy = function(df, xvars, yvars) {
  M = df[,c("iteration", xvars,yvars)]
  M = melt(M, measure.vars=xvars, value.name="xValue", variable.name="xVariable")
  M = melt(M, id.vars=c("iteration","xValue","xVariable"), measure.vars=yvars, value.name="ResponseValue", variable.name="Response")
  M
}
# xvars = c("transmissionEfficiency","lethalTime","sporulationOn","timeAcceleration","cadaverDuration","propExposedImmigrants","immunityCost")
xvars = c("sporulationOn","lethalTime","transmissionEfficiency","timeAcceleration","cadaverDuration","propExposedImmigrants","immunityCost")
yvars = c("yieldImprovement","maxPrevalence","maxCadaverPrevalence")
M = meltxy(sim, xvars, yvars)

my_scale = function(x) {
  xmin = max(0,min(x))
  xmax = max(x)
  (x-xmin)/(xmax -xmin)
}

f = function(m) {
  m$xValueScaled = my_scale(m$xValue)
  model = gam(ResponseValue ~ s(xValueScaled, bs="cs"), data=m)
  M = data.frame(
    xValueScaled = (0:100)/100
  )
  M$ResponseValue = predict(model,M)
  M
}
g = function(m) {
  M = m
  M$ResponseValueScaled = my_scale(M$ResponseValue)
  M
}

M = ddply(M, .(xVariable,Response), f)
ddply(M, .(Response), summarize, Min=min(ResponseValue), Max=max(ResponseValue))
M = ddply(M, .(Response), g)
ddply(M, .(Response), summarize, Min=min(ResponseValueScaled), Max=max(ResponseValueScaled))

levels(M$xVariable) = c("Humidity\nthreshold", "Lethal\ntime", "Transmiss.\nefficiency", "Time\nacceleration","Cadaver\nduration", "Exposed\nimmigrants", "Immunity\ncost")
levels(M$Response) = c("Yield\nimprovement\n","Peak exposed\nprevalence\n","Peak cadaver\nprevalence\n")

M$Response = reorder_levels(M$Response, c(2,3,1)) 

make_plot = function() {
  ggplot(M, aes(xValueScaled, ResponseValueScaled)) +
    geom_line(linewidth=0.6) +
    geom_area(alpha=0.2, fill="black") +
    labs(x="",y="") +
    xlim(0,1) +
    ylim(0,1) +
    facet_grid(Response~xVariable, switch="both") +
    theme1 +
    theme(
      strip.background = element_blank(),
      strip.text.y.left = element_text(angle=0),
      axis.line = element_blank(),
      axis.text = element_blank(),
      axis.ticks = element_blank(),
      panel.grid = element_blank(),
      plot.margin = margin(10,10,10,0)
    )
}

W = 174
H = 80

# Screen plot
graphics.off()
open_plot_window(mm(W),mm(H))
print(make_plot())

# Write figures
write_figure = function(file_type) {
  file_name_path = paste0(output_folder, "/fig-6-bw.", file_type)
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
# Figure for manuscript id also used for journal after labels have been added in PowerPoint
write_figure("png")


# Hook for batch command
figures = function(df) {
  Pages = list(Page = list(Grob=make_plot(), Width=mm(W), Height=mm(H)))
}
