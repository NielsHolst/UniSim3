library(cowplot)

iterationColumn   = "iteration"
stepColumn        = "step"
skip_formats      = FALSE
box_script_folder = "C:/MyDocuments/QDev/UniSim3/input/models/vg"
output_file_name  = "C:/MyDocuments/QDev/UniSim3/output/2023-10-16-Niels all screens_0020.txt"
sim = read_output(output_file_name)

ix_none = which(sim$ScreenName == "none")
referenceCost = sim$heatingCost[ix_none]
sim = sim[-ix_none,]
rownames(sim) = {}
sim$ScreenName = factor(sim$ScreenName)
sim$Uinsulation = as.numeric(sim$Uinsulation)
referenceCost
summary(sim)
sim

s = sim[1:10,]
estimate_U = function(s) {
  saving = unique(s$energySaving)
  targetCost = referenceCost*(1-saving)
  U = approx(s$heatingCost, s$Uinsulation, targetCost)$y
  no_U = is.na(U) 
  if (no_U) {
    U = 0
    achievedCost = min(s$heatingCost)
    achievedSaving = 1 - achievedCost/referenceCost
  } else {
    achievedCost = targetCost
    achievedSaving = saving
  }
  result = paste0("Estimated Uinsulation = ", round(U,2))
  if (no_U)
    result = paste0(result, " (saved ", round(100*achievedSaving,0), "%)")
  label_x = 0
  label_size = 3
  P = ggplot(s, aes(Uinsulation, heatingCost)) +
    geom_line(linewidth=1, colour=blue) +
    geom_point(size=3, colour=red) +
    geom_hline(yintercept=referenceCost, colour=violet, linewidth=1, linetype="dashed") +
    annotate("segment", x=-Inf, xend=U, y=achievedCost, yend=achievedCost, colour=violet, linewidth=1) +
    annotate("segment", x=U, xend=U, y=-Inf, yend=achievedCost, colour=violet, linewidth=1) +
    annotate("label", x=label_x, y=referenceCost, label="100%", size=label_size) +
    annotate("label", x=label_x, y=achievedCost, label=paste0("-", round(100*achievedSaving,0), "%"), size=label_size) +
    labs(title=unique(s$ScreenName), subtitle=paste0("Specified energy saving ", round(100*saving,0), "%\n", result)) +
    xlim(-0.3, NA) +
    ylim(NA, referenceCost*1.02) 
    if (!no_U)
      P = P + annotate("label", x=U, y=min(s$heatingCost), label=round(U,2), size=label_size)
  P
}

# Divide list into a list of lists, each of size n, the last one of size <= n
divide_list = function(old_list, n) {
  old_length = length(old_list)
  new_length = ceiling(old_length/n)
  new_list = vector("list", length=new_length)
  for (i in 1:new_length) {
    from = (i-1)*n + 1
    to   = from + n - 1
    if (to > old_length) to = old_length
    new_list[i] = list(old_list[from:to])
  }
  new_list
}

plots = dlply(sim, .(ScreenName), estimate_U)
compiled_plots = divide_list(plots, 12)

save_plots = function(index) {
  file_name = paste0(output_file_folder(), "/calibration-", index, ".pdf")
  print(paste("Writing to", file_name))
  P = ggarrange(plotlist=compiled_plots[[index]], ncol=3, nrow=5)
  save_plot(file_name, P, ncol=3, nrow=4, base_width=3.5, base_height=3.5)
}
a_ply(1:length(compiled_plots), 1, save_plots)
