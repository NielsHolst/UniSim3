sim = subset(sim, date>dmy("14/5/1995") & date<dmy("16/8/1995"))
sim$Nymphs = with(sim, nymphSusc+nymphExp)
sim$Adults = with(sim, adultSusc+adultExp)
S = melt(sim, id.vars=c("iteration","date"), measure.vars=c("Nymphs","Adults","cadavers"))
summary(S)

setwd(box_script_folder)
load("validation/Stages.Rdata")
load("validation/K95_aphids.Rdata")
obs = K95_aphids

O = melt(obs, id.vars="Date", measure.vars=c("Nymphs","AdultApterous","Cadavers"))
summary(O)

levels(O$variable) =
levels(S$variable) = c("Nymphs", "Adults", "Cadavers")
colnames(S)[2] = "Date"
S$Date = as.Date(S$Date)

open_plot_window(8,8)
show = function(do_log) {
  S2 = S
  O2 = O
  if (do_log) {
    S2$value = log10(S2$value)
    S2$value[S2$value < -5] = NA
    O2$value = log10(O2$value)
    O2$value[!(is.na(O$value)) & O$value==0] = NA
  } 
  P = ggplot(S2, aes(Date, value)) +
    geom_line(aes(group=iteration), colour=blue, alpha=0.3) +
    geom_point(colour=red, size=1.5, data=O2) +
    geom_vline(aes(xintercept=Date), data=subset(Stages, Field=="K95"), colour=brown, alpha=0.5, size=1) +
    facet_wrap(~variable, scales="free", nrow=1) +
    theme_bw(base_size=8) 
  if (do_log) {
    P + 
    labs(x="", y="Individuals per tiller (log10)", subtitle="Log-transformed") +
    ylim(-5,NA) 
  } else {
    P +
    labs(x="", y="Individuals per tiller", title="Kalø", subtitle=subtitle)
  }
}

Q = ggarrange(
      show(FALSE),  
      show(TRUE),
      ncol = 1
)

print(Q)
ggsave("fig13.png", Q, width=15 , height=8, units="cm")
