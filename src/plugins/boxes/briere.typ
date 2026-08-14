// accumulator.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

=== Background

The `Briere` box implements the non-linear model for physiological development of #link("https://doi.org/10.1093/ee/28.1.22")[Briere et al. (1999)] defined by three inputs: `T0`, `Tmax` and `a`. They designate the lower and upper threshold for development, respectively, and a scaling parameter with no direct biological meaning.  The equation for development rate is

$
1/L = a T (T - T_0) sqrt(T_"max" - T)
$

The position of the temperature optimum is given implicitly by $T_0$ and $T_"max"$ and can be derived from the above equation as

$
T_"opt" = (sqrt(9 T_0^2 - 16 T_0 T_"max" + 16 T_"max"^2) + 3 T_0 + 4 T_"max")/10
$

Maybe of more relevance is the finding, that if you want to define the curve by its optimum rather than its maximum temperature, you can calculate the maximum temperature from the optimal temperature by

$
T_"max" = (T_"opt" (5 T_"opt" - 3 T_0)) / (4 T_"opt" - 2 T_0)
$

=== Parameter estimation