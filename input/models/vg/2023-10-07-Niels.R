M = sim[, c("indoorsT", "spHeatingTemperature", "spVentilationTemperature", "actHeatingPower", "actVentilation")] #, "statusCode", "actionCode")]
M$Tick = 1:nrow(M)
M = melt(M, id.vars="Tick")

# Sp = sim[, c("spVentilationTemperature", "spHeatingTemperature")]
# Sp$Tick = 1:nrow(Sp)
# Sp = melt(M, id.vars="Tick")

P = ggplot(M, aes(Tick, value, colour=variable)) +
  geom_point() +
  scale_colour_discrete(guide = "none") +
  facet_wrap(~variable, scales="free", ncol=1)

print(P)
