// maximum_at.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

=== Background
If you need to summarise the trend of a port by the maximum value it
attained and in which step it happened, you can use a
`MaximumAt` box. The interface is simple. Just watch
the example below. If you run the simulation for several iterations, the
outputs `at` and `max` may differ between iterations.

=== Usage
We will use a #link(<boxes_Hump>)[Hump] box for demonstration since we now the
position of its maximum value:

#code-with-link(
  "https://ecolmod.org/input/demo/maximum_at/maximum_at1.box",
  raw("// maximum_at1.box"),
  "Simulation sim {
  .steps = 50
  Hump hump {
    .x = sim[step]
    .x0 = 20
    .x1 = 48
    .ymax = 150
  }
  MaximumAt maxValue {
    .track = hump[value]
  }
  OutputR {
    PageR {
      PlotR {
        .ports = hump[value] | maxValue[output::*]
        .ggplot = \"geom_vline(xintercept=34, colour='black', linetype='dashed')\"
      }
    }
  }
}"
)

With this parameter setting we expect to find the maximum in the point
\(34, 150). This is confirmed when we run the script,

```
> run demo/maximum_at/maximum_at1.box
```

and study the output:

#demo-figure(
  "maximum_at/maximum_at1.png",
  caption: [MaximumAt example],
)

Further confirmation can be gotten by the
#link("commands.html#tail")[tail] command:

```
> tail 3
```
