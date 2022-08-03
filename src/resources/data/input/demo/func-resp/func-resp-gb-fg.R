my_plot = function(sim) {
  attacker = unique(sim$gb.attacker)
  sim$iteration = factor(sim$iteration)
  levels(sim$iteration) = paste("Attacker density =", attacker)

  M = melt(subset(sim, gb.attacker==5), id.vars=c("iteration", "prey"), measure.vars=c("demandGross", "supplyGross", "hostsAttacked"))
  levels(M$variable) = c("Demand", 
                         "G-B functional response\n(no. of prey attacked)", 
                         "F-G functional response\n(no. of hosts attacked)")

  ggplot(M) +
    geom_abline(slope=1, intercept=0) +
    geom_line(aes(x=prey, y=value, colour=variable)) +
    labs(x="Number available", y="Number attacked") +
    theme(
      legend.title = element_blank(),
      legend.key.height = unit(10,"mm")
    )
}

open_plot_window(5,2.5)
print(my_plot(sim))

figures <- function(df) {
  Pages = list(Page = list(Grob=my_plot(df), Width=5, Height=2.5))
}
