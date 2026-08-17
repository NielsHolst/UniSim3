// hump.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

=== Usage
You use the `Hump` box to produce a generic, symmetric hump-or
ditch-shaped curve with set limits on the x- and y-axis. The curve is a
fourth degree polynomium:

$ f_("hump")(x) = y_("min") + g(x) \
  f_("ditch")(x) = y_("max") - g(x) $

where

$ g(x) = cases( ((y_("max") - y_("min")) (4 ((x-x_0)(x-x_1))/((x_0-x_1)(x_1-x_0)))^2, "for " x in [x_0; x_1]), (0, "otherwise") ) $

The following script demonstrates the shapes that can be obtained:

#code-with-link(
  "https://ecolmod.org/input/demo/hump/hump1.box",
  raw("// hump1.box"),
  "Simulation sim {
  .steps = temperature[steps]
  SequenceByStep temperature {
    .min = -5
    .max = 40
    .by = 0.5
  }
  Box humps {
    Hump hump {
      .x = temperature[value]
      .x0 = 12
      .x1 = 34
      .ymax = 100
    }
    Hump humpOffset {
      .x = temperature[value]
      .x0 = 12
      .x1 = 34
      .ymax = 100
      .ymin = 20
    }
    Hump ditch {
      .x = temperature[value]
      .x0 = 12
      .x1 = 34
      .ymax = 100
      .ditch = TRUE
    }
    Hump ditchOffset {
      .x = temperature[value]
      .x0 = 12
      .x1 = 34
      .ymax = 100
      .ymin = 20
      .ditch = TRUE
    }
  }
  OutputR {
    PageR {
      .xAxis = temperature[value]
      PlotR {
        .ports = humps/*[value]
        .ncol = 2
      }
    }
  }
}"
)

Here is the output:

#demo-figure(
  "hump/hump1.png",
  caption: [Hump example],
)

The curve takes off exactly from its cloor or ceiling exactly at the
limits `x0` and `x1` as you can see by the `rows` command:

```
> rows 34 37
...
temperature     hump humpOffset   ditch ditchOffset iteration step
       11.5      0.0       20.0   100.0       100.0         1   33
       12.0      0.0       20.0   100.0       100.0         1   34
       12.5 0.789307    20.6314 99.2107     99.3686         1   35
       13.0  3.01209    22.4097 96.9879     97.5903         1   36
> rows 77 80
...
temperature     hump humpOffset   ditch ditchOffset iteration step
       33.0  3.01209    22.4097 96.9879     97.5903         1   76
       33.5 0.789307    20.6314 99.2107     99.3686         1   77
       34.0      0.0       20.0   100.0       100.0         1   78
       34.5      0.0       20.0   100.0       100.0         1   79
```
