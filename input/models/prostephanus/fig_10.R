print(summary(sim))
sim$Time = sim$step
sim$adult[sim$adult==0] = NA
sim$Experiment = factor(sim$Experiment)
sim$Immigration = factor(sim$Immigration)
sim$Immigration = reorder_levels(sim$Immigration, c(2,1,3))

obs = read.table(paste0(box_script_folder, "/obs.txt"), header=TRUE, sep="\t", stringsAsFactors=TRUE)
obs$adult = 10^obs$LogDensity



plot_experiment = function(experiment) {
  ggplot(subset(sim, Experiment==experiment), aes(Time, adult)) +
    geom_line(aes(linetype=Immigration), colour=blue) +
    scale_linetype_manual(values=c("solid", "dotted", "dashed")) + 
    geom_point(colour=red, size=2, data=subset(obs, Experiment==experiment)) +
    xlim(1,NA) +
    scale_y_log10(breaks = trans_breaks("log10", function(x) 10^x), 
                  labels = trans_format("log10", math_format(10^.x))) +
    labs(
      title = experiment,
      x = "Days after stocking",
      y = "Beetles per kg grain"
    )
}

windows(7, 11)
P = ggarrange(
  plot_experiment("Mono 1992-93"), 
  plot_experiment("Mono 1993-94"),
  plot_experiment("IITA 1994-95"),
  ncol=1,
  common.legend = TRUE,
  legend = "right"
  
) |> print()

# Hook for batch command
figures = function(df) {
  Pages = list(Page = list(Grob=P, Width=7, Height=11))
}
