date_col = 1
zero_cols = which(colSums(sim[,2:ncol(sim)]) == 0) + 1
counter_cols = ncol(sim) - 0:1
all_cols = 1:ncol(sim)
keep_cols = setdiff(all_cols, c(zero_cols, counter_cols))

M = sim[, keep_cols]

to_sum = str_detect(colnames(M), "wf") & (colnames(M) != "wfAh")

M$wfSum = rowSums(M[, to_sum])
M$wfBalance = with(M, wfSum - wfAh)

Melted = melt(M, id.vars="dateTime")
Melted$Date = date(Melted$dateTime)
Daily = ddply(Melted, .(Date, variable), summarise, Avg=mean(value))

windows()
Q = ggplot(Daily, aes(Date, Avg, colour=variable)) +
  geom_line() +
  # scale_x_date(breaks="1 month", date_labels = "%b") +
  labs(
    x = "",
    y = "Water flux (g/m2/min)"
  ) +
  facet_wrap(~variable, scales="free", ncol=2)
print(Q)
