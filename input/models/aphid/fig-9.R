setwd("C:/Users/au152367/Documents/PAPERS/2021/aphids-fungi/sim")
source("setup.R")

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
      xlim(0,5) +
      labs(x="Cadaver prevalence (%) in GS 43", y="") + 
      theme1 +
      theme(
        legend.position = "none",
        axis.text.y = element_blank(),
        axis.ticks.y = element_blank()
      )
    ,
    ggplot(M, aes(percentageCadaversGs61, fill=Biocontrol)) +
      scale_fill_manual(values=grey_scale2) +
      geom_density(alpha=0.8, size=0.3) +
      xlim(0,10) +
      labs(x="Cadaver prevalence (%) in GS 61", y="Probability density") +
      theme1 +
      theme(
        legend.position = "none",
        axis.text.y = element_blank(),
        axis.ticks.y = element_blank()
      )
    ,
    ggplot(M, aes(percentageCadaversGs73, fill=Biocontrol)) +
      scale_fill_manual(values=grey_scale2) +
      geom_density(alpha=0.8, size=0.3) +
      xlim(0,80) +
      labs(x="Cadaver prevalence (%) in GS 73", y="") + 
      theme1 +
      theme(
        legend.position = "none",
        axis.text.y = element_blank(),
        axis.ticks.y = element_blank()
      )
    ,
    ncol=1, align="hv"
  )
}

# Dimensions
W = 84
H = 110

# Screen plot
open_plot_window(mm(W),mm(H))
print(make_plot())

# Black and white for manuscript
png("output/fig-9-bw.png", width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot())
dev.off()

# Black and white for journal
cairo_ps("output/fig-9-bw.eps", width=mm(W), height=mm(H))
print(make_plot())
dev.off()
