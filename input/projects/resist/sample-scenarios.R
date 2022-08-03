rm(list=ls(all=TRUE))
library(ggplot2)
library(gridExtra)
library(reshape2)
library(sensitivity)
library(stringr)


setwd("/Users/au152367/Documents/QDev/UniSim2/input/projects/resist")

# decoupled model
X = read.table("resist-6-1-sa-generate-scenarios-output.txt", header=TRUE)
X = X[complete.cases(X),-1]
N = nrow(X)
X1 = X[1:(N/2),]
X2 = X[(N/2+1):N,]
x = sobolSalt(NULL, X1, X2, scheme="A", nboot = 100)
sa_inputs = data.frame(x$X)
colnames(sa_inputs) = str_replace_all(colnames(X), "\\.", "_")
head(sa_inputs)
write.table(sa_inputs, file="scenarios.txt", quote=FALSE, sep="\t", row.names=FALSE)

### Run Universal Simulator and collect output
### ...

y = read.table("resist-6-2-sa-use-scenarios-output.txt", header=TRUE)
y = y[complete.cases(y), -(1:3)]
colnames(y) = str_replace_all(colnames(y), "\\.", "_")
summary(y)

rn = colnames(y)[-(1:2)]
rn = str_replace(rn, "_end", "")

x1 = x
y1 = as.numeric(y[,2]<30)
tell(x1,y1)
rownames(x1$S) = rn
rownames(x1$T) = rn
x1

x2 = x
y2 = as.numeric(y[,1]<30)
tell(x2,y2)
rownames(x2$S) = rn
rownames(x2$T) = rn
x2

reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

plot_sa = function(x, title) {
  M = data.frame("ModelParameter" = rownames(x$S),
                 "FirstOrderEffect" = x$S$original,
                 "TotalEffect" = x$T$original)
  ix = order(M$ModelParameter)
  M = M[ix,]
  rownames(M) = NULL

  ix = order(M$TotalEffect)
  levels(M$ModelParameter)
  M$ModelParameter = reorder_levels(M$ModelParameter, ix)
  levels(M$ModelParameter)
  
  # M = M[rev(ix),]
  # cbind(Parameter=M[,1], round(M[,2:3], 3))

  M = melt(M, id.vars="ModelParameter", variable.name="Index", value.name="Value")

  ggplot(M, aes(x=ModelParameter, y=Value, colour=Index)) +
    geom_point(size=3) +
    labs(title=title, y="Index value", x="") +
    coord_flip()
}

grid.arrange(
  plot_sa(x1, "Resistance risk"),
  plot_sa(x2, "Outbreak risk"),
  ncol=2
)

  
  