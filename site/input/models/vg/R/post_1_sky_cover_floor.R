print("Post-processing")
if (exists("prev_sim")) {
  ratio = function(variable, scenario, day) {
    y = subset(sim, Scenario==scenario & isDay==day)[,variable] / subset(prev_sim, Scenario==scenario & isDay==day)[,variable]
    y[!is.na(y)] |> quantile(probs=c(0.25, 0.5, 0.75)) |> round(2)*100
  }
  
  M = expand.grid(
    Variable = setdiff(colnames(sim), c("Scenario","dateTime","isDay","iteration","step")),
    Scenario = unique(sim$Scenario),
    Period = 0:1,
    stringsAsFactors = FALSE
  )
  Q = ddply(M, .(Variable, Scenario, Period), function(x) ratio(x$Variable, x$Scenario, x$Period))
  Q$Period = factor(Q$Period, labels=c("Day", "Night"))
  print(Q)
  
 } else {
  print("R remembered this as prev_sim")
  prev_sim = sim
}

# Run to remove prev_sim
# rm(prev_sim)
