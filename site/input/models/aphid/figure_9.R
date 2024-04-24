# Universal Simulator input and output folder
parent = "C:/MyDocuments/QDev/UniSim3"
input_folder  = paste0(parent, "/input")
output_folder = paste0(parent, "/output")


# Load standard script
source(paste0(input_folder, "/scripts/begin.R"))

# Load validation data
load(paste0(input_folder, "/models/aphid/validation/Stages.Rdata"))
load(paste0(input_folder, "/models/aphid/validation/A95_weather.Rdata"))
load(paste0(input_folder, "/models/aphid/validation/K95_weather.Rdata"))
load(paste0(input_folder, "/models/aphid/validation/A95_aphids.Rdata"))
load(paste0(input_folder, "/models/aphid/validation/K95_aphids.Rdata"))


show_a95 = function(title) {
  aphids = with(A95_aphids, Nymphs + AdultApterous)
  
  M = data.frame(
    Date        = A95_aphids$Date,
    Susceptible = with(A95_aphids, aphids*(100-Prevalence)/100),
    Exposed     = with(A95_aphids, aphids*Prevalence/100),
    Cadavers    = A95_aphids$Cadavers,
    Syrphids    = A95_aphids$Syrphids
  )
  
  M = melt(M, id.vars="Date")
  ggplot(M, aes(Date, value, colour=variable)) +
    geom_line() +
    geom_point() +
    scale_colour_manual(values=c(red, blue, green, pink), name="") +
    geom_vline(aes(xintercept=Date), data=subset(Stages, Field=="A95"), colour=brown, alpha=0.5, size=1) +
    labs(title=title, x="", y="Individuals per tiller") +
    facet_wrap(~variable, ncol=1, scales="free_y") +
    theme_bw(base_size=text_size) +
    theme(legend.position="none")
}

show_k95 = function(title) {
  M = data.frame(
    Date     = K95_aphids$Date,
    Aphids   = with(K95_aphids, Nymphs + AdultApterous + AdultAlate),
    Cadavers = K95_aphids$Cadavers,
    Syrphids = K95_aphids$Syrphids
  )
  M = melt(M, id.vars="Date")

  ggplot(M, aes(Date, value, colour=variable)) +
    geom_line() +
    geom_point() +
    scale_colour_manual(values=c(violet, green, pink), name="") +
    geom_vline(aes(xintercept=Date), data=subset(Stages, Field=="K95"), colour=brown, alpha=0.5, size=1) +
    labs(title=title, x="", y="Individuals per tiller") +
    facet_wrap(~variable, ncol=1, scales="free_y") +
    theme_bw(base_size=text_size) +
    theme(legend.position="none")
}

show_weather =  function(field) {
  Range = data.frame(
    Field = factor(rep(c("A95","A95","K95","K95"), 2)),
    Date  = rep(c(min(A95_weather$Date), max(A95_weather$Date), min(K95_weather$Date), max(K95_weather$Date)), 2),
    variable = c(rep("Tavg",4), rep("RHmax",4)),
    value = c(rep(c(min(weather$Tavg), max(weather$Tavg)), 2), rep(c(min(weather$RHmax), max(weather$RHmax)), 2))
  )
  Range$variable = reorder_levels(factor(Range$variable), 2:1)
  Range

  W = subset(weather, Field==field)
  W = melt(W, id.vars=c("Field","Date"))
  ggplot(W, aes(Date, value, colour=variable)) +
    geom_blank(data=subset(Range, Field==field)) +
    geom_line() +
    geom_vline(aes(xintercept=Date), data=subset(Stages, Field==field), colour=brown, alpha=0.5, size=1) +
    labs(x ="", y=" ") +
    facet_wrap(~variable, ncol=1, scales="free_y") +
    theme_bw(base_size=text_size) +
    theme(legend.position="none")
}

# Create plot
text_size = 8
A95_weather = subset(A95_weather, Date>=min(A95_aphids$Date) & Date<=max(A95_aphids$Date))
K95_weather = subset(K95_weather, Date>=min(K95_aphids$Date) & Date<=max(K95_aphids$Date))

weather = rbind(
  A95_weather,
  K95_weather
)

P = ggarrange(
  ggarrange(
    show_a95("Ågerup"),
    show_k95("Kalø"),
    ncol = 2
  ),
  ggarrange(
    show_weather("A95"),
    show_weather("K95"),
    ncol = 2,
    align = "hv"
  )
  ,
  ncol = 1, heights = c(2,1)
)

# Screen plot
print(P)

# Write figure
W = 15
H = 16
if (!dir.exists(output_folder)) dir.create(output_folder, recursive=TRUE)
ggsave(paste0(output_folder, "/fig-9-colour.png"), P, width=W, height=H, units="cm")


# Hook for batch command
figures = function(df) {
  Pages = list(Page = list(Grob=P, Width=mm(10*W), Height=mm(10*H)))
}
