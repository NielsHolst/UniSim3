setwd("C:/Users/au152367/Documents/PAPERS/2021/aphids-fungi/sim")
source("setup.R")

graphics.off()

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
open_plot_window(mm(W),mm(H))
print(make_plot())

# Black and white for manuscript
png("output/fig-8-bw.png", width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot())
dev.off()

# Black and white for journal
cairo_ps("output/fig-8-bw.eps", width=mm(W), height=mm(H))
print(make_plot())
dev.off()
