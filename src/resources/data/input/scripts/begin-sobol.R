colour_first_order = "#66c2a5"
colour_total       = "#fc8d62"
colour_effects = c(colour_first_order, colour_total)
S = NULL

input_columns = function() {
  1:sobol_k
}

input_names = function() {
  colnames(sim)[input_columns()]
}

output_column = function(ix_output) {
  sobol_k + ix_output
}

output_names = function() {
  colnames(sim)[output_column(1):(ncol(sim)-2)]
}

rows = function(offset, nrows=NA) {
  n = if (is.na(nrows)) sobol_N else nrows
  from = offset*sobol_N + 1
  to = from + n - 1
  from:to
}

A_rows = function(nrows=NA) rows(0,nrows)
B_rows = function(nrows=NA) rows(1,nrows)
C_rows = function(i, nrows=NA) rows(i+1,nrows)
C_rows_all = function(nrows=NA) {
  v = {}
  for (i in 1:sobol_k) v = c(v, C_rows(i, nrows))
  v
}

yA = function(ix_output, nrows=NA) {
  as.matrix( sim[A_rows(nrows), output_column(ix_output)] )
}

yB = function(ix_output, nrows=NA) {
  as.matrix( sim[B_rows(nrows), output_column(ix_output)] )
}

yC = function(ix_output, nrows=NA) {
  n = if (is.na(nrows)) sobol_N else nrows 
  matrix(sim[C_rows_all(nrows), output_column(ix_output)], nrow=n, ncol=sobol_k)
}

sobol_inputs = function(ix_output, nrows=NA) {
  n = if (!is.na(nrows) & nrows>sobol_N) sobol_N else nrows 
  list(yA = yA(ix_output, n), 
       yB = yB(ix_output, n), 
       yC = yC(ix_output, n))
}

bootstrap = function(sobol_inputs) {
  rows = sample(1:sobol_N, sobol_N, TRUE)
  list(yA = matrix(sobol_inputs$yA[rows,], nrow=sobol_N), 
       yB = matrix(sobol_inputs$yB[rows,], nrow=sobol_N), 
       yC = matrix(sobol_inputs$yC[rows,], nrow=sobol_N))
}

sobol_indices = function(y) {

  sum_sq = function(a,b) {
    sum( (a-b)^2 )
  } 

  mult = function(a,b) {
    sum(a*b)/N
  } 
  
  Si = function(i) {
    (mult(yA, yC[,i]) - f02) / V
  }

  STi = function(i) {
    1 - (mult(yB, yC[,i]) - f02) / V
  }
  
  Si_Jansen = function(i) {
    1 - sum_sq(yA, yC[,i])/2/N/V
  }

  STi_Jansen = function(i) {
    sum_sq(yB, yC[,i])/2/N/V
  }

  yA = y$yA
  yB = y$yB
  yC = y$yC
  N = nrow(yA)
  
  # Use both matrices to compute f02 and V
  # A. Saltelli in email 29-11-2018: As you cannot use mixed matrices for V and f0,
  # you can either use the estimates corresponding to A or those corresponding to B or both. 
  # When you use e.g. both you double the number of elementary effects and hence you improve your estimate.
  yAB = c(yA, yB)
  f02 = mean(yAB)^2
  V = mult(yAB,yAB)/2 - f02

  rbind(
    data.frame(
      Input = input_names(),
      Measure = "FirstOrder",
      Effect  = sapply(1:sobol_k, Si_Jansen)
    )
    ,
    data.frame(
      Input = input_names(),
      Measure = "Total",
      Effect = sapply(1:sobol_k, STi_Jansen) 
    )
  )  
}

plot_against_sample_size = function() {

  f = function(combi) { 
    M = sobol_indices( sobol_inputs(combi$ix_output, combi$N) )
    M$Output = output_names()[combi$ix_output]
    M
  }
  g = function(m) {
    data.frame(Effect=sum(m$Effect))
  }

  power2 = floor(log2(sobol_N))
  from2 = max(power2-5, 1)
  breaks = 2^(from2:power2)
  if (2^power2 < sobol_N)
    breaks = c(breaks, sobol_N)
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
  
  ggplot(M, aes(x=N, y=Effect, colour=Measure)) +
    geom_line() +
    geom_point() +
    scale_x_continuous(trans="log2") +
    labs(y="Sobol' index") +
    ylim(-0.05,NA) +
    geom_hline(yintercept=0, colour="grey") +
    geom_hline(yintercept=1, colour="grey", size=1) +
    facet_grid(Output~Input) +
    guides(color=guide_legend(reverse=TRUE)) +
    scale_colour_manual(values = c(colour_first_order, colour_total)) + 
    theme_minimal() +
    theme(
      strip.background = element_rect(fill="grey90", color=NA),
      legend.position = "bottom",
      legend.title = element_blank()
    ) 
}

plot_effects = function(stats) {
  # Some inputs might have been dropped
  stats$Input = droplevels(stats$Input)
  # Re-order on total effect then on input name 
  M = subset(stats, Measure=="Total")
  n = nrow(M)
  M$Input = as.character(M$Input)
  M = M[order(M$Input),]
  M$EffectMean[M$LowerPercentile<0.001] = 0
  M = M[order(M$EffectMean, decreasing=TRUE),]
  M$NewOrder = 1:n
  M = M[c("Input", "NewOrder")]
  N = data.frame(
    Input = as.character(levels(stats$Input)),
    PrevOrder = 1:n
  )
  M = join(M,N)
  prev_order = M$PrevOrder
  stats$Input = reorder_levels(stats$Input, prev_order)
  stats$Input = reorder_levels(stats$Input, n:1)
  # Now plot
  dodge = position_dodge(width = 0.9)
  ggplot(stats, aes(x=Input, y=EffectMean, fill=Measure)) +
    geom_bar(alpha=0.3, stat="identity", position=dodge) +
    geom_hline(yintercept=0, colour="grey", size=1) +  
    geom_errorbar(aes(ymin=LowerPercentile, ymax=HigherPercentile, colour=Measure), 
                  size=2, width=0,
                  position=dodge) +
    scale_fill_manual(values=colour_effects) +
    scale_colour_manual(values=colour_effects) +
    guides(colour = guide_legend(reverse=T), fill = guide_legend(reverse=T)) +
    scale_y_continuous("Sobol' index") + #, breaks=(0:8)/4) +
    theme_classic() +
    theme(
      axis.ticks.y = element_blank(),
      axis.line.y = element_blank(),
      legend.position = "bottom",
      legend.title = element_blank()
    ) +
    labs(x="", title=paste("Sensitivity of", unique(stats$Output))) +
    coord_flip() 
}

sobol_bootstrap = function(ix_output, n) {
  f = function(i) {
    g = function(m) {
      data.frame(Effect=sum(m$Effect))
    }

    M = sobol_indices( bootstrap(inputs) )
    Msum = ddply(M, .(Measure), g)
    Msum$Input = "Sum"
    rbind(M,Msum)
  }
  
  inputs = sobol_inputs(ix_output)
  M = adply(1:n, 1, f)
  colnames(M)[1] = "Sample"
  M$Output = output_names()[ix_output]
  M
}

sobol_statistics = function(B, num_digits=2) {
  M = ddply(B, .(Input, Measure), summarize,
    EffectMean       = mean(Effect),
    EffectSE         = sum((EffectMean-Effect)^2),
    LowerPercentile  = quantile(Effect, 0.025),
    HigherPercentile = quantile(Effect, 0.975),
    P = length(which(Effect<=0))/length(Effect)
  )
  M$EffectMean = round(M$EffectMean, num_digits)
  M$EffectSE = round(sqrt(M$EffectSE/(nrow(B)-1)), num_digits+1)
  M$LowerPercentile = round(M$LowerPercentile, num_digits)
  M$HigherPercentile = round(M$HigherPercentile, num_digits)
  M$Sig = "ns"
  M$Sig[M$P<=0.05]  = "*"
  M$Sig[M$P<=0.01]  = "**"
  M$Sig[M$P<=0.001] = "***"
  M$P = round(M$P, 4)

  totals = subset(M, Measure=="Total")
  alphabetic_ordering = order(totals$Input)
  totals = totals[alphabetic_ordering, ]
  row_ordering = order(totals$EffectMean, decreasing=TRUE)
  levels_ordering = order(totals$EffectMean, decreasing=FALSE)
  M$Input = reorder_levels(factor(M$Input), levels_ordering)
    
  f = function(x) c(x,x-1)
  row_numbers = c(sapply(2*row_ordering, f))
  M = M[row_numbers,]
  M
}

plot_sobol_convergence = function() {
  plot_against_sample_size()
}

compute_sobol_indices = function() {
    n_outputs = length(output_names())
    print(paste0("Bootstrapping (n=", sobol_B, ")..."))
    B = adply(1:n_outputs, 1, sobol_bootstrap, n=sobol_B)
    S <<- ddply(B, .(Output), sobol_statistics)
    # Save S to file
    file_name_R = paste0(output_file_folder(), "/", output_file_base_name(), "-S.Rdata")
    print(paste("Writing S data frame to", file_name_R))
    save(S, file=file_name_R)
    S
}

plot_sobol_indices = function(S=NULL) {
  # Use argument S
  if (!is.null(S)) {
    S <<- S
  # Compute S
  } else {
    S = compute_sobol_indices()
  }
  print(nrow(S))
  print(colnames(S))
  plots = dlply(subset(S, Input!="Sum"), .(Output), plot_effects)
  plots
}





     