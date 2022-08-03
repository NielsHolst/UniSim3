sim = read_output(output_file_name)
# plot_all(sim)

file_name = paste(box_script_folder, "env-AnGiang.txt", sep="/")
obs = read.table(file_name, header=TRUE, stringsAsFactors=FALSE)
obs$Date = ymd(obs$Date)
ix = colnames(obs)=="Adult"
colnames(obs)[ix] = "ObservedAdult"
head(obs)

sim$date = as.Date(sim$date)
ix = colnames(sim)=="date"
colnames(sim)[ix] = "Date"
ix = colnames(sim)=="adult"
colnames(sim)[ix] = "SimulatedAdult"
head(sim)

class(obs$Date)
class(sim$Date)

M = merge(obs, sim[,c("iteration","Date","SimulatedAdult","ImmigrantFecundity","AdultReproduction")], by="Date")
M = M[order(M$iteration, M$Date),]
ix = which(M$SimulatedAdult==0)
M = M[-ix,]

one_to_one_plot = function() {
  n = M[,c("ObservedAdult","SimulatedAdult")]
  XY = data.frame(
    Span = c(min(n), max(n))
  )

  P =
  ggplot(M, aes(x=ObservedAdult, y=SimulatedAdult)) +
    geom_abline() +
    geom_blank(aes(x=Span, y=Span), data=XY) +
    scale_x_continuous(trans="log10") +
    scale_y_continuous(trans="log10") 

  grid.arrange(
    P + geom_point(aes(colour=Week)) 
    ,
    P + geom_point(aes(colour=factor(Year))) 
    ,
    P + geom_point(aes(colour=factor(Sequence))) 
    ,
    ncol=1
  )
}
# one_to_one_plot()

pc1 = function(x,y) {
  xmean = mean(x)
  ymean = mean(y)
  slope = sd(y) / sd(x)
  c(ymean - slope*xmean, slope)
}
# pc1(log10(M$ObservedAdult), log10(M$SimulatedAdult))

discrepancy = function(M) {
  model = pc1(log10(M$ObservedAdult), log10(M$SimulatedAdult))
  data.frame(Discrepancy = sum((log10(M$SimulatedAdult) - log10(M$ObservedAdult))^2), 
             DiscrepancyLin = log(sum((M$SimulatedAdult - M$ObservedAdult)^2)), Intercept=model[1], Slope=model[2])
}

D = ddply(M, .(iteration, ImmigrantFecundity, AdultReproduction), discrepancy)
best = D[order(D$Discrepancy),]
print(head(best,30))
print(subset(best, Slope>0.9 & Slope<1.1 & Intercept>-0.1 & Intercept<0.1))



windows()
ggplot1 = ggplot(D,aes(x=log10(ImmigrantFecundity), y=log10(AdultReproduction))) # + facet_wrap(~K,nrow=1)
gradient = scale_fill_gradient(low="green", high="red")
alpha = 0.8
D$Discrepancy[D$Discrepancy>1000] = 1000

P = 
grid.arrange(
  ggplot1 + gradient +
    geom_raster(aes(fill=Discrepancy), interpolate=TRUE, alpha=alpha)
  ,  
  ggplot1 + gradient +
    geom_raster(aes(fill=DiscrepancyLin), interpolate=TRUE, alpha=alpha)
  ,  
  ggplot1 + gradient +
    geom_raster(aes(fill=abs(Slope-1)), interpolate=TRUE, alpha=alpha)
  ,
  ggplot1 + gradient +
    geom_raster(aes(fill=abs(Intercept)), interpolate=TRUE, alpha=alpha)
  ,
  ncol=1
)

# show_iteration = function(it) {
  # M = subset(sim, iteration==it)
  # from = min(M$Date)
  # to   = max(M$Date)
  # obs2 = subset(obs, Date>=from & Date<=to)
  # ggplot(M) +
    # geom_line(aes(x=Date, y=SimulatedAdult), colour="brown") +
    # geom_point(aes(x=Date, y=SimulatedAdult), colour="brown") +
    # geom_line(aes(x=Date, y=ObservedAdult), colour="forestgreen", data=obs2) +
    # geom_point(aes(x=Date, y=ObservedAdult), colour="forestgreen", data=obs2) 
# }
# show_iteration(42)

    
   

