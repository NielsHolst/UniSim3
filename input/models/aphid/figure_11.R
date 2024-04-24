# Arrange simulation output
sim = subset(sim, date>dmy("14/5/1995") & date<dmy("16/8/1995"))
sim$Nymphs = with(sim, nymphSusc+nymphExp)
sim$Adults = with(sim, adultSusc+adultExp)
S = melt(sim, id.vars=c("iteration","date"), measure.vars=c("Nymphs","Adults","cadavers"))
summary(S)

# Load validation data
load(paste0(box_script_folder, "/validation/Stages.Rdata"))
load(paste0(box_script_folder, "/validation/K95_aphids.Rdata"))
obs = K95_aphids

# Arrange validation data
O = melt(obs, id.vars="Date", measure.vars=c("Nymphs","AdultApterous","Cadavers"))
summary(O)

# Reconcile columns
levels(O$variable) =
levels(S$variable) = c("Nymphs", "Adults", "Cadavers")
colnames(S)[2] = "Date"
S$Date = as.Date(S$Date)

# Create plot
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

# Dimensions
W = 160
H = 90

# Screen plot
graphics.off()
open_plot_window(mm(W),mm(H))
print(Q)

# Write plot to file
ggsave(paste0(output_file_folder(), "/fig-11-colour.png"), Q, width=W , height=H, units="mm")

# Hook for batch command
figures = function(df) {
  Pages = list(Page = list(Grob=P, Width=7.0, Height=7.0))
}
