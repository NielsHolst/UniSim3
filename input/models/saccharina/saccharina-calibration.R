# print(summary(sim))
sim$calibration = as.numeric(sim$calibration)
sim = subset(sim, !is.na(calibration))

M = sim[order(sim$calibration),]
print(head(M,10))
