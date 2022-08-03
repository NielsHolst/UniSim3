sim = read_output(output_file_name)

graphics.off()
bw = c("white", "grey", "black")
colours = c(green, blue, violet)

Acols = c("cropGrowthStage", "susceptible", "exposed", "cadavers")
A = melt(sim[,Acols], .(cropGrowthStage), value.name="Density", variable.name="Population")

Pcols = c("cropGrowthStage", "prevalence", "cadaverPrevalence")
P = melt(sim[,Pcols], .(cropGrowthStage), value.name="Prevalence", variable.name="Type")

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


make_plot = function(size1, size2, colours) {
  alpha = if (colours==bw) 1 else 0.3
  ggarrange(
    ggplot(A, aes(x=cropGrowthStage)) +
      geom_area(aes(y=Density, fill=Population), colour="black", size=0.1, alpha=alpha) +
      geom_line(aes(y=uncontrolled), size=size1, linetype="dashed", data=sim) +
      scale_fill_manual(values=colours) +
      labs(x="", y="Aphids per tiller") +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme1 +
      theme (
        legend.position = "none"
      ) 
    ,
    ggplot(subset(P, cropGrowthStage<80) , aes(x=cropGrowthStage, Prevalence, colour=Type)) +
      geom_line(size=size2) +
      scale_colour_manual(values=colours[2:3]) +
      labs(x="Crop growth stage", y="Prevalence (%)") +
      scale_x_continuous(breaks = 10*(3:9), limits=c(30,90)) +
      theme1 +
      theme (
        legend.position = "none"
      ) 
    ,
    ncol=1, align="hv", labels="auto"
  )
}

# Dimensions
mm = function(x) x/25.4
W = 84
H = 110

# Screen plots in colours and grey scale
open_plot_window(mm(W),mm(H))
print(make_plot(1,1,colours))

open_plot_window(mm(W),mm(H))
print(make_plot(1,1,bw))

# File destination
setwd(output_file_folder())

# Black and white for manuscript
png("fig-2-bw.png", width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot(0.3,1,bw))
dev.off()

# Black and white for journal
cairo_ps("fig-2-bw.eps", width=mm(W), height=mm(H))
print(make_plot(0.3,1,bw))
dev.off()

