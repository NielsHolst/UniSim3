my_plot = function(sim) {
  attacker = unique(sim$attacker)
  sim$lv = sim$hostsAttacked #sim$attackRate*sim$host*sim$attacker

  sim$iteration = factor(sim$iteration)
  levels(sim$iteration) = paste("Attacker density =", attacker)


  M = melt(sim, id.vars=c("iteration", "host"), measure.vars=c("supply", "demand", "lv"))
  levels(M$variable) = c("No. of attacks", "Demand", "No. of hosts attacked")
  M$variable = reorder_levels(M$variable, c(2,1,3))

  ggplot(M) +
    geom_abline(slope=1, intercept=0) +
    geom_line(aes(x=host, y=value, colour=variable)) +
    labs(x="No. of hosts", y="No. of attacks   -or-   No. of hosts attacked") + 
    facet_wrap(~iteration, ncol=1) +
    theme(
      legend.title = element_blank()
    )
}

print(my_plot(sim))

figures <- function(df) {
  Pages = list(Page = list(Grob=my_plot(df), Width=7, Height=7))
}
