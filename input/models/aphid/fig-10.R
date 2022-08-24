# Load your S data
S_data_file = "~/sites/ecolmod3/code/biocontrol-model-sa_0023-S.Rdata"

# Load standard script
source("~/QDev/UniSim3/input/scripts/begin.R")
source("D:/Documents/QDev/UniSim3/input/scripts/begin-sobol.R")

# Output folder
output_folder = "~/QDev/UniSim3/output"

# Here goes
load(S_data_file)
dim(S)

# Prepare analysis
sobol_k = length(unique(S$Input)) - 1
sobol_N = log2(nrow(sim)/(sobol_k+2))
iterationColumn = "iteration"
stepColumn      = "step"

# sobol_k = 11
# sobol_N = nrow(sim)/(sobol_k+2)

# input_columns = function() {
  # 2:(sobol_k+1)
# }

# input_names = function() {
  # s = colnames(sim)[input_columns()]
  # s = str_replace(s, "\\.end", "")
  # str_replace(s, "\\.value", "")
# }

# output_column = function(ix_output) {
  # sobol_k + ix_output + 1
# }

# output_names = function() {
  # names = colnames(sim)[output_column(1):ncol(sim)]
  # names[names!="Outcome"]
# }

# rows = function(offset, nrows=NA) {
  # n = if (is.na(nrows)) sobol_N else nrows
  # from = offset*sobol_N + 1
  # to = from + n - 1
  # from:to
# }

# A_rows = function(nrows=NA) rows(0,nrows)
# B_rows = function(nrows=NA) rows(1,nrows)
# C_rows = function(i, nrows=NA) rows(i+1,nrows)

# C_rows_all = function(nrows=NA) {
  # v = {}
  # for (i in 1:sobol_k) v = c(v, C_rows(i, nrows))
  # v
# }

# yA = function(ix_output, nrows=NA) {
  # as.matrix( sim[A_rows(nrows), output_column(ix_output)] )
# }

# yB = function(ix_output, nrows=NA) {
  # as.matrix( sim[B_rows(nrows), output_column(ix_output)] )
# }

# yC = function(ix_output, nrows=NA) {
  # n = if (is.na(nrows)) sobol_N else nrows 
  # matrix(sim[C_rows_all(nrows), output_column(ix_output)], nrow=n, ncol=sobol_k)
# }

# sobol_inputs = function(ix_output, nrows=NA) {
  # n = if (!is.na(nrows) & nrows>sobol_N) sobol_N else nrows 
  # list(yA = yA(ix_output, n), 
       # yB = yB(ix_output, n), 
       # yC = yC(ix_output, n))
# }

# sobol_indices = function(y) {

  # sum_sq = function(a,b) {
    # sum( (a-b)^2 )
  # } 

  # mult = function(a,b) {
    # sum(a*b)/N
  # } 
  
  # Si = function(i) {
    # (mult(yA, yC[,i]) - f02) / V
  # }

  # STi = function(i) {
    # 1 - (mult(yB, yC[,i]) - f02) / V
  # }
  
  # Si_Jansen = function(i) {
    # 1 - sum_sq(yA, yC[,i])/2/N/V
  # }

  # STi_Jansen = function(i) {
    # sum_sq(yB, yC[,i])/2/N/V
  # }

  # yA = y$yA
  # yB = y$yB
  # yC = y$yC
  # N = nrow(yA)
  
  # # Use both matrices to compute f02 and V
  # # A. Saltelli in email 29-11-2018: As you cannot use mixed matrices for V and f0,
  # # you can either use the estimates corresponding to A or those corresponding to B or both. 
  # # When you use e.g. both you double the number of elementary effects and hence you improve your estimate.
  # yAB = c(yA, yB)
  # f02 = mean(yAB)^2
  # V = mult(yAB,yAB)/2 - f02

  # rbind(
    # data.frame(
      # Input = input_names(),
      # Measure = "FirstOrder",
      # Effect  = sapply(1:sobol_k, Si_Jansen)
    # )
    # ,
    # data.frame(
      # Input = input_names(),
      # Measure = "Total",
      # Effect = sapply(1:sobol_k, STi_Jansen) 
    # )
  # )  
# }


make_plot = function() {

  f = function(combi) { 
    M = sobol_indices( sobol_inputs(combi$ix_output, combi$N) )
    M$Output = output_names()[combi$ix_output]
    M
  }
  g = function(m) {
    data.frame(Effect=sum(m$Effect))
    }

  breaks = 2^(9:log2(sobol_N))
  combinations = expand.grid(ix_output=1:length(output_names()), N=breaks)
  M = adply(combinations, 1, f)[,-1]

  Msum = ddply(M, .(Output, Measure, N), g)
  Msum$Input = "Sum"
  M = rbind(M,Msum)

  totals = subset(M, Measure=="Total" & Output==output_names()[1] & N==max(N))
  alphabetic_ordering = order(totals$Input)
  totals = totals[alphabetic_ordering, ]
  effect_ordering = order(totals$Effect, decreasing=TRUE)
  M$Input = reorder_levels(factor(M$Input), effect_ordering)
  M$Output = factor(M$Output)
  M$Measure = factor(M$Measure)
  # Show only relevant variables 
  M = subset(M, Input=="Sum" & Output %in% c("yieldImprovement","maxPrevalence","maxCadaverPrevalence"))
  M$Output = droplevels(M$Output)
  M$Output = reorder_levels(factor(M$Output), c(2,1,3))
  levels(M$Output) = c("Peak exposed\nprevalence","Peak cadaver\nprevalence","Yield\nimprovement")
  levels(M$Measure) = c("First\norder","Total")

  ggplot(M, aes(x=N, y=Effect, colour=Measure)) +
    geom_line() +
    geom_point() +
    scale_x_continuous(trans="log2", breaks=breaks, labels = scales::trans_format("log2", scales::math_format(2^.x))) +
    labs(x="Sample size (N)", y="Sobol' index") +
    ylim(-0.05,NA) +
    geom_hline(yintercept=0, colour="grey") +
    geom_hline(yintercept=1, colour="grey", size=1) +
    facet_grid(~Output) +
    guides(color=guide_legend(reverse=TRUE)) +
    scale_colour_manual(values = c(blue, red)) + 
    theme_bw() 
}

W = 174
H = 80

# Screen plot
open_plot_window(mm(W),mm(H))
print(make_plot())


# Black and white for manuscript
png("output/fig-10-bw.png", width=W, height=H, units="mm", res=1200, type="cairo-png")
print(make_plot())
dev.off()
