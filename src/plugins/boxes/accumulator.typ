// accumulator.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

=== Usage

The `Accumulator` box keeps track of an accumulated sum. For every simulation step, change (can be negative) is added to its `value`. This boxscript calculates the day-degrees above a threshold of 10 °C (a #link(<boxes_DayDegrees>)[DayDegrees] box would allow you more control with less BoxScript code):

#code-with-link(
"https://ecolmod.org/input/demo/accumulator/accumulator1.box",
raw("// accumulator1.box"),
"Simulation sim {
  Calendar calendar {
    .begin = 01/05/2009
    .end   = 30/09/2009
  }
  Records weather {
    .fileName = \"weather/flakkebjerg 2009.txt\"
  }
  Accumulator dd18 {
    .change = if (weather[Tavg] > 18) then weather[Tavg] − 18 else 0
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .layout = \"merged\"
        .ports = weather[Tavg] | dd18[change] | dd18[value]
      }
    }
  }
}"
)

Note that in the output, if a port is named "value" then it is by default labelled after its parent name (here "dd18"):
#demo-figure(
  "accumulator/accumulator1.png",
  caption: [Accumulator example],
  args: (width: 80%),
)