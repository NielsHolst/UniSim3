# Load your S data
S_data_file = "~/sites/ecolmod3/code/biocontrol-model-sa_0023-S.Rdata"

# Load standard script
source("~/QDev/UniSim3/input/scripts/begin.R")

# Output folder
output_folder = "~/QDev/UniSim3/output"

# Here goes
load(S_data_file)
dim(S)

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

pick_response = function(response) {
  s = subset(S, Output==response & !(Input %in% c("Sum", "k", "immunityCost")))
  s$Input = droplevels(s$Input)

  # Re-order on total effect then on input name 
  M = subset(s, Measure=="Total")
  n = nrow(M)
  M$Input = as.character(M$Input)
  M = M[order(M$Input),]
  M$EffectMean[M$LowerPercentile<0.001] = 0
  M = M[order(M$EffectMean, decreasing=TRUE),]
  M$NewOrder = 1:n
  M = M[c("Input", "NewOrder")]
  N = data.frame(
    Input = as.character(levels(s$Input)),
    PrevOrder = 1:n
  )
  M = join(M,N,by="Input")
  prev_order = M$PrevOrder
  s$Input = reorder_levels(s$Input, prev_order)
  s$Input = reorder_levels(s$Input, n:1)
  
  # Show error bars above zero
  s$LowerPercentile[s$LowerPercentile<0] = 0
  s
}

sub_plot = function(response, top_margin=10) {
  s = pick_response(response)
  
  # Show y-axis labels
  M = data.frame(
    Response = rev(levels(s$Input))
  )
  colnames(M) = response
  
  # Now plot
  col_width = 0.7
  dodge = position_dodge(width = col_width)
  ggplot(s, aes(x=Input, y=EffectMean, fill=Measure)) +
    geom_col(position=dodge, width=col_width) +
    geom_errorbar(aes(ymin=LowerPercentile, ymax=HigherPercentile), 
                  size=0.3, width=0, colour="black",
                  position=dodge) +
    scale_fill_manual(values=grey_scale2) +
    guides(colour = guide_legend(reverse=T), fill = guide_legend(reverse=T)) +
    scale_y_continuous(limits=c(0,NA), expand=c(0,0)) +
    labs(x="", y="") +
    coord_flip() +
    theme1 +
    theme(
      legend.position = "none",
      axis.ticks.y = element_blank(),
      axis.text.y = element_blank(),
      plot.margin = margin(top_margin,10,10,20)    
    ) 
}

make_plot = function() {
  ggarrange(
    sub_plot("maxPrevalence", 20),
    sub_plot("maxCadaverPrevalence"),
    sub_plot("yieldImprovement"),
    ncol=1, align="hv", labels="auto", label.y=1.02
  )
}

make_table = function() {
  rbind(
    pick_response("maxPrevalence"),
    pick_response("maxCadaverPrevalence"),
    pick_response("yieldImprovement")
  )
}

# Dimensions
W = 84
H = 200

# Screen plot
open_plot_window(mm(W),mm(H))
print(make_plot())

# Black and white for manuscript, also used for journal after labels have been added in PowerPoint
file_name_path = paste0(output_folder, "/fig-6-bw.png")
png(file_name_path, width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot())
dev.off()
print(paste("Figure written to", file_name_path))

# Write table with figure data
file_name_path = paste0(output_folder, "/fig-6-bw-table.txt")
write.table(make_table(), file_name_path, quote=FALSE, sep="\t", row.names=FALSE)



