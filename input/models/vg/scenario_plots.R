plot_title = function() {
  title = output_file_name |> trimws(whitespace=".*/") 
  n = nchar(title)
  substring(title, 1, n-9)
}

sim$Season = factor(sim$Season)
sim$isDay = factor(sim$isDay)
levels(sim$isDay) = c("NightTime", "DayTime")
sim$Cycle = sim$isDay

ids = c("Season", "Cycle", "dateTime")
vars = setdiff(colnames(sim), c(ids, "isDay", "iteration", "step"))
M = melt(sim, id.vars=ids, 
         measure.vars=vars, 
         variable.name="Variable", value.name="Value")
M$Season = reorder_levels(M$Season, 2:1)
M$CycleIndex = 0


M$CycleIndex[M$Cycle=="DayTime"] = 1
M$DiffCycleIndex = c(0, diff(M$CycleIndex))

sunrises = which(M$DiffCycleIndex == 1)
sunsets  = which(M$DiffCycleIndex == -1)
n = length(sunsets)
m = n-1
Cycles = rbind(
  data.frame(
    Cycle      = "DayTime",
    Begin   = sunrises[1:m],
    End     = sunsets [1:m],
    CycleId    = 1:m
  ),
  data.frame(
    Cycle      = "NightTime",
    Begin = sunsets [1:m] + 1,
    End   = sunrises[2:n] - 1,
    CycleId    = 1:m
  )
)

M$CycleId = NA
for (i in 1:(2*m)) {
  ix = Cycles$Begin[i]:Cycles$End[i]
  day_span = date(M$dateTime[ix]) |> unique() |> length()
  if (day_span < 3)
    M$CycleId[ix] = Cycles$CycleId[i]
}


Quartiles = ddply(subset(M, !is.na(CycleId)), .(Season, CycleId, Cycle, Variable), summarise,
  From = min(dateTime),
  To = max(dateTime),
  Q1 = quantile(Value, 0.25),
  Q2 = quantile(Value, 0.50),
  Q3 = quantile(Value, 0.75)
) 

P3 = ggplot(M) +
  geom_rect(aes(xmin=From, xmax=To, ymin=Q1, ymax=Q3, fill=Cycle), alpha=0.2, data=Quartiles) +
  geom_segment(aes(x=From, xend=To, y=Q1, yend=Q1), data=Quartiles) +
  geom_segment(aes(x=From, xend=To, y=Q2, yend=Q2), data=Quartiles) +
  geom_segment(aes(x=From, xend=To, y=Q3, yend=Q3), data=Quartiles) +
  scale_fill_manual(values=c("black", orange)) +
  geom_line(aes(dateTime, Value, colour=Variable)) +
  labs(title=plot_title(), x="", y="") +
  facet_wrap(Season~Variable, scales="free", nrow=2) +
  theme(
    legend.position = 'none'
  )

open_plot_window(14,4)
print(P3)


figures <- function(df) {
  Pages = list(Page = list(Grob=P3, Width=12.0, Height=4.0))
}
