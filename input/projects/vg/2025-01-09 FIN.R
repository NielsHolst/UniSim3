date_col = 1
zero_cols = which(colSums(sim[,2:ncol(sim)]) == 0) + 1
counter_cols = ncol(sim) - 0:1
all_cols = 1:ncol(sim)
keep_cols = setdiff(all_cols, c(zero_cols, counter_cols))

M = sim[, keep_cols]

wf_cols = str_detect(colnames(M), "wf")
to_sum = wf_cols & (colnames(M) != "wfAh")
M$wfSum = rowSums(M[, to_sum])
M$wfBalance = with(M, wfSum - wfAh)

Melted = melt(M, id.vars="dateTime")


windows()
Q = ggplot(Melted, aes(dateTime, value, colour=variable)) +
  geom_line() +
  labs(
    x = "",
    y = "Water flux (kg/m2/h)"
  ) +
  facet_wrap(~variable, scales="free", ncol=2)
print(Q)



ggplot(sim, aes(transpiration, wfTranspiration)) +
  geom_point(aes(colour=factor(subSteps)))
  
