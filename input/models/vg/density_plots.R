plot_title = function() {
  title = output_file_name |> trimws(whitespace=".*/") 
  n = nchar(title)
  substring(title, 1, n-9)
}

sim$Season = factor(sim$Season)
sim$isDay = factor(sim$isDay)
levels(sim$isDay) = c("Night", "Day")
sim$Time = sim$isDay

M = melt(sim, id.vars=c("Season", "Time", "dateTime"), 
         measure.vars=c("temperature", "rh", "totalPar", "balanceCover", "ventilation"), 
         variable.name="Variable", value.name="Value")
M$Season = reorder_levels(M$Season, 2:1)

Span = ddply(M, .(Season, Variable, Time), summarise, Median = median(Value)) |> #, MinDateTime = min(dateTime), MaxDateTime = max(dateTime)) |>
       dcast(Season+Variable~Time, value.var="Median")

Span$Ymin = with(Span, pmin(Night, Day))
Span$Ymax = with(Span, pmax(Night, Day))
Span$MinTime = "Night"
Span$MinTime[Span$Ymin == Span$Day] = "Day"
Span$MinTime = factor(Span$MinTime)

Span1 = Span2 = Span

Span1$dateTime = min(sim$dateTime[sim$Season == "Winter"])
Span1$dateTime[Span1$Season == "Summer"] = min(sim$dateTime[sim$Season == "Summer"])

Span2$dateTime = max(sim$dateTime[sim$Season == "Winter"])
Span2$dateTime[Span2$Season == "Summer"] = max(sim$dateTime[sim$Season == "Summer"])

Span = rbind(Span1, Span2)


P3 = ggplot(M) +
  geom_ribbon(aes(x=dateTime, ymin=Ymin, ymax=Ymax, fill=MinTime), data=Span, alpha=0.3) +
  scale_fill_manual(values=c("black", "yellow")) +
  geom_line(aes(dateTime, Ymin), data=Span, linewidth=0.5) +
  geom_line(aes(dateTime, Ymax), data=Span, linewidth=0.5) +
  geom_line(aes(dateTime, Value, colour=Variable)) +
  labs(title=plot_title(), x="", y="") +
  facet_wrap(Season~Variable, scales="free", nrow=2) +
  theme(
    legend.position = 'none'
  )

open_plot_window(12,4)
print(P3)
  