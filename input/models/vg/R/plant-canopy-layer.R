sigma = 0.2
rho_h = (1 - sqrt(1-sigma))/(1 + sqrt(1-sigma))
rho_h

rho = function(k, Lai) {
  (exp(-k*Lai) - exp(k*Lai)) / (rho_h*exp(-k*Lai) - exp(k*Lai)/rho_h)
}

alpha = function(k, Lai) {
  1 - exp(-k*Lai)
}

tau = function(k, Lai) {
  pmax(1 - rho(k, Lai) - alpha(k, Lai), 0)
}

M = expand.grid(
  k = c(0.7, 1),
  Lai = (0:30)/10
)

M$reflectivity   = with(M, rho  (k, Lai))
M$absorptivity   = with(M, alpha(k, Lai))
M$transmissivity = with(M, tau  (k, Lai))

M = melt(M, id.vars=c("k","Lai"), variable.name="Parameter", value.name="Value")
M$klab = paste("k =", M$k)

windows(7,3)
ggplot(M, aes(Lai, Value, fill=Parameter)) +
  geom_area(alpha=0.3) +
  scale_fill_manual(values=c(blue, green, red), name="") +
  labs(x="LAI", y="") + 
  facet_wrap(~klab, nrow=1)

round(
  c(
    alpha(0.65, 1.9),
    rho(  0.65, 1.9),
    tau(  0.65, 1.9)
  )
  , 2
)
  
round(
  c(
    alpha(1, 1.9),
    rho(  1, 1.9),
    tau(  1, 1.9)
  )
  , 2
)
