output_files = function(folder) {
  setdiff(list.files(folder, "[^.]*.txt"), "clip.txt")
}

file_id = function(file_name) {
  n = nchar(file_name)
  substring(file_name, 1, n-9)
}

read_file = function(folder, file_name) {
  sim = read_output(paste0(folder, "/", file_name))
  sim$Id = file_id(file_name)
  sim
}

#
# Main
#

# Collate data

folder = "C:/MyDocuments/QDev/UniSim3/output/buildup"
files = output_files(folder)

file_id(files[1])
sim = read_file(folder,files[1])
head(sim)

W = adply(files, 1, function(file_name) read_file(folder, file_name))[,-1]
W$Id = factor(W$Id)
summary(W)

# Melt data

ids = c("Id", "dateTime", "isDay")
vars = setdiff(colnames(W), c(ids, "iteration", "step"))
M = melt(W, id.vars=ids, 
         measure.vars=vars, 
         variable.name="Variable", value.name="Value")

# Polish day and night

M$isDay = factor(M$isDay)
colnames(M)[colnames(M)=="isDay"] = "Time"
levels(M$Time) = c("Night", "Day")
exclude = with(M, Time=="Night" & Variable=="totalPar") 
M = M[-exclude,]

# Compute densities


D = ddply(M, .(Id, Time, Variable), 
  function(m) {
    if (all(m$Time=="Night" & m$Variable=="totalPar"))
      return(NULL)
    d = density(m$Value)
    data.frame(
      X = d$x,
      Y = d$y
    )
  }
)
D2 = ddply(D, .(Id, Variable), mutate, Yscaled = Y/max(Y))

n = length(levels(D2$Id))
levels(D2$Id) = paste("Scenario", 1:n)




# Plot

ggplot(D2, aes(x=X, ymax=Yscaled)) +
  geom_ribbon(aes(group=Time, fill=Time, colour=Time), ymin=0, alpha=0.2) +
  scale_fill_manual(values=c("black", orange)) +
  scale_colour_manual(values=c("black", orange)) +
  facet_grid(Id~Variable, scales="free") +
  theme(
    axis.text.y = element_blank(),
    axis.ticks.y = element_blank(),
    axis.title = element_blank()
  )
  
