sim = read_output(output_file_name)
sim = sim[,-1]
st = c("Vegetative", "Reproductive", "Ripening")
colnames(sim)=
  c("Step", "DayLength", st, "Tavg", st)
  
M1 = melt(sim[,c(1,3:5)],  id.vars=1, value.name="Simulated", variable.name="Stage")
M2 = melt(sim[,c(1,7:9)], id.vars=1, value.name="Observed")
M = cbind(M1, M2$Observed)
colnames(M)[4] = "Observed"
head(M,30)

P = 
ggplot(M,aes(x=Step)) + 
  geom_point(aes(y=Observed, colour=Stage), size=3) +
  geom_line(aes(y=Simulated, colour=Stage), size=1) +
  geom_line(aes(y=DayLength/24), data=sim) +
  geom_line(aes(y=Tavg/30), data=sim, colour="orange", size=1)

print(P)
