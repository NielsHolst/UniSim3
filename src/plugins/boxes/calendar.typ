// calendar.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

*Topics*

- #link(<daily-time-step>)[Daily time step]
- #link(<changing-the-time-step>)[Changing the time step]
- #link(<setting-the-time-period-of the-simulation>)[Setting the time period of the simulation]

=== Daily time step <daily-time-step>

It is common to include a `Calendar` box in your boxscript. Its logical place is at the very top because for every time step, you will usually want update the current time before everything else:

#code-with-link(
  "https://ecolmod.org/input/demo/calendar/calendar1.box",
  raw("// records1.box"),
  "Simulation sim {
  .steps = 3
  Calendar calendar {
    .begin = 15/12/2005
  }
  Records weather { 
    .fileName = \"../weather/flakkebjerg 2005.txt\"
  }
  OutputR {
     OutputText {
       .ports = calendar[date] | weather[Tmin] | weather[Tmax]
     }
  }
}"
)


=== Daily readings
<daily-readings>
You use a `Records` box to read logged data, i.e., data with a time
stamp, typically a weather file. Here is one example showing the first
and last lines of
#link("https://ecolmod.org/input/demo/weather/flakkebjerg%202005.txt")[flakkebjerg 2005.txt⏷];:

```
      Date        Tmin Tmax  Tavg  I
01/01/2005  0    4.7   2.8   1.4
02/01/2005  0.7  6.2   3.6   1.3
03/01/2005  1.4  7     4.1   2
...
29/12/2005  -3.6 0.9  -2.2   1.3
30/12/2005  -2.4 1.3  -0.3   1.7
31/12/2005  -3  -0.8  -2.5   1.5
```

The input file must be a column-oriented text file with columns
separated by whitespace \(any number of blanks and tab stops between
each column) and one row for each records. Columns must be complete; you
cannot have an empty value. Rows, on the other hand, need not be
complete. If some records are missing, say a few days due to a
malfunctioning weather station, then the `Records` box will
automatically interpolate \(linearly) between neighboring records.

The `Records` box must be accompanied by a `Calendar` box. It is the
`Calendar` box that keeps track of time during a simulation, not the
`Records` box. In this example, the simulation runs through three days
in December:

#code-with-link(
  "https://ecolmod.org/input/demo/records/records1.box",
  raw("// records1.box"),
  "Simulation sim {
  .steps = 3
  Calendar calendar {
    .begin = 15/12/2005
  }
  Records weather { 
    .fileName = \"../weather/flakkebjerg 2005.txt\"
  }
  OutputR {
     OutputText {
       .ports = calendar[date] | weather[Tmin] | weather[Tmax]
     }
  }
}"
)

We get this output from the simulation, as we would expect:

```
> load demo/records/records1.box
```

Each of the columns in the input file produces an output port with the
same name as the column heading.

=== Interpolation
<interpolation>
The temperatures are daily estimates, which for interpolation purposes
are assumed to have occured at midnight. We can explore this by changing
the time step to 8 hours:

#code-with-link(
  "https://ecolmod.org/input/demo/records/records2.box",
  raw("// records2.box"),
  "Simulation sim {
  .steps = 6
  Calendar calendar {
    .begin = 15/12/2005
    .timeStep = 8
    .timeUnit = \"h\"
  }
  Records weather { 
    .fileName = \"../weather/flakkebjerg 2005.txt\"
  }
  OutputR {
     OutputText {
       .ports = calendar[dateTime] | weather[Tmin] | weather[Tmax]
     }
  }
}"
)

The output hits the same values as before every midnight
\(#raw(lang:"command", "head") shows only the first 6 rows of output,
write #raw(lang:"command", "head 10") to see up to ten lines):

```
> head 10
```

#emph[Pitfalls];. Plants and animals have a diurnal rythm. Be careful
that your model makes sense, if you decrease the time step to less than
1 day. If use a daily accumulated variable like precipitation, you need
to transform the data. If your time step is 1 hour, for example, then
you must divide the precipitation data in the input file by 24 before
using it in a `Records` box.

=== Hourly readings
<hourly-readings>
If you model runs on a finer time scale, you may have records at an
hourly interval, or even down to seconds, corresponding to the smallest
time step supported by `Calendar` \(1 second). You may also have records
at irregular intervals with a time stamp of date and time. In any case,
you provide the time of the day in a separate column, as exemplified by
the file
#link("https://ecolmod.org/input/demo/weather/copenhagen-may-hourly.txt")[copenhagen-may-hourly.txt⏷];,
which contains hourly data for the first half of May:

```
Date       Time  Tair Rhair GlobRad DifRad Windspeed
01-05-2001 00:00 4.7  85     0      0      2.6
01-05-2001 01:00 4.1  84     0      0      2.6
01-05-2001 02:00 3.8  84     0      0      2.6
...                                        
15-05-2001 22:00 7.4  83     0      0      5.8
15-05-2001 23:00 6.7  84     0      0      5.5
16-05-2001 00:00 6.1  85     0      0      5.1
```

In our boxscript, we specify the period of the simulation in the
`Calendar` box:

#code-with-link(
  "https://ecolmod.org/input/demo/records/records3.box",
  raw("// records3.box"),
  "Simulation sim {
  Calendar calendar {
    .begin = 01/05/2001T00:00
    .end   = 06/05/2001T24:00
    .timeStep = 15
    .timeUnit = \"m\"
  }
  Records weather { 
    .fileName = \"../weather/copenhagen-may-hourly.txt\"
  }
  OutputR {
    PageR {
      .xAxis = calendar[dateTime]
      PlotR {
       .ports = weather[Tair] | weather[GlobRad] |weather[Windspeed]
      }
    }
  }
}"
)

We let the simulation run with a 15-minute time step, which is a finer
time resolution than given in the weather file. The `Records` box will
oblige by interpolating the missing data, as you can see here:

```
> ht 3
```

The plot looks like this:

~

#demo-figure(
  "records/records3.png",
  caption: [Records example],
)

#emph[The two flavours of midnight];. The time stamps
2001/05/06T24:00:00 and 2001/05/07T00:00:00 are equivalent because 24
hours is translated into 0 hours \(midnight) on the following day.

=== Ignoring the year
<ignoring-the-year>
It happens quite often that you want to use a weather file without
regards to the year noted in the time stamp. This is the purpose of
`ignoreYear`, which defaults to `FALSE` but here set it to `TRUE`:

#code-with-link(
  "https://ecolmod.org/input/demo/records/records4.box",
  raw("// records4.box"),
  "Simulation sim {
  .steps = 3
  Calendar calendar {
    .begin = 15/12/2022
  }
  Records weather { 
    .fileName = \"../weather/flakkebjerg 2005.txt\"
    .ignoreYear = TRUE
  }
  ...
}"
)

The Calendar box sets the simulation to begin on 15 December 2022. In
response, the Records box will begin producing output from the first row
that has a time stamp of 15 December. In this case, it wll be ignoring
the time stamp\'s year \(2005). As a result we get this output:

We do get the same output as previously, except the year now follows the
`Calendar` box:

```
      date Tmin Tmax iteration step
2022/12/15  2.7  8.1         1    0
2022/12/16  1.3  4.5         1    1
2022/12/17 -4.2  0.7         1    2
2022/12/18 -2.8  1.6         1    3
```

=== Extrapolation
<extrapolation>
What happens if we run past the end of the weather file? Consider this:

#code-with-link(
  "https://ecolmod.org/input/demo/records/records6.box",
  raw("// records6.box"),
  "Simulation sim {
  .steps = 5
  Calendar calendar {
    .begin = 28/12/2005
  }
  Records weather { 
    .fileName = \"../weather/flakkebjerg 2005.txt\"
  }
  ...
}"
)

The `Records` box responds by extrapolating the final reading:

```
      date Tmin Tmax iteration step
2005/12/28 -3.2 -1.4         1    0
2005/12/29 -3.6 -0.9         1    1
2005/12/30 -2.4  1.3         1    2
2005/12/31 -3.0 -0.8         1    3
2006/01/01 -3.0 -0.8         1    4
2006/01/02 -3.0 -0.8         1    5
```

=== Recycling the year
<recycling-the-year>
If your weather file goes from 1 January to 31 December, or if it
otherwise forms a whole year from beginning to end, e.g. from 21 March
one year to 20 March the following year, then you can ask the Records
box to recycle the readings. In that way, you can use one year\'s
weather data to simulate several years. All right, that\'s a quick and
dirty way of doing it, but sometimes we just want it like that.

You achieve this behaviour by setting the `cycle` flag `TRUE` \(it
defaults to `FALSE`). If `cycle` is `TRUE` then `ignoreYear` will always
be forced to `TRUE` as well, since the year registered in the input file
has become irrelevant. Let\'s try it:

#code-with-link(
  "https://ecolmod.org/input/demo/records/records7.box",
  raw("// records7.box"),
  "Simulation sim {
  .steps = 5
  Calendar calendar {
    .begin = 30/12/2022
  }
  Records weather { 
    .fileName = \"../weather/flakkebjerg 2005.txt\"
    .cycle = TRUE
  }
  ...
}"
)

We have set the calendar to reach past the end of the year. The output
shows that we have successfully achieved to wrap around the weather file
on 31 December and cycle back to its beginning on 1 January:

```
      date Tmin Tmax iteration step
2022/12/30 -2.4  1.3         1    0
2022/12/31 -3.0 -0.8         1    1
2023/01/01  0.0  4.7         1    2
2023/01/02  0.7  6.2         1    3
2023/01/03  1.4  7.0         1    4
2023/01/04  2.6  7.7         1    5
```
