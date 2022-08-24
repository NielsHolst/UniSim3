# Load your sim data
sim_data_file = "~/sites/ecolmod3/code/biocontrol-model-sa_0023.Rdata"

# Load standard script
source("~/QDev/UniSim3/input/scripts/begin.R")

# Output folder
output_folder = "~/QDev/UniSim3/output"

# Here goes
load(sim_data_file)
dim(sim)

# Common yield threshold
yieldThreshold   = unname(quantile(sim$yieldImprovement,0.90))
thresholdLabel = paste0(round(yieldThreshold,1), "%-points")
print(paste("90% fractile of yield improvement =", thresholdLabel))

# Two-dimensional stratified sampling
# Example: stratify2d(x, y, 32, 5) will stratify x and y both into 32 strata, 
# providing 32^2=1024 quadrants from which 5 numbers will be picked from each,
# resulting in 5120 rows
stratify2d = function(x,y,num_strata,sample_size) {
  sample_rows = function(m) {
    n = nrow(m)
    if (sample_size>n) stop(paste("Sample size", sample_size, "is larger than group size", n))
    m[sample(1:n, sample_size),]
  }

  N = length(x)
  
  M = data.frame(
    index = 1:N,
    x = x,
    y = y
  )
  M = M[order(M$x),]

  cuts = c(0, round(N*(1:num_strata)/num_strata,0))
  cuts

  W = {}
  for (i in 1:num_strata) {
    interval = (cuts[i]+1):cuts[i+1]
    m = M[interval,]
    interval_ordered = order(m$y)
    m_ordered = m[interval_ordered,]
    group_size = N/num_strata^2
    m_ordered$group = floor(0:(nrow(m_ordered)-1)/group_size)
    W = rbind(W, ddply(m_ordered, .(group), sample_rows))
  }
  W$index
}

prevalence_lab = "Peak exposed prevalence (%)"
cadaver_lab = "Peak cadaver prevalence (%)"

successful =  "More successful biocontrol"
unsuccessful = "Less successful biocontrol"
sim$Outcome = unsuccessful
sim$Outcome[sim$yieldImprovement > yieldThreshold] = successful
sim$Outcome = factor(sim$Outcome)

sub_plot = function(x,y,ylab,ylim) {
  # Sub-sample 32^2*5 = 5,120 rows
  sub_sample = stratify2d(sim[,x], sim[,y], 32, 5)
  M = sim[sub_sample,]
  M$Outcome = reorder_levels(M$Outcome, 2:1)

  ggplot(M, aes_string(x,y)) +
    geom_point(alpha=1, shape=16, size=0.3) +
    scale_x_continuous(breaks = 5*(0:20), name="Crop growth stage") +
    scale_y_continuous(limits=ylim,       name=ylab) +
    facet_wrap(~Outcome, ncol=1, scales="free_y") +
    theme_classic() +
    theme(
      strip.background = element_rect(colour=NA, fill="#D0D0D0"),
      legend.position = "none"
    )
}

make_plot = function() {
  ggarrange(
    sub_plot("maxPrevalenceGS", "maxPrevalence", prevalence_lab,  c(NA,NA)),
    sub_plot("maxCadaverPrevalenceGS", "maxCadaverPrevalence", cadaver_lab,  c(0,100)),
    ncol=1, align="hv", labels="auto"
  )
}


# Dimensions
W = 84
H = 150

# Screen plot
open_plot_window(mm(W),mm(H))
print(make_plot())

# Black and white for manuscript
file_name_path = paste0(output_folder, "/fig-5-bw.png")
png(file_name_path, width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot())
dev.off()
print(paste("Figure written to", file_name_path))

# Black and white for journal
file_name_path = paste0(output_folder, "/fig-5-bw.eps")
cairo_ps(file_name_path, width=mm(W), height=mm(H))
print(make_plot())
dev.off()
print(paste("Figure written to", file_name_path))



