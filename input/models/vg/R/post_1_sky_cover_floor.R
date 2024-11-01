print("Post-processing")
if (exists("prev_sim")) {

  ratio = function(variable, season, day) {
    y = subset(sim, Season==season & isDay==day)[,variable] / subset(prev_sim, Season==season & isDay==day)[,variable]
    y[!is.na(y)] |> quantile(probs=c(0.25, 0.5, 0.75)) |> round(2)*100
  }
  
  M = expand.grid(
    Variable = setdiff(colnames(sim), c("Season","dateTime","isDay","iteration","step")),
    Season = c("Summer", "Winter"),
    Period = 0:1,
    stringsAsFactors = FALSE
  )
  Q = ddply(M, .(Variable, Season, Period), function(x) ratio(x$Variable, x$Season, x$Period))
  Q$Period = factor(Q$Period, labels=c("Day", "Night"))
  print(Q)
  
 } else {
  print("R remembered this as prev_sim")
  prev_sim = sim
}

# Run to remove prev_sim
# rm(prev_sim)
