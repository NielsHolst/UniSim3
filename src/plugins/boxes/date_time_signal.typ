// date_time_signal.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

=== Usage

The `DateTimeSignal` box sets the `signal` to `signalInside` when the `currentDateTime` is inside the set limits, otherwise it is set to `signalOutside`. The limits are set by date (`beginDate` and `endDate`) and time of the day (`beginTime` and `endTime`) separately. Note that `beginDate` and `endDate` are _bare dates_, which means that they just specify the day and month, not the year. This means that you can have a calendar interval across 1 January, e.g. from 1 December (`beginDate`) to 1 March (`endDate`). All intervals of dates and times are closed (i.e. limits are inclusive). If `beginTime` and `endTime` keep their default value (`00:00`), the time of day is ignored.

If `circadian` is `TRUE` then `currentDateTime` is considered inside the limits if the date of `currentDateTime` is inside the [`beginDate`; `endDate`] period, and the time of `currentDateTime` is inside the [`beginTime`; `endTime`] period (specifying a recurring signal).

If `circadian` is `FALSE` then `currentDateTime` is considered inside the limits if it falls inside the period from `beginTime` on `beginDate` to `endTime` on `endDate` (specifying a single signal).

=== Circadian signals

The difference that `circadian` makes is easier to understand by example. First we demonstrate a circadian signal:

#code-with-link(
  "https://ecolmod.org/input/demo/date_time_signal/date_time_signal1.box",
  raw("// date_time_signal1.box"),
  "Simulation sim {
  Calendar calendar {
    .begin = 01/01/2001T12:00
    .end   = 10/01/2001T12:00
    .timeStep = 15
    .timeUnit = \"m\"
  }
  DateTimeSignal signal {
    .beginDate  = 03/01/2001
    .endDate    = 07/01/2001
    .beginTime  = 14:00
    .endTime    = 20:00
    .initialSignal = 50
    .signalInside  = 80
    .signalOutside = 50
  }
  OutputR {
    PageR {
      .xAxis = calendar[dateTime]
      PlotR {
        .ports = signal[value] | signal[flagIsUp] |
                 signal[flagJustRaised] | signal[flagJustLowered]
        .ggplot = \"scale_x_datetime(breaks=date_breaks('days'),
                                    labels = date_format ('%d')) +
                   labs(x='January')\"
      }
    }
  }
}"
)

The boxscript raises the signal for five days between 14:00 and 20:00 every day, as shown in the output:

#demo-figure(
  "date_time_signal/date_time_signal1.png",
  caption: [Circadian DateTimeSignal],
  args: (width: 80%),
)

You can check the precision of the signal going _on_ at the Universal Simulator prompt with the `rows 295 300` command:

```text
> rows 295 300
...
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/04T13:30:00        0              0               0   50.0         1  294
2001/01/04T13:45:00        0              0               0   50.0         1  295
2001/01/04T14:00:00        1              1               0   80.0         1  296
2001/01/04T14:15:00        1              0               0   80.0         1  297
2001/01/04T14:30:00        1              0               0   80.0         1  298
2001/01/04T14:45:00        1              0               0   80.0         1  299
```

Or at the R prompt with `sim[295:300,]`:

```text
> sim[295:300,]
               dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
295 2001-01-04 13:30:00        0              0               0     50         1  294
296 2001-01-04 13:45:00        0              0               0     50         1  295
297 2001-01-04 14:00:00        1              1               0     80         1  296
298 2001-01-04 14:15:00        1              0               0     80         1  297
299 2001-01-04 14:30:00        1              0               0     80         1  298
300 2001-01-04 14:45:00        1              0               0     80         1  299
```

Similarly, you can check the timeliness of the signal going _off_:

```text
> rows 320 325
...
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/04T19:45:00        1              0               0   80.0         1  319
2001/01/04T20:00:00        1              0               0   80.0         1  320
2001/01/04T20:15:00        0              0               1   50.0         1  321
2001/01/04T20:30:00        0              0               0   50.0         1  322
2001/01/04T20:45:00        0              0               0   50.0         1  323
2001/01/04T21:00:00        0              0               0   50.0         1  324
```

=== Non-circadian signals

In the next script (#link("https://ecolmod.org/input/demo/date_time_signal/date_time_signal2.box")[date_time_signal2.box]), we only changed one detail, namely the value of `circadian` in the `signal` box:

```boxscript
DateTimeSignal signal {
  .beginDate  = 03/01/2001
  .beginTime  = 14:00
  .endDate    = 07/01/2001
  .endTime    = 20:00
  .initialSignal = 50
  .signalInside  = 80
  .signalOutside = 50
  .circadian = FALSE
}
```

We also re-arranged the ordering of the date and time inputs to make their meaning more clear. The output shows that the signal is raised only through one time period stretching 4 days and 6 hours:

#demo-figure(
  "date_time_signal/date_time_signal2.png",
  caption: [Non-circadian DateTimeSignal],
  args: (width: 80%),
)

The precise timing is readily revealed:

```text
> rows 199 202
...
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/03T13:30:00        0              0               0   50.0         1  198
2001/01/03T13:45:00        0              0               0   50.0         1  199
2001/01/03T14:00:00        1              1               0   80.0         1  200
2001/01/03T14:15:00        1              0               0   80.0         1  201

> rows 608 611
...
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/07T19:45:00        1              0               0   80.0         1  607
2001/01/07T20:00:00        1              0               0   80.0         1  608
2001/01/07T20:15:00        0              0               1   50.0         1  609
2001/01/07T20:30:00        0              0               0   50.0         1  610
```

_Alternatively_, we can ignore the time of day all together, as shown in the #link("https://ecolmod.org/input/demo/date_time_signal/date_time_signal3.box")[date_time_signal3.box] script:

```boxscript
DateTimeSignal signal {
  .beginDate  = 03/01/2001
  .endDate    = 07/01/2001
  .initialSignal = 50
  .signalInside  = 80
  .signalOutside = 50
  .circadian = FALSE
}
```

This gives us five full days. Here the signal is going _on_ at midnight:

```text
> rows 143 146
...
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/02T23:30:00        0              0               0   50.0         1  142
2001/01/02T23:45:00        0              0               0   50.0         1  143
2001/01/03T00:00:00        1              1               0   80.0         1  144
2001/01/03T00:15:00        1              0               0   80.0         1  145
```

And here the signal is going _off_ at midnight:

```text
> rows 623 626
...
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/07T23:30:00        1              0               0   80.0         1  622
2001/01/07T23:45:00        1              0               0   80.0         1  623
2001/01/08T00:00:00        0              0               1   50.0         1  624
2001/01/08T00:15:00        0              0               0   50.0         1  625
```
