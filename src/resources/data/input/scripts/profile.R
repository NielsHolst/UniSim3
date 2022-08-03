rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(plyr)

file_path = "C:/data/QDev/UniSim2/tmp/output/TestOKO1_tr_0059.txt"

P = read.table(file_path, header=FALSE, sep="\t")
colnames(P) = c("Box", "Function", "Time")
ddply(P, .(Function), function(x) sum(x$Time))

cull = function(x) {
	n = nrow(x)/10
	head(x[order(x$Time, decreasing=TRUE),], n)	
}

costly = function(x) {
	head(x[order(x$Time, decreasing=TRUE),])
}

Q = cull(P)

ggplot(Q) + 
	geom_histogram(aes(x=Time, fill=Function)) + 
	facet_wrap(~ Function, scales="free_x")
	
	
ddply(Q, .(Function), costly)


