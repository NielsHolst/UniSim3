plot_title = function() {
  title = output_file_name |> trimws(whitespace=".*/") 
  n = nchar(title)
  substring(title, 1, n-9)
}

ids = c("dateTime", "isDay")
vars = setdiff(colnames(sim), c(ids, "iteration", "step"))
M = melt(sim, id.vars=ids, 
         measure.vars=vars, 
         variable.name="Variable", value.name="Value")
M$isDay = factor(M$isDay)
colnames(M)[colnames(M)=="isDay"] = "Time"
levels(M$Time) = c("Night", "Day")

M$Value[M$Time=="Night" & M$Variable=="totalPar"] = NA

Plot = ggplot(M) +
  geom_density(aes(Value, group=Time, colour=Time, fill=Time), alpha=0.2) +
  scale_fill_manual(values=c("black", orange)) +
  scale_colour_manual(values=c("black", orange)) +
  facet_wrap(~Variable, scales="free", nrow=1) +
  labs(title=plot_title()) +
  theme(
    axis.text.y = element_blank(),
    axis.ticks.y = element_blank(),
    axis.title = element_blank()
  )
windows(10,2)
print(Plot)


figures <- function(df) {
  Pages = list(Page = list(Grob=Plot, Width=14.0, Height=4.0))
}
