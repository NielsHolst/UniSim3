sigma = 5.670e-8

E = function(T, a=0.83) {
  a*sigma*(T + 273.15)^4
}

E(-25) - E(0)



