
source("~/Rdev/setup.R")

# Constants

T0 = 273.15
R = 8.314
RhoAir = 1.19
Mwater = 18.016e-3

# Virtual temperature from ah

shFromAh = function(ah) {
  ah/RhoAir;
}

virtualTemperatureFromAh = function(temperature, ah) {
  (temperature+T0)*(1+0.608*shFromAh(ah));
}

# ah from rh

svp = function(temperature) {
  610.8*exp(17.27*temperature/(temperature+237.3));
}

ahFromVp = function(temperature, vp) {
  vp*Mwater/(R*(temperature + T0));
}


sah = function(temperature) {
  ahFromVp(temperature, svp(temperature));
}

ahFromRh = function(temperature, rh) {
  rh/100*sah(temperature);
}

# Virtual temperature from rh
virtualTemperatureFromRh = function(temperature, rh) {
  virtualTemperatureFromAh(temperature, ahFromRh(temperature, rh))
}

#
# Test
#

virtualTemperatureFromRh(20, 80)
