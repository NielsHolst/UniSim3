# Generate scenarios
n = 10

M = data.frame (
  ImmigrantFecundity = 10^seq(-1, 0, length.out=n)
)

f = function(M) {
  data.frame(AdultReproduction = 10^seq(0, 1, length.out=n))
}

g = function(M) {
  data.frame(K = round(10^seq(3, 6, length.out=n), 0))
}

M = adply(M, 1, f)
# M = adply(M, 1, g)
M

write.table(M, "/Users/au152367/Documents/QDev/UniSim2/input/projects/bph/bph1-scenarios.txt", quote=FALSE, sep="\t", row.names=FALSE)
