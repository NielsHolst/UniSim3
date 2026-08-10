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
}"
)

Note that in the output a port named value is by default labelled after its parent (here dd18):
#demo-figure(
  "accumulator/accumulator1.png",
  caption: [Accumulator example],
  args: (width: 80%),
)