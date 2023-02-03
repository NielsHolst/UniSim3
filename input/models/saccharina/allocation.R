rm(list=ls(all=TRUE))

# Parameters
# Carbon store
concCC = 0.47
# Nitrogen store
concNN = 0.37

# Initial resources
R0 = data.frame(
  # Sources
  dPC = 50, dUN = 2, dUP = 0.1, dRC = 5, dRN = 1,
  # Stores
  CC = 250, NN = 15
)

# Current resources
R = R0

take = function(demand, sources) {
  supply = 0
  lacking = demand
  # Go through sources in order of priority
  for (source in sources) {
    lacking = demand - supply
    taken = if (lacking > R[source]) R[source] else lacking
    supply = supply + taken
    R[source] <<- R[source] - taken
    # Stop if supply meets demand
    if (abs(supply - demand) < 1e-6) break
  }
  supply
}

test_take = function(demand, sources) {
  R <<- R0
  supply = take(demand, sources)
  side_effects = rbind(R0, R)
  rownames(side_effects) = c("Before", "After")
  list(
    Supply       = supply,
    Side_effects = side_effects
  )
}

test_take(  4, c("dPC", "dRC", "CC"))
test_take( 52, c("dPC", "dRC", "CC"))
test_take( 70, c("dPC", "dRC", "CC"))
test_take(500, c("dPC", "dRC", "CC"))

test_take(  1.5, c("dUN", "dRN", "NN"))
test_take(  2.3, c("dUN", "dRN", "NN"))
test_take(  5.0, c("dUN", "dRN", "NN"))

