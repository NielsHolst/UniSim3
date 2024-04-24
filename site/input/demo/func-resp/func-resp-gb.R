my_plot = function(sim) {
  sim$s = sim$searchEfficacy*sim$prey
  searchEfficacy = round(unique(sim$searchEfficacy), 2)

  sim$iteration = factor(sim$iteration)
  levels(sim$iteration) = paste("Attacker density =", paste0(unique(sim$attacker),";"), 
                                " Search efficacy =", searchEfficacy)

  M = melt(sim, id.vars=c("iteration", "prey"), measure.vars=c("supplyGross", "demandGross", "s"))
  levels(M$variable) = c("Gutierrez-Baumgaertner", "Demand", "Lotka-Volterra")
  M$variable = reorder_levels(M$variable, c(2,3,1))

  ggplot(M) +
    geom_abline(slope=1, intercept=0) +
    geom_line(aes(x=prey, y=value, colour=variable), size=1.1) +
    scale_y_continuous(breaks = 10*(0:10), limits=c(0, 1.1*max(sim$demand))) +
    labs(x="No. of prey", y="No. of prey killed") + 
    facet_wrap(~iteration, ncol=1) +
    theme(
      legend.title = element_blank()
    )
}

print(my_plot(sim))

figures <- function(df) {
  Pages = list(Page = list(Grob=my_plot(df), Width=7, Height=7))
}
