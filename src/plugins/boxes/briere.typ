// briere.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

=== Background

The `Briere` box implements the non-linear model for physiological development of #link("https://doi.org/10.1093/ee/28.1.22")[Briere et al. (1999)] defined by three inputs: `T0`, `Tmax` and `a`. They designate the lower and upper threshold for development, respectively, and a scaling parameter with no direct biological meaning. The equation for development rate is

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

The three parameters of the curve can be estimated by non-linear regression. Here is a typical data set for temperature-dependent insect development rate. It shows the development time (_L_) and rate (1/_L_) at different temperatures (_T_) for brown marmorated stink bug (_Halyomorpha halys_) (from #link("https://pubmed.ncbi.nlm.nih.gov/18419906/")[Nielsen et al. (2008)]):

#table(
  columns: (1fr, 1fr, 1fr),
  inset: 5pt,
  [*$T$ [°C]*], [*$L$ [d]*], [*$1/L$ [$d^(-1)$]*],
  [15], [$infinity$], [0.0000],
  [17], [121.50], [0.0082],
  [20], [81.07], [0.0123],
  [25], [44.65], [0.0224],
  [27], [36.67], [0.0273],
  [30], [33.73], [0.0296],
  [33], [37.67], [0.0265],
  [35], [$infinity$], [0.0000],
)

Using R _nls_ we arrive at the estimates, $T_0 = 14.2$ °C, $T_"max" = 35.0$ °C and $a = 2.836 times 10^(-5)$ $d^(-1)$ that we can plug into this boxscript, which also shows two alternative models (simple linear and the #link(<boxes_DayDegrees>)[DayDegrees] box, which features a breakpoint at the optimum temperature):

#code-with-link(
  "https://ecolmod.org/input/demo/briere/briere1.box",
  raw("// briere1.box"),
  "Simulation sim {
  .steps = temperature[steps]
  SequenceByStep temperature {
    .min = 12
    .max = 37
    .by = 0.2
  }
  DayDegrees dayDegrees {
    .timeStepDays = 1
    .T0 = 14.2
    .Topt = 29
    .Tmax = 35
    .T = temperature[value]
  }
  Briere briere {
    .timeStepDays = 1
    .T0 = 14.2
    .Tmax = 35
    .a = 2.836e-5
    .T = temperature[value]
  }
  OutputR {
    .scripts = \"briere1.R\"
    OutputText {
      .ports = temperature[value] | dayDegrees[step] | briere[step]
    }
  }
}"
)

This is the plot produced:

#demo-figure(
  "briere/briere1.png",
  caption: [Briere development-rate example],
  args: (width: 80%),
)

=== Usage

In practical usage, you often want to turn the ticking of the physiological clock on and off. Sometimes, you may want to reset it to zero too. This is what the two remaining input ports (`isTicking` and `resetTotal`) are for. They are both of the boolean type. In this script, the clock is only ticking in the period 1 May to 1 October. Moreover, it is reset to zero on 1 May and 1 July:

#code-with-link(
  "https://ecolmod.org/input/demo/briere/briere2.box",
  raw("// briere2.box"),
  "Simulation sim {
  Calendar calendar {
    .begin = 01/01/2009
    .end   = 31/12/2010
  }
  Records weather {
    .fileName = \"weather/flakkebjerg 2009.txt\"
    .cycle = TRUE
  }
  Briere clock {
    .isTicking = (calendar[date] >= 01/05) && (calendar[date] < 01/10)
    .resetTotal = (calendar[date] == 01/05) || (calendar[date] == 01/07)
    .T0 = 8
    .Tmax = 32
    .a = 2.836e-5
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .ports = clock[output::*] | clock[isTicking] | clock[resetTotal]
        .ggplot = \"scale_x_datetime(breaks=date_breaks('3 months'),
                                    labels = date_format ('%b')) +
                   labs(x='2009 - 2010')\"
      }
    }
  }
}"
)

Noticable BoxScript tricks include

- the use of the `cycle` port in `Records` to recycle the weather data;
- the use of _bare dates_ to ignore the year of the date (e.g., `01/05` to represent 1 May any year);
- the use of the `ggplot` port to `PlotR` to add details to the plot produced.

The output shows two years worth of data, identical since the weather of 2009 is used for both years:

#demo-figure(
  "briere/briere2.png",
  caption: [Briere clock switched on, off and reset],
  args: (width: 80%),
)
