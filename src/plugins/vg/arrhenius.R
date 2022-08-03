R = 8.314
K = function(T) T+273.15

arrhenius = function(v25, Ea, T) {
  v25*exp(Ea*(T-25)/R/K(T)/K(25))
}

arrhenius_peaked = function(v25, Ea, DS, DHd, T) {

  f = function(x) {
    1 + exp((DS*K(x) - DHd)/R/K(x))
  }
  
  arrhenius(v25, Ea, T)*f(25)/f(T)
}
