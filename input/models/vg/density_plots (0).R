plot_title = function() {
  title = output_file_name |> trimws(whitespace=".*/") 
  n = nchar(title)
  substring(title, 1, n-9)
}

sim$Season = factor(sim$Season)
sim$isDay = factor(sim$isDay)
levels(sim$isDay) = c("Night", "Day")

Lim = data.frame(
  Season = c("Winter", "Winter", "Summer", "Summer"),
  isDay  = c("Night", "Day", "Night", "Day"),
  dateTime    = rep(sim$dateTime[1], 4),
  temperature = rep(c(0, 40), 2),
  # ah          = rep(c(0, 0.02), 2),
  rh          = rep(c(0, 100), 2),
  totalPar    = rep(c(0, 2000), 2),
  balanceConv = rep(c(-150, 100), 2),
  balanceLw   = rep(c(-300, -100), 2),
  ventilation = rep(c(-100,500), 2)
)

M = melt(sim, id.vars=c("Season", "isDay", "dateTime"), 
         measure.vars=c("temperature", "rh", "totalPar", "balanceConv", "balanceLw", "ventilation"), 
         variable.name="Variable", value.name="Value")
M2 = melt(Lim, id.vars=c("Season", "isDay", "dateTime"), 
         measure.vars=c("temperature", "rh", "totalPar", "balanceConv", "balanceLw", "ventilation"), 
         variable.name="Variable", value.name="Value")

# P1 = ggplot(M, aes(Value)) +
  # geom_density(aes(colour=Season, fill=Season), alpha=0.3) +
  # labs(title=plot_title(), x="", y="") +
  # facet_wrap(~Variable, scales="free") +
  # theme(
    # axis.text.y = element_blank(),
    # axis.ticks.y  = element_blank(),
    # panel.grid  = element_blank()
  # )

P2 = ggplot(M, aes(Season, Value)) +
  geom_blank(aes(colour=Season, fill=Season), data=M2) +
  geom_boxplot(aes(colour=Season, fill=Season), alpha=0.3) +
  scale_colour_manual(values = c(red, blue)) +
  scale_fill_manual(values = c(red, blue)) +
  labs(title=plot_title(), x="", y="") +
  coord_flip() +
  facet_grid(isDay~Variable, scales="free") +
  theme(
    legend.position = 'none'
  )
  
P3 = ggplot(M, aes(dateTime, Value, colour=Variable)) +
  geom_line() +
  labs(title=plot_title(), x="", y="") +
  facet_wrap(Season~Variable, scales="free", nrow=2) +
  theme(
    legend.position = 'none'
  )

open_plot_window(12,4)
print(P3)
  
open_plot_window(12,3)
print(P2)
  
  