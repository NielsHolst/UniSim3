M = sim[, c("dateTime","floorT","indoorsT")]
M = melt(M, id.vars="dateTime")
head(M)
ggplot(M, aes(dateTime, value, colour=variable)) +
  geom_line()
  