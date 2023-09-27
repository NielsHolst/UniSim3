ix_none = which(sim$ScreenName == "none")
referenceCost = sim$heatingCost[ix_none]
sim = sim[-ix_none,]
rownames(sim) = {}
sim$ScreenName = factor(sim$ScreenName)
sim$Uinsulation = as.numeric(sim$Uinsulation)
referenceCost
summary(sim)
sim

# s = sim[6:12,]
estimate_U = function(s) {
  saving = unique(s$saving)
  targetCost = referenceCost*(1-saving)
  U = approx(s$heatingCost, s$Uinsulation, targetCost)$y
  label_x = 0
  label_size = 3
  P = ggplot(s, aes(Uinsulation, heatingCost)) +
    geom_line(linewidth=1, colour=blue) +
    geom_point(size=3, colour=red) +
    geom_hline(yintercept=referenceCost, colour=violet, linewidth=1, linetype="dashed") +
    annotate("segment", x=-Inf, xend=U, y=targetCost, yend=targetCost, colour=violet, linewidth=1) +
    annotate("segment", x=U, xend=U, y=-Inf, yend=targetCost, colour=violet, linewidth=1) +
    annotate("label", x=label_x, y=referenceCost, label="100%", size=label_size) +
    annotate("label", x=U, y=min(s$heatingCost), label=round(U,2), size=label_size) +
    labs(title=unique(s$ScreenName), subtitle=paste0("Documented energy saving ", round(100*saving,0), "%\nEstimated Uinsulation = ", round(U,2))) +
    xlim(-0.3, NA) +
    ylim(NA, referenceCost*1.02) 
  if (is.na(U))
    P = P + geom_hline(yintercept=targetCost, colour=violet, linewidth=1)
  P = P + annotate("label", x=label_x, y=targetCost, label=paste0(round(100*(1-saving),0), "%"), size=label_size)
  P
}

plots = dlply(sim, .(ScreenName), estimate_U)
print(ggarrange(plotlist=plots, align="hv"))
