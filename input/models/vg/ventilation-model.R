opening = (0:100)/100

flux = function(opening) {
  1 - (1 - opening)^4
}

M = data.frame(
  Opening = opening,
  Flux = flux(opening)
)

ggplot(M, aes(Opening, Flux)) +
  geom_line() +
  scale_x_continuous(breaks=(0:10)/10) +
  scale_y_continuous(breaks=(0:10)/10)
  
  