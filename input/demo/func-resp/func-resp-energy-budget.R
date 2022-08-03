my_plot = function(sim) {

  sim$iteration = factor(sim$iteration)
  levels(sim$iteration) = paste("Attacker density =", unique(sim$attacker))

  M = melt(sim, id.vars=c("iteration", "prey"), 
           measure.vars=c("demandGross", "supplyGross", "supplyNet", "egestedAmount", "demandResp", "conversionCostAmount"))
  levels(M$variable) = c("Gross demand", "Gross supply", "Net supply", "Egested", "Respired (base)", "Respired (conversion)")

  ggplot(M) +
    geom_abline(slope=1, intercept=0) +
    geom_line(aes(x=prey, y=value, colour=variable)) +
    scale_y_continuous(breaks = 10*(0:10), limits=c(NA, 1.05*max(sim$demandGross))) +
    labs(x="No. of prey", y="No. of prey killed") + 
    facet_wrap(~iteration, ncol=1) +
    theme(
      legend.title = element_blank()
    )
}

open_plot_window(5,7)
print(my_plot(sim))

figures <- function(df) {
  Pages = list(Page = list(Grob=my_plot(df), Width=5, Height=7))
}
