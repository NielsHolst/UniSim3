# The boxes plugin

## Introduction

The `boxes` plugin contains model building blocks of general utility. The functionality of these building blocks ([classes](#classes)) is described below

You can run the boxscripts shown in the sub-sections below yourself. For that purpose you must [install Universal Simulator](#software-installation). For example, to run the  `briere1.box` script, you write at the Universal Simulator prompt:

```
run demo/briere/briere1.box
```

Paste the clipboard at the R prompt to see the output as described in [Running simulations](#running-simulations).

## Accumulator

### Interface


| Inputs | Type | Default | Purpose |
| ------ | ---- | ------- | ------- |
|initial |double|          0.0 []|Initial value at reset|
|change  |double|          0.0 []|Change added to value|
|minValue|double|-1.79769e+308 []|Minimum possible value|
|maxValue|double| 1.79769e+308 []|Maximum possible value|
| **Outputs** | | | |
|value   |double|              []|Accumulated value|
|signal  |double|              []|Synonym for 'value'|

### Usage

The Accumulator box keeps track of an accumulated sum. For every simulation step, `change` (can be negative) is added to its `value`. This boxscript calculates the day-degrees above a threshold of 10 °C (a [DayDegrees](#daydegrees) box would allow you more control with less BoxScript code):

```
// accumulator1.box
Simulation sim {
  Calendar calendar {
    .begin = 01/05/2009
    .end   = 30/09/2009
  }
  Records weather {
    .fileName = "weather/flakkebjerg 2009.txt"
  }
  Accumulator dd18 {
    .change = if (weather[Tavg]>18) then weather[Tavg]-18 else 0
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .layout = "merged"
        .ports = weather[Tavg] | dd18[change] | dd18[value]
      }
    }
  }
}
```

Note that in the output a port named `value` is by default labelled after its parent (here `dd18`):

![](../media/demo/accumulator1.png)

## BaseSignal

### Interface


| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|initialSignal  |double  |     0.0 []|Value of signal at reset|
|initialFlag    |bool    |   FALSE []|Value of flag at reset|
| **Outputs** | | | |
|signal         |double  |         []|Value of the signal|
|flagIsUp       |bool    |         []|Is the signal on?|
|flagJustRaised |bool    |         []|Was the signal just set on?|
|flagJustLowered|bool    |         []|Was the signal just set off?|
|value          |double  |         []|Synonym for signal|

### Usage

`BaseSignal` is the base class for several derived `Box` classes:

* [DateTimeSignal](#datetimesignal)
* [FixedSignal](#fixedsignal)
* [PrioritySignal](#prioritysignal)
* [ProportionalSignal](#proportionalsignal)
* [ThresholdSignal](#thresholdsignal)

`BaseSignal` cannot be used directly in a boxscript, only indirectly through one of its derived classes. The input and outputs of `BaseSignal` have the same purpose in all the derived classes as described here.

The `signal` output reflects the status of other inputs (defined in the derived class). As a convenience `signal` has the synonym `value`. When the box is reset it will start out with the `initialSignal`.

When the `signal` is used as a binary response (`signal` is either on or off), the various `flag` ports may provide more legible code. The description above should suffice to understand their use. 

Please find concrete examples of usage in the derived classes listed above.

## Briere

### Interface

The inputs and outputs written in *italics* are inherited from the [PhysiologicalTime](#physiologicaltime) box. See explanation there.


| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|*T*           |double|*computed*|*= weather[Tavg]*|
|*timeStepDays*|double|*computed*|*= calendar[timeStepDays]*|
|*resetTotal*  |bool  |  FALSE []   |Reset total to zero?|
|*isTicking*   |bool  |   TRUE []   |Is the physiological time running? Else step will be zero|
|T0          |double|    0.0 [oC] |Lower temperature threshold|
|Tmax        |double|    0.0 [oC] |Upper temperature threshold|
|a           |double|    0.0 [/oC]|Scaling parameter|
| **Outputs** | | | |
|*step*        |double|        [PTU]|Increment in physiological time units|
|*total*       |double|        [PTU]|Total time since reset|

### Background

The `Briere` box implements the non-linear model for physiological development of [Briere et al. (1999)](https://www.doi.org/10.1093/ee/28.1.22), defined by three parameters `T0`, `Tmax` and `a`. They designate the lower and upper threshold for development, respectively, and a scaling parameter with no biological meaning.  The equation for development rate is
$$
1/L = aT(T-T_0)\sqrt{T_{max}-T}
$$
The position of the temperature optimum is given implicitly by `T0` and `Tmax` and can be derived from the above equation as
$$
T_{opt}= \frac{\sqrt{9T_0^2 - 16T_0T_{max} + 16T_{max}^2} + 3T_0 + 4T_{max}}{10}
$$

Maybe of more relevance is the finding, that if you want to define the curve by its optimum rather than its maximum temperature, you can calculate the maximum temperature from the optimal temperature by
$$
T_{max} = \frac {T_{opt}(5T_{opt} - 3T_0)}{4T_{opt} - 2T_0}
$$


### Parameter estimation

The three parameters of the curve can be estimated by non-linear regression. Here is a typical data set for temperature-dependent insect development rate. It shows the development time (*L*) and rate (1/*L*) at different temperatures (*T*) for brown marmorated stink bug (*Halyomorpha halys*) (from [Nielsen
et al. (2008)](https://pubmed.ncbi.nlm.nih.gov/18419906/):

|T [&#176;C] | L [d]| 1/L (d^-1^) |
|-------|------|--------|
|15 | &infin; | 0.0000 |
|17 | 121.50   | 0.0082 |
|20 |  81.07   | 0.0123 |
|25 |  44.65   | 0.0224 |
|27 |  36.67   | 0.0273 |
|30 |  33.73   | 0.0296 |
|33 |  37.67   | 0.0265 |
|35 | &infin; | 0.0000 |

 Using R *nls* we arrive at the estimates, T0=14.2&#176;C, Tmax=35.0&#176;C and a=2.836 × 10^−5^ d^−1^ that we can plug into this boxscript, which also shows two alternative models (simple linear and the [DayDegrees](#daydegrees) box, which features a breakpoint at the optimum temperature):

```
// briere1.box
Simulation sim {
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
    .scripts = "briere1.R"
    OutputText {
      .ports = temperature[value] | dayDegrees[step] | briere[step]
    }
  }
}
```

This is the plot produced:

![](../media/demo/briere1.png)

### Usage

In practical usage, you often want to turn the ticking of the physiological clock on and off. Sometimes, you may want to reset it to zero too. This is what the two remaining input ports (`isTicking` and `resetTotal`) are for. They are both of the boolean type. In this script, the clock is only ticking in the period 1 May to 1 October. Moreover, it is reset to zero on 1 May and 1 July:

```
// briere2.box
Simulation sim {
  Calendar calendar {
    .begin = 01/01/2009
    .end   = 31/12/2010
  }
  Records weather {
   .fileName = "weather/flakkebjerg 2009.txt"
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
        .ggplot = "scale_x_datetime(breaks=date_breaks('3 months'), 
                                    labels = date_format ('%b')) +
                   labs(x='2009 - 2010')"
      }
    }
  }
}
```

Noticable BoxScript tricks include 

* the use of the `cycle` port in `Records` to recycle the weather data; 
* the use of *bare dates* to ignore the year of the date (e.g., `01/05` to represent 1 May any year);
* the use of the `ggplot` port to `PlotR` to add details to the plot produced.

The output shows two years worth of data, identical since the weather of 2009 is used for both years:

![](../media/demo/briere2.png)


## Calendar

### Interface

| Inputs | Type | Default | Purpose |
| ------ | ---- | ------- | ------- |
|latitude      |double  |               52.0 [-90,90]  |Latitude|
|longitude     |double  |               11.0 [-180,180]|Longitiude|
|timeZone      |int     |                  1 [h]       |Time zone|
|begin         |datetime|2000/01/01T00:00:00 []        |When simulation begins|
|end           |datetime|               null []        |When simulation ends (optional)|
|timeStep      |int     |                  1 [>0]      |Time step in units of timeUnit|
|timeUnit      |string  |                "d" []        |Unit of time step (y&#124;d&#124;h&#124;m&#124;s)|
| **Outputs** | | | |
|steps         |int     |                    []        |Number of steps from begin to end|
|date          |date    |                    []        |Current date|
|time          |time    |                    []        |Current time of the day|
|dateTime      |datetime|                    []        |Current date and time|
|timeStepSecs  |double  |                    [s]       |Time step duration|
|timeStepDays  |double  |                    [d]       |Time step duration|
|totalTimeSteps|int     |                    []        |Total number of time steps since calendar was reset|
|totalTime     |int     |                    [d]       |Total time since calendar was reset; in units of timeUnit|
|totalDays     |double  |                    [d]       |Total time since calendar was reset|
|dayOfYear     |int     |                    [1;366]   |Julian day|
### Daily time step

It is common to include a `Date` box in your boxscript. Its logical place is at the very top because for every time step, you will usually want update the current time before everything else:

```
// calendar1.box
Simulation sim {
  .steps = 3
  Calendar calendar {
  	.begin = 15/12/2022
  }
  OutputR {
  	 OutputText {
  	   .ports = calendar[date] | calendar[time] | calendar[dateTime]
  	 }
  }
}
```

`Calendar` defaults to daily increments. Hence we get this output from the boxscript:

```
> load demo/calendar/calendar1.box
...
> run
...
> head
...
      date     time            dateTime iteration step
2022/12/15 00:00:00 2022/12/15T00:00:00         1    0
2022/12/16 00:00:00 2022/12/16T00:00:00         1    1
2022/12/17 00:00:00 2022/12/17T00:00:00         1    2
2022/12/18 00:00:00 2022/12/18T00:00:00         1    3
```

You will often use `date`, `time` or `dateTime` to put on the x-axis in the output plots. They can also be used as inputs to boxes that depends on the date or the hour of the day.

### Changing the time step

You can change the time step. Here we also chose to set the specific time of the day for the simulation to start:

```
// calendar2.box
Simulation sim {
  .steps = 3
  Calendar calendar {
    .begin = 15/12/2022T17:30
    .timeStep = 2
    .timeUnit = "m"
  }
  ...
}
```

You  combine a date and a time with a `T` inbetween (no spaces). The `timeStep` must be an integer value, while the `timeUnit` is a one-character string providing you these options: y)ears, d)ays, h)ours, m)inutes and s)econds. Write the first character in apostrophs as shown above. The output is no surprise:

```
      date     time            dateTime iteration step
2022/12/15 17:30:00 2022/12/15T17:30:00         1    0
2022/12/15 17:32:00 2022/12/15T17:32:00         1    1
2022/12/15 17:34:00 2022/12/15T17:34:00         1    2
2022/12/15 17:36:00 2022/12/15T17:36:00         1    3
```

### Setting the time period of the simulation

Maybe you prefer to specify the simulation period by date (maybe by date and time) rather then by the number of steps. That is easily done:

```
// calendar3.box
Simulation sim {
  Calendar calendar {
    .begin = 15/12/2022
    .end   = 01/05/2024
  }
  ...
}
```

When you have `run` this, use the command `ht 3` to show the three head and 3 tail rows of output:

```
      date     time            dateTime iteration step
2022/12/15 00:00:00 2022/12/15T00:00:00         1    0
2022/12/16 00:00:00 2022/12/16T00:00:00         1    1
2022/12/17 00:00:00 2022/12/17T00:00:00         1    2
                                    ...
2024/04/29 00:00:00 2024/04/29T00:00:00         1  501
2024/04/30 00:00:00 2024/04/30T00:00:00         1  502
2024/05/01 00:00:00 2024/05/01T00:00:00         1  503
```

How did that happend you might ask? It seems as if `sim[steps]` was magically set to 503, so that we would end up on 5 May 2024. Well, if we inspect the final values of the `calendar` ports, we will find among them an outport called `steps`:

```
> list calendar p
...
>steps = 503
...
```

So, `calendar` always calculates how many time steps will be need to reach `end` from `begin` and puts this value in its `steps` output. The `Simulation` box by default looks this up and uses it for its own `steps` (which is an input, not an output). This is the default expression for `steps` defined in the C++ code for the `Simulation` class:

```c++
Input(steps).computes("if exists(Calendar::*[steps]) then Calendar::*[steps] else 1");
```

## Computation

### Interface


| Outputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|step   |string|        []|Name of current computation step|

### Usage

The `Computation` box simply has a string output telling you the current [computation step](#computational-model). The possible values are

* `"ready"` 
* `"construct"` 
* `"amend"`
* `"initialize"`
* `"reset"`
* `"update"` 
* `"cleanup"` 
* `"debrief"`

It can be useful when learning BoxScript. Here is a simple boxscript for exponential growth:

```
// computation1.box
Simulation sim {
  .iterations = 2
  .steps = 3
  Computation comp {  
  }
  Box pop { 
    &r = 1.05
    &density = if    (comp[step]=="reset" ) then 100.0 
               elsif (comp[step]=="update") then .[r] * .[density]
               else .[density]
  }
  OutputR {
    OutputText output {
      .ports = pop[*] | sim[step] | comp[step]
    }
  }
}
```

The simulation will run for two iterations, three steps each as governed by the inputs to `sim`. The Computation box is simply declared without any input values (it has no inputs). The `pop` box is of the generic (empty) type `Box`. We equip it with [auxiliary ports](#objects) using the ampersand designator (&#38;). Here we have set the growth rate `r` to 5% per step. The `density` is set to `100.0` in the `reset` step. In the `update` step the 5% growth is applied, while in all other steps it just keeps its value.

BoxScript tricks work noting:

* We include the decimal point `100.0` to set the `density` port to be of `double` type. A value of `100` would have set it to an `int`.
* References to sibling ports inside a box begins with a period to signify *this box*, e.g. `.[density]`.

The output is sent to a text file. Use the command `head 10` to see the first 10 (or fewer) lines:

```
> head 10
...
comp.step    r density iteration sim.step
    reset 1.05   100.0         1        0
   update 1.05   105.0         1        1
   update 1.05  110.25         1        2
   update 1.05 115.762         1        3
    reset 1.05   100.0         2        0
   update 1.05   105.0         2        1
   update 1.05  110.25         2        2
   update 1.05 115.762         2        3	
```

You can see that `OutputText` by default writes one line of output for every `reset` or `update` step.

## Date

### Interface


| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|day    |int |      1 [1;31]   |Day of month|
|month  |int |      1 [1;12]   |Month|
|year   |int | 2000 |Year|
| **Outputs** | | | |
|date   |date|        []       |The date constructed|

### Usage

Use this box to construct a date from `day`, `month` and `year`. Here is an example:

```
// date1.box
Simulation sim {
  .steps = 3
  Date date {
    .day   = sim[step] + 10
    .month = 5
  }
  OutputR {
    OutputText {
      .ports = date[date]
    }
  }
}
```

Here is the output:

```
> head
...
      date iteration step
2000/05/10         1    0
2000/05/11         1    1
2000/05/12         1    2
2000/05/13         1    3
```



## DateSplit

### Interface


| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|date   |date|   null []|The date to split|
| **Outputs** | | | |
|day    |int |        []|Day of month|
|month  |int |        []|Month|
|year   |int |        []|Year|

### Usage

You use the `DateSplit` box to split a `date` variable into `day`, `month` and `year`. If you are trying to do something complicated then consider instead moving the complications to an R script that can be executed after the simulation has run (see [OutputR](#outputr)). This simple R script demonstrates the functionality:

```
// date_spit1.box
Simulation sim {
  .steps = 3
  Calendar calendar {
    .begin = 24/12/2018
  }
  DateSplit dmy {
    .date   = calendar[date]
  }
  OutputR {
    OutputText {
      .ports = dmy[output::*]
    }
  }
}
```

The unsurprising output is

```
> head
...
day month year iteration step
 24    12 2018         1    0
 25    12 2018         1    1
 26    12 2018         1    2
 27    12 2018         1    3
```



## DateTimeSignal

### Interface

The inputs and outputs written in *italics* are inherited from the [BaseSignal](#basesignal) box. See explanation there.

| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|*initialSignal*  |double  |     0.0 []|The value that signal is set to at reset|
|*initialFlag*    |bool    |   FALSE []|The value that flag is set to at reset|
|currentDateTime|datetime|*computed*|*= calendar[dateTime]*|
|beginDate      |baredate|     1/1 []|The beginning of the date interval|
|endDate        |baredate|   31/12 []|The end of the date interval|
|beginTime      |time    |00:00:00 []|The beginning of the time interval|
|endTime        |time    |00:00:00 []|The end of the time interval|
|signalInside   |double  |     1.0 []|Resulting signal when day and time are inside the day and time intervals|
|signalOutside  |double  |     0.0 []|Resulting signal when day and time are outside the day and time intervals|
|circadian      |bool    |    TRUE []|Tells whether signal follows a daily rythm|
| **Outputs** | | | |
|*signal*         |double  |         []|Value of the signal|
|*flagIsUp*       |bool    |         []|Is the signal different from zero?|
|*flagJustRaised* |bool    |         []|Did flagIsUp change from false to true during the latest update?|
|*flagJustLowered*|bool    |         []|Did flagIsUp change from true to false during the latest update?|
|*value*          |double  |         []|Synonym for 'signal'|
### Usage

The `DateTimeSignal` box sets the `signal` to `signalInside` when the `currentDateTime` is inside the set limits, otherwise it is set to `signalOutside`. The limits are set by date (`beginDate` and `endDate`) and time of the day (`beginTime` and `endTime`) separately. Note that `beginDate` and `endDate` are *bare dates*, which means that they just specify the day and month, not the year. This means that you can have a calendar interval across 1 January, e.g. from 1 December (`beginDate`) to 1 March (`endDate`). All intervals of dates and times are closed (i.e. limits are inclusive). If `beginTime` and `endTime` keep their default value (`00:00`), the time of day is ignored.

If `circadian` is `TRUE` then `currentDateTime` is considered inside the limits, if the date of `currentDateTime` is inside the [`beginDate`;`endDate`] period, and the time of `currentDateTime` is inside the [`beginTime`;`endTime`] period (specifying a recurring signal). 

If `circadian` is `FALSE` then `currentDateTime` is considered inside the limits, if it falls inside the period from `beginTime` on `beginDate` to `endTime` on `endDate` (specifying a single signal). 

#### Circadian signals

The difference that `circadian` makes is easier to understand by example. First we demonstrate a circadian signal:

```
// date_time_signal1.box
Simulation sim {
  Calendar calendar {
    .begin = 01/01/2001T12:00
    .end   = 10/01/2001T12:00
    .timeStep = 15
    .timeUnit = "m"
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
        .ports = signal[value] | signal[flagIsUp] | signal[flagJustRaised] | signal[flagJustLowered]
        .ggplot = "scale_x_datetime(breaks=date_breaks('days'), 
                                    labels = date_format ('%d')) +
                   labs(x='January')"
      }
    }
  }
}
```

The boxscript raises the signal for five days between 14:00 and 20:00 every day, as shownin the output:

![](../media/demo/date_time_signal1.png)

You can check the precision of the signal going on at the Universal Simulator prompt with the `rows 295 300` command:

```
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

```
> sim[295:300,]
               dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
295 2001-01-04 13:30:00        0              0               0     50         1  294
296 2001-01-04 13:45:00        0              0               0     50         1  295
297 2001-01-04 14:00:00        1              1               0     80         1  296
298 2001-01-04 14:15:00        1              0               0     80         1  297
299 2001-01-04 14:30:00        1              0               0     80         1  298
300 2001-01-04 14:45:00        1              0               0     80         1  299
```

Similarly, you can check the timeliness of the signal going off:

```
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

#### Non-circadian signals

In the `date_time_signal2.box` script, we only changed one detail, namely the value of `circadian` in the `signal` box:

```
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

We also re-arranged the ordering of the date and time inputs to make their meaning more clear.

The output shows that the signal is raised only through one time period stretching 4 days and 6 hours:

![](../media/demo/date_time_signal2.png)

The precise timing is readily revealed:

```
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

**Alternatively**, we can ignore the time of day all together, as shown in the `date_time_signal3.box` script:

```
  DateTimeSignal signal {
    .beginDate  = 03/01/2001
    .endDate    = 07/01/2001
    .initialSignal = 50
    .signalInside  = 80
    .signalOutside = 50
    .circadian = FALSE
  }
```

This gives us five full days with the signal on:

```
> rows 143 146
...
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/02T23:30:00        0              0               0   50.0         1  142
2001/01/02T23:45:00        0              0               0   50.0         1  143
2001/01/03T00:00:00        1              1               0   80.0         1  144
2001/01/03T00:15:00        1              0               0   80.0         1  145
Showing D:/Documents/QDev/UniSim3/output/date_time_signal3_0002.txt
           dateTime flagIsUp flagJustRaised flagJustLowered signal iteration step
2001/01/07T23:30:00        1              0               0   80.0         1  622
2001/01/07T23:45:00        1              0               0   80.0         1  623
2001/01/08T00:00:00        0              0               1   50.0         1  624
2001/01/08T00:15:00        0              0               0   50.0         1  625
```

## DayDegrees

### Interface

The inputs and outputs written in *italics* are inherited from the [PhysiologicalTime](#physiologicaltime) box. See explanation there.


| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|*T*           |double|*computed*|*= weather[Tavg]*|
|*timeStepDays*|double|*computed*|*= calendar[timeStepDays]*|
|*resetTotal*  |bool  |    FALSE []   |Reset total to zero?|
|*isTicking*   |bool  |     TRUE []   |Is the physiological time running? Else step will be zero|
|T0          |double|      0.0 []   |Lower temperature threshold|
|Topt        |double|    100.0 []   |Optimum temperate; linear decline from here to Tmax|
|Tmax        |double|    100.0 []   |Upper temperature threshold|
| **Outputs** | | | |
|*step*        |double|          [PTU]|Increment in physiological time units|
|*total*       |double|          [PTU]|Total time since reset|

### Usage

The DayDegrees box implements the classical, linear day-degree scale of physiological development. The `briere1.box` script (see [Briere](#briere)) compares linear and non-linear models:

![](../media/demo/briere1.png)

Here, the brown line shows a pure linear model, while the broken green line shows how DayDegrees work. The lower threshold for development (`T0`) was set to 14.2&#176;C in this example with optimum temperature (`Topt`) at 29.0&#176;C and maximum temperature (`Tmax`) at 35.0&#176;C. 

If you choose only to set `T0` in your boxscsript, while keeping the default values of `Topt` and `Tmax`, you will achiece the pure linear model (brown line).

This boxscript is identical to the `briere2.box` script, except a `DayDegree` box has replaced the `Briere` box:

```
// day_degrees1.box
Simulation sim {
  Calendar calendar {
    .begin = 01/01/2009
    .end   = 31/12/2010
  }
  Records weather {
   .fileName = "weather/flakkebjerg 2009.txt"
   .cycle = TRUE
  }
  DayDegrees clock {
    .isTicking = (calendar[date] >= 01/05) && (calendar[date] < 01/10)
    .resetTotal = (calendar[date] == 01/05) || (calendar[date] == 01/07)
    .T0 = 8
    .Topt = 29
    .Tmax = 35
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .ports = clock[output::*] | clock[isTicking] | clock[resetTotal]
        .ggplot = "scale_x_datetime(breaks=date_breaks('3 months'), 
                                    labels = date_format ('%b')) +
                   labs(x='2009 - 2010')"
      }
    }
  }
}
```

The output of the boxscript is much the same as that of the `briere2.box` script, because ambient temperature remained mostly in the linear part of the temperature response:

![](../media/demo/day_degrees1.png)

For further explanation of the output, please see [Briere](#briere).

## FixedSignal

### Interface

The inputs and outputs written in *italics* (i.e., all of them) are inherited from the [BaseSignal](#basesignal) box. See explanation there.

| Inputs            | Type     | Default     | Description                                                  |
| ----------------- | -------- | ----------- | ------------------------------------------------------------ |
| *initialSignal*   | double   | 0.0 []      | The value that signal is set to at reset                     |
| *initialFlag*     | bool     | FALSE []    | The value that flag is set to at reset                       |
| **Outputs**       |          |             |                                                              |
| *signal*          | double   | []          | Value of the signal                                          |
| *flagIsUp*        | bool     | []          | Is the signal different from zero?                           |
| *flagJustRaised*  | bool     | []          | Did flagIsUp change from false to true during the latest update? |
| *flagJustLowered* | bool     | []          | Did flagIsUp change from true to false during the latest update? |
| *value*           | double   | []          | Synonym for 'signal'                                         |

### Usage

The `FixedSignal` simply fixes the signal at the value given as `initialSignal`. It is useful when you have a group of boxes (e.g., of sibling boxes) from which you want to select all the boxes of type `BaseSignal`. Here is the beginning of a boxscript. It does not yet produce any output:

```
// fixed_signal1.box
Simulation sim {
  Calendar calendar {
    .begin = 01/01/2001
    .end   = 31/12/2001
  }
  Box vivacity {
    Accumulator accumulator {
    }
    DateTimeSignal summer {
      .beginDate  = 01/06
      .endDate    = 31/08
      .signalInside = 25
    }
    DateTimeSignal autumn {
      .beginDate  = 01/09
      .endDate    = 30/11
      .signalInside = 5
    }
    FixedSignal anyTime {
      .initialSignal = -10
    }
  }
}
```

You can select the `value` port of any child box inside `vivacity` with the path `vivacity/*[value]` but, more to the point, you can select only those belonging to boxes of the `BaseSignal` base class with `vivacity/BaseSignal::*`[value]:

```
> find vivacity/*[value]
Port double sim/vivacity/accumulator[value]
Port double sim/vivacity/summer[value]
Port double sim/vivacity/autumn[value]
Port double sim/vivacity/anyTime[value]
> find vivacity/BaseSignal::*[value]
Port double sim/vivacity/summer[value]
Port double sim/vivacity/autumn[value]
Port double sim/vivacity/anyTime[value]
```

You may find it enlightning to leave out the `[value]` part of the path:

```
> find vivacity/*
Accumulator    sim/vivacity/accumulator
DateTimeSignal sim/vivacity/summer
DateTimeSignal sim/vivacity/autumn
FixedSignal    sim/vivacity/anyTime
> find vivacity/BaseSignal::*
DateTimeSignal sim/vivacity/summer
DateTimeSignal sim/vivacity/autumn
FixedSignal    sim/vivacity/anyTime
```

## Hump

### Interface


| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|x      |double|    0.0 []|x-value at which to calculate curve value|
|x0     |double|    0.0 []|Beginning of curve|
|x1     |double|    1.0 []|End of curve|
|ymin   |double|    0.0 []|Minimum value of curve|
|ymax   |double|    1.0 []|Maximum value of curve|
|ditch  |bool  |  FALSE []|Is the curve a ditch?|
| **Outputs** | | | |
|value  |double|        []|Curve value at x-value|

### Usage

You use the `Hump` box to produce a generic, symmetric hump-or ditch-shaped curve with set limits on the x- and y-axis. The curve is a fourth degree polynomium:
$$
f_{hump}(x) &= y_{min} + g(x)  \\
f_{ditch}(x) &= y_{max} - g(x) 
$$
where
$$
g(x)= \begin{cases}
{(y_{max}-y_{min})\left[4{{(x-x_0)(x-x_1)}\over{(x_0-x_1)(x_1-x_0)}}\right]^2} & \text{for} \ x \ \epsilon \ [x_0;x_1] \\
0 & \text{otherwise}
\end{cases}
$$

The following script demonstrates the shapes that can be obtained:

```
// hump1.box
Simulation sim {
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
}
```

Here is the output:

![](../media/demo/hump1.png)

The curve takes off exactly from its cloor or ceiling exactly at the limits `x0` and `x1` as you can see by the `rows` command:

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

### MaximumAt

### Interface

| Inputs      | Type   | Default       | Description                              |
| ----------- | ------ | ------------- | ---------------------------------------- |
| step        | int    | *computed* [] | */.[step]*                               |
| value       | double | 0.0 []        | Value to track for max value             |
| **Outputs** |        |               |                                          |
| at          | int    | []            | Step in which maximum value was attained |
| max         | double | []            | The maximum value attained               |

### Background

If you need to summarise the trend of a port by the maximum value it attained and in which step it happened, you can use a `MaximumAt` box. The interface is simple. Just watch the example below. If you run the simulation for several iterations, the outputs `at` and `max` may differ between iterations.

### Usage

 We will use a [Hump](#hump) box for demonstration since we now the position of its maximum value:

```
// maximum_at1.box
Simulation sim {
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
        .ggplot = "geom_vline(xintercept=34, colour='black', linetype='dashed')"
      }
    }
  }
}
```

With this parameter setting we expect to find the maximum in the point (34, 150). This is confirmed when we run the script,

```
run demo/maximum_at/maximum_at1.box
```

and study the output:

<img src="../media/demo/maximum-at1.png" style="zoom:50%;" />

Further confirmation can be gotten by the [tail](#tail) command:

```
> tail 3
...
hump at maxValue iteration step
 0.0 34    150.0         1   48
 0.0 34    150.0         1   49
 0.0 34    150.0         1   50
```
## Maker

## MaxAt

## OnOff

## OutputR

## OutputSelector

### Interface

| Inputs              | Type     | Default           | Description                                                  |
| ------------------- | -------- | ----------------- | ------------------------------------------------------------ |
| beginStep           | int      | 0 [int]           | Output is written when this step is reached                  |
| beginDateTime       | datetime | null [DateTime]   | Output is written when this time point is reached (optional) |
| step                | int      | *computed* []     | */.[step]*                                                   |
| dateTime            | datetime | *computed* []     | *if exists(Calendar&#58;&#58;\*[dateTime]) then Calendar&#58;&#58;\*[dateTime] else .[beginDateTime]* |
| period              | int      | 1 [int>0]         | If &#62;1&#58; A row of output will be produced with this period |
| final               | bool     | FALSE [bool]      | Overrules 'period'                                           |
| useLocalDecimalChar | bool     | FALSE [bool]      | Use local decimal character in output?                       |
| skipFormats         | bool     | *computed* [bool] | *!exists(OutputR&#58;&#58;\*[skipFormats])*                  |
| **Outputs**         |          |                   |                                                              |
| isActive            | bool     | [bool]            | Should output be written?                                    |
| isSkipping          | bool     | [bool]            | Are lines being skipped?                                     |

### Usage

The `OutputSelector` box determines when output should be produced by the `OutputWriter` box. Hence, if you do not include an `OutputSelector` box in your boxscript then the `OutputWriter` box will create one for you. The default behaviour of `OutputSelector` is to write one line of output for every *reset* and *update* step during the simulation (see [Computationel model](#computational-model)). The `useLocalDecimalChar` and `skipFormats` define different features of the output file as explained for [OutputWriter](#outputwriter).

If you want to change the default behaviour of `OutputSelector`, you must include an `OutputSelector` box in your boxscript. The right place to put it is as a child box of the `OutputText` (as exemplified below) or `OutputR` box, since `OutputSelector` affects the output produced by `OutputText` and `OutputR`.

#### Skip a number of lines

You can set the number `steps` and `iterations` a  simulation is running in the [Simulation](#simulation) object. That will by itself result in `iterations` * (`steps` + 1) lines in the output file (adding one for each iteration because a line is written also in the reset step). You can skip a number of initial lines in each iteration by setting the `beginStep` of the `OutputSelector` box. Maybe the model needs some warming up before it produces reliable or relevant output. Here is one simple example:

```
// output_selector1.box
Simulation sim {
  .iterations = 2
  .steps = 12
  Calendar calendar {
  }
  OutputText {
    .ports = calendar[date]
    OutputSelector {
      .beginStep = 10
    }
  }
}
```

When you run it

```
run demo/output_selector/output_selector1.box
```

and subsequently inspect the few lines produced with the `head` command, you won't be surprised:

```
> head
Showing C:/Users/au152367/Documents/QDev/UniSim3/output/output_selector1_0001.txt
      date iteration step
2000/01/11         1   10
2000/01/12         1   11
2000/01/13         1   12
2000/01/11         2   10
2000/01/12         2   11
2000/01/13         2   12
>
```

Well, unless you had not anticipated that `beginStep` is acting on every `iteration`.

#### Skip lines until a certain time

As an alternative to `beginStep`, you can set `beginDateTime` to commence output at a certain date-time. You can combine them both, which will postpone output until both a certain step and a certain date-time is reached, though I never found the need to do so. Here is a simple example, working in 5-minute time steps

```
// output_selector2.box
Simulation sim {
  .iterations = 2
  Calendar calendar {
    .begin = 01/08/2022T12:00
    .end = 01/08/2022T12:40
    .timeStep = 5
    .timeUnit = "m"
  }
  OutputText {
    .ports = calendar[dateTime]
    OutputSelector {
      .beginDateTime = 01/08/2022T12:30
    }
  }
}
```

Run the simulation

```
run demo/output_selector/output_selector2.box
```

and again use `head` to see the expected output:

```
> head
...
           dateTime iteration step
2022/08/01T12:30:00         1    6
2022/08/01T12:35:00         1    7
2022/08/01T12:40:00         1    8
2022/08/01T12:30:00         2    6
2022/08/01T12:35:00         2    7
2022/08/01T12:40:00         2    8
```

#### Periodic output

You can write only every other line by setting `period` = 2, every third line by `period` = 3, and so forth. This can be combined with `beginStep` and `beginDateTime` as you wish:

```
// output_selector3.box
Simulation sim {
  .iterations = 2
  .steps = 25
  Calendar calendar {
  }
  OutputText {
    .ports = calendar[date]
    OutputSelector {
      .beginStep = 10
      .period = 6
    }
  }
}
```

Notice that the final output written for each iteration is in step 22, even though an iteration lasts 25 steps. Note also that the period counts from the first line written in each iteration (in step 10), not from the beginning of the simulation in each iteration (step 0):

```
> head
...
      date iteration step
2000/01/11         1   10
2000/01/17         1   16
2000/01/23         1   22
2000/01/11         2   10
2000/01/17         2   16
2000/01/23         2   22
```

#### Final output

If you want only the final values to appear in the output, i.e. those in the last step, then you set `final` to `TRUE`. This will overrule any of the settings on `beginStep`, `beginDateTime` and `period`. Here is a simple boxscript:

```
// output_selector4.box
Simulation sim {
  .iterations = 3
  .steps = 25
  Calendar calendar {
  }
  OutputText {
    .ports = calendar[date]
    OutputSelector {
      .final = TRUE
    }
  }
}
```

When you run the script,

```
run demo/output_selector/output_selector4.box
```

and look at the output, you will find that only the final step 25 of each of the three iterations appears in the output:

```
> head 20
...
      date iteration step
2000/01/26         1   25
2000/01/26         2   25
2000/01/26         3   25
> 
```

If you run an uncertainty or sensitivity analysis, you will usually set `final` to `TRUE`.

## OutputText

## OutputWriter

### Interface

| Inputs              | Type   | Default       | Description                                                  |
| ------------------- | ------ | ------------- | ------------------------------------------------------------ |
| ports               | path   | *computed* [] | */.[iteration] &#124; /.[step] &#124; OutputText&#58;&#58;\*[ports] &#124; OutputR&#58;&#58;\*[ports]* |
| skipFormats         | bool   | *computed* [] | *OutputSelector&#58;&#58;\*[skipFormats]*                    |
| useLocalDecimalChar | bool   | *computed* [] | *OutputSelector&#58;&#58;\*[useLocalDecimalChar]*            |
| isActive            | bool   | *computed* [] | *OutputSelector&#58;&#58;\*[isActive]*                       |
| **Outputs**         |        |               |                                                              |
| filePath            | string | []            | Name of output file including absolute path                  |
| decimalChar         | string | []            | Decimal character used in output                             |

### Usage

You'll never need to include an `OutputWriter` box in a boxscript explicitly. It will be created automaticall by boxes that need it, such as [OutputR](#outputr) and [OutputText](#outputtext). It is the sole responsibility of `OutputWriter` to write the simulation output to a file. All of its inputshas been given default expressions, which look up the relevant data in other boxes. You set *what* to change what is written in the [OutputR](#outputr) or [OutputText](#outputtext) box and you can change *how* it is written in the [OutputSelector](#outputselector) box.

`OutputWriter` depends much on `OutputSelector`, in fact, so much that if no `OutputSelector` box is present in the boxscript then the `OutputWriter` box will create an `OutputSelector` as a child box. 

The `sun1.box` script, for example, includes an `OutputR` box explicitly in the script. In a chain reaction, the `OutputR` box creates an `OutputWriter` box, which again creates an `OutputSelector` box:

```
> load demo/sun/sun1.box
Constructing...
Amending...
8 boxes created
> list
Simulation sim
  Calendar calendar
  Sun sun
  OutputR 
    PageR 
      PlotR 
  OutputWriter outputWriter
    OutputSelector selector
>
```

Most often you are not interested in the output file, as Universal Simulator does its best to handle all output automatically behind the scenes but, anyway, here are the details.

#### The output folder

The output file will be written to your output folder, which you can locate with the  [get folders](#get-folders) command:

```
get folders
```

The name of the output file will be the same as the boxscript that produced it, just with a four-digit running number at the end. As a pure text file, its suffix (also known as its file type) is "txt". The file consists of tab-separated columns, separated by tab characters (no, you cannot change the separator character to something else than a tab). 

The output folder will grow slowly for every run you make. Therefore, you should sometimes empty it.

#### Output file format

The first line contains unique column names constructed from the names of the `ports` included. The column names have been shortened as much as possible while keeping them unique. Ports named `value` will have a column name without the port part if possible; they will be named after their parent box instead ("value" is not an informative label).

If `skipFormats` is `FALSE`, which it defaults to because that's the default value imported from [OutputSelector](#outputselector), then the second line in the output file will be information about the format of the values in the column. These format specifications are used when the file is read into R as a data frame. If you want to do all the handling of the output file yourself then set `skipFormats` to `TRUE` in the `OutputSelector` box.

If you want to use the decimal character specific to the language settings on your computer then set `useLocalDecimalChar` to `TRUE` in the `OutputSelector` box; otherwise a decimal point will be used.

## PageR

## PlotR

## PopUp

## PhysiologicalTime

### Interface


| Inputs       | Type   | Default    | Description                                               |
| ------------ | ------ | ---------- | --------------------------------------------------------- |
| T            | double | *computed* | *= weather[Tavg]*                                         |
| timeStepDays | double | *computed* | *= calendar[timeStepDays]*                                |
| resetTotal   | bool   | FALSE []   | Reset total to zero?                                      |
| isTicking    | bool   | TRUE []    | Is the physiological time running? Else step will be zero |
| **Outputs**  |        |            |                                                           |
| step         | double | [PTU]      | Increment in physiological time units                     |
| total        | double | [PTU]      | Total time since reset                                    |

### Usage

`PhysiologicalTime` is the base class for two derived `Box` classes:

* [Briere](#briere)
* [DayDegrees](#daydegrees)

`PhysiologicalTime` cannot be used directly in a boxscript, only indirectly through one of its derived classes. The input and outputs of `PhysiologicalTime` have the same purpose in both the derived classes as described here.

The temperature `T` defaults to the `Tavg` port of a `weather`, which would typically be a `Records` box reading a weather log with a column labelled `Tavg`. Feel free to change it in your boxscript.

It is necessary to know the time step of the simulation to calculate the time step in physiological time units. The `timeStepDays` input acquires this information from the `calendar` box. You should change this in your boxscript, only if you have no `calendar` box.

In practical usage, you may want to turn the ticking of a physiological clock on and off. Sometimes, you may also want to reset it to zero too. This is what the input ports `isTicking` and `resetTotal`, respectively, are for. You can find examples of this under [Briere](#briere).

## PrioritySignal

## ProportionalSignal

## RandomBase

## RandomiserBase

### Interface

| Inputs                | Type | Default       | Description                                                  |
| --------------------- | ---- | ------------- | ------------------------------------------------------------ |
| iteration             | int  | *computed* [] | */.[iteration]*                                              |
| iterations            | int  | *computed* [] | */.[iterations]*                                             |
| doSensitivityAnalysis | bool | FALSE []      | Carry out a sensitivity analysis?                            |
| bootstrapSize         | int  | 1000 []       | Size of bootstrap sample (cheap in computation time); only used in sensitivity analysis |
| seed                  | int  | 0 []          | Seed for random numbers; if this is zero a random seed value will be used |
| drawAtInitialize      | bool | FALSE []      | Draw a value when a box is initialized?                      |
| drawAtReset           | bool | TRUE []       | Draw a value when a box is reset?                            |
| drawAtUpdate          | bool | FALSE []      | Draw a value when a box is updated?                          |
| **Outputs**           |      |               |                                                              |
| numVariables          | int  | [0;inf)       | Number of variables which are randomised                     |

### Background

There will be only one source of random numbers in your boxscript. This will be a box of one of the classes:

* [RandomiserMonteCarlo](#randomisermontecarlo)
* [RandomiserSobolSequence](#randomisersobolsequence)
* [RandomiserStratified](#randomiserstratified)

These box class have the same interface defined by `RandomiserBase`. You cannot use `RandomiserBase` directly in your boxscript (in C++ parlance it is virtual); it only serves as a base class for the three classes above, which you can use&#8212;but only one of them and only once in a boxscript.

You can change the `seed` to a positive value, if you want the same sequence of pseudo-random numbers every time you run the boxscript. For a `RandomiserSobolSequence` box, though, the `seed` has no effect because this box will produce the same sequence of quasi-random numbers always.

By default random numbers will be drawn in every **reset** step (see [computational model](#computational-model)), as indicated by `drawAtReset`. Other options (they can be combined) are `drawAtInitialize` and `drawAtUpdate`. If you choose to draw random numbers at every **update**, you will end up with a **stochastic model**. The default setting will change model behaviour for every iteration that you run the model. This is useful for **uncertainty analysis** and **sensitivity analysis**. If you want to carry out a sensitivity analysis then set `doSensitivityAnalysis` to `TRUE` and consider changing `bootstrapSize` to 10,000 for serious use.

The single output `numVariables` is mysterious. It tells you how many variables are randomised, yet there are no inputs to indicate which variables to randomise? That's because a certain structure of boxes is assumed, as exemplified by this box from the `randomiser1.box` script:

```
  Box random {
    RandomiserMonteCarlo randomiser {
    }
    RandomUniformInt k {
      .min = 10
      .max = 30
    }
    RandomNormal r {
      .min = 10
      .max = 30 
    }
  }
```

The box names (`random`, `randomiser`, `k` and `r`) are all up to you to choose but are used here for reference. There must be one `randomiser` box with any number of sibling boxes. The sibling boxes of classes derived from [RandomBase](#randombase) represent the random variables. Here there are two, `k` and `r`. There can be other sibling boxes beside that but you should always have the randomiser as the top-most child. Arrange it all neatly within a common parent box, here `random`.

### Usage

#### Stochastic modelling

It is easiest to understand how to set up a stochastic model by starting out with one that is not stochastic, yet makes use of random numbers. This is exemplified by the `randomiser1.box` script:

```
// randomiser1.box
Simulation sim {
  .iterations = 4
  .steps = 5
  Box random {
    RandomiserMonteCarlo randomiser {
    }
    RandomUniformInt k {
      .min = 10
      .max = 30
    }
    RandomNormal r {
      .min = 10
      .max = 30 
    }
  }
  OutputR {
    PageR {
      PlotR {
        .ports = random/*[value]
        .layout = "merged"
        .ggplot = "geom_point(size=2)"
      }
    }
  }
}
```

Run the model,

```
run demo/randomiser/randomiser1.box
```

and study the output:

<img src="../media/demo/randomiser1.png" style="zoom:50%;" />

The four panels show the four iterations of the model. Within each iteration the variables remain constant. It is only from one iteration to the next that they attain new values inside their designated intervals (which happens to be [10;30] for both of them). This set-up of the random variables are useful for an uncertainty or a sensitivity analysis.

Only little changed in the next boxscript. Most importantly, we are now drawing random numbers for every simulation update:

```
// randomiser2.box
Simulation sim {
  .iterations = 4
  .steps = 100
  Box random {
    RandomiserMonteCarlo randomiser {
      .drawAtUpdate = TRUE
    }
    RandomUniformInt k {
      .min = 10
      .max = 30
    }
    RandomNormal r {
      .min = 10
      .max = 30 
    }
    OutputR {
      PageR {
        PlotR {
          .ports = random/*[value]
          .layout = "merged"
        }
      }
    }
  }
}
```

Run the model,

```
run demo/randomiser/randomiser2.box
```

and marvel at the changed behaviour of the model. We are now running each of the four iterations for 100 steps to highligh the dynamics:

<img src="../media/demo/randomiser2.png" style="zoom:50%;" />

This shows the behaviour of a stochastic model. Use those two variables elsewhere in your model as inputs to other boxes, and your model will behave erratically, hopefully within meaningful boundaries. You would refer to the current values by `random/k[value]` and `random/r[value]`.

#### Uncertainty analysis

In uncertainty analysis we study  the uncertainty on model outputs induced by uncertainty in model parameters. We have take the introductory `demo/butterfly.box` script and made two of the model parameters uncertain, here in `randomiser3.box`

```
// randomiser3.box
Simulation sim {
  .iterations = 4
  Calendar calendar {
    .begin = 01/05/2009
    .end   = 30/09/2009
  }
  Records weather {
    .fileName = "weather/flakkebjerg 2009.txt"
  }
  Box random {
    RandomiserStratified randomiser {
    }
    RandomUniformInt k {
      .min = 10
      .max = 30
    }
    RandomUniform deltaT {
      .min = -3.0
      .max =  3.0
    }
  }
  Box butterfly {
    DayDegrees time {
      .T0 = 5
      .T  = weather[Tavg] + random/deltaT[value]
    }
    Stage egg {
      .initial  = 100 
      .duration = 140
      .timeStep = ../time[step]
      .k = random/k[value]
    }
    Stage larva {
      .inflow   = ../egg[outflow]
      .duration = 200
      .timeStep = ../time[step]
      .k = random/k[value]
    }
    Stage pupa {
      .inflow   = ../larva[outflow]
      .duration = 100
      .timeStep = ../time[step]
      .k = random/k[value]
    }
    Stage adult {
      .inflow   = ../pupa[outflow]
      .duration = 28
      .timeStep = 1
      .k = random/k[value]
    }
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .ports = Stage::*[value]
        .layout = "merged"
      }
    }
  }
}
```

The parameters chosen to be uncertain are `k` used as input to the [Stage](#stage) boxes and ambient temperature. Temperature enters the model from the [Records](#records) box, which reads it from a weather file. Assuming the ambient temperature is an uncertain measure for the microclimate experienced by all life stages of the butterfly population, an uncertainty of &plusmn;3&#8451; was added to the temperate. This is represented by the [RandomUniform](#randomuniform) variable `deltaT` (well,  more precisely,`deltaT` is a box; the variable value is provided by its `value` output).

You can run the boxscript,

```
run demo/randomiser/randomiser3.box
```

and study the uncertainty in the output:

<img src="../media/demo/randomiser3.png" style="zoom:50%;" />

An alternative presentation of the uncertainty is shown in the next boxscript, running 30 iterations:

```
run demo/randomiser/randomiser4.box
```

There are 30 curves in each panel, representing the course of life stages phenology for each of the 30 iterations:

<img src="../media/demo/randomiser4.png" style="zoom:60%;" />

How about summarising the 30 curves in the panels for larva, pupa and adult by the date of their maximum? Then we could show the distribution of these dates as ann alternative illustration of the uncertainty. That's what we'll do in the next script. Here are the final lines of the `randomiser5.box` script:

```
Box max {
  MaximumAt larva {
    .track = butterfly/larva[value]
  }
  MaximumAt pupa {
    .track = butterfly/pupa[value]
  }
  MaximumAt adult {
    .track = butterfly/adult[value]
  }
}
OutputR {
  .scripts = "randomiser5.R"
  OutputSelector {
    .final = TRUE
  }
  OutputText {
    .ports = max/*[at]
  }
}
```

We use three [MaxAt](#maxat) boxes to catch the simulation step, in which their maximum is reached. By setting `final` to `TRUE` in the [OutputSelector](#outputselector), we get only one line of out from each simulation iteration. Let's run that,

```
run demo/randomiser/randomiser5.box
```

The `randomiser5.R` script grabbed the output to show these `geom_density` plots:
<img src="../media/demo/randomiser5.png" style="zoom:60%;" />

That's a lot of uncertainty! The expected date of maximum occurence stretches over a month for all three life stages. The question is, which of the two uncertain parameters `k` or `deltaT` is the main cause of this uncertainty? That's what we'll find out next in the sensitivity analysis.

#### Sensitivity analysis

Sensitivity analysis demands many simulation iterations. Therefore, you should use the randomiser that most efficiently explores the *n*-dimensional space formed by your *n* uncertain model parameters. That means you should use [RandomiserSobolSequence](#randomisersobolsequence). This has been set up in the `randomiser6.box` script like so:

```
// randomiser6.box
Simulation sim {
  .iterations = 1024
  .silent = TRUE
  Calendar calendar {
    .begin = 01/05/2009
    .end   = 30/09/2009
  }
  Records weather {
    .fileName = "weather/flakkebjerg 2009.txt"
  }
  Box random {
    RandomiserSobolSequence randomiser {
      .doSensitivityAnalysis = TRUE
      .bootstrapSize = 1000
    }
    RandomUniformInt k {
      .min = 10
      .max = 30
    }
    RandomUniform deltaT {
      .min = -3.0
      .max =  3.0
    }
  }

```

When we are using Sobol' numbers we should be careful to make a balanced sampling of parameter space. To obtain that, the number of simulation `iterations` must equal  $N(p+2)$ with $N=2^n$, where  $n$ is a whole, positive number, and  $p$ is the number of parameters sampled (we've got 2). $N$ is known as the sample size of the sensitivity analysis. These are all valid options for $p=2$:
$$
\begin{align*}
2^1\cdot(2+2) &= 8 \\
2^2\cdot(2+2) &= 16 \\
2^3\cdot(2+2) &= 32 \\
\ldots \\
\end{align*}
$$
If you set `iterations` to an invalid number (which depends only on your value of $p$), the simulation will stop immediately if you try to run it. As a service, you will get suggestions to valid values for `iterations` near the number you picked. So, if you want in the range of, say, 50,000 iterations, write that, and you will be told nearby valid numbers.

To carry out the sensitivity analysis on the simulation output, we must set `doSensitivityAnalysis` to `TRUE` and choose a reasonable `bootstrapSize` to do statistics on the sensitivity indices. A `bootstrapSize` of 10,000 seems to be a standard choice in literature but here we'll use 1000 for demonstration.

The [OutputR](#outputr) box  in the `randomiser6.box` script will show three pages of output produced by [PageR](#pager) boxes:

```
OutputR {
  OutputSelector {
    .final = TRUE
  }
  PageR {
    .xAxis = random/*[value]
    PlotR {
      .ports = max/*[at]
      .maxData = 1000
      .ggplot = "geom_smooth(colour='yellow')"
    }
  }  
  PageR {
    .xAxis = random/*[value]
    PlotR {
      .ports = max/*[at]
      .type = "SobolConvergence"
    }
  }
  PageR {
    .xAxis = random/*[value]
    PlotR {
      .ports = max/*[at]
      .type = "SobolIndices"
    }
  }
}
```

We invoke the [OutputSelector](#outputselector) to write only the `final` values of each iteration. For each of the `PageR` boxes, you must put all the uncertain parameters on the `xAxis`. You can use the [find](#find) command to check whether you got it right:

```
> load demo/randomiser/randomiser6.box
...
> find random/*[value]
Port int    sim/random/k[value]
Port double sim/random/deltaT[value]
```

Likewise, you must put all the model response variables into the `ports` of the [PlotR](#plotr) boxes:

```
> find max/*[at]
Port int sim/max/larva[at]
Port int sim/max/pupa[at]
Port int sim/max/adult[at]
```

Now, it's time to run the model,

```
run demo/randomiser/randomiser6.box
```

The 1024 iterations took 31 seconds on my machine. Paste into R and wait for the 1000 bootstraps statistics to finish. That took 14 seconds for me. For larger models, you might need >100,000 iterations while 10,000 bootstraps should suffice. For hours' long simulation, you should `unattended` to `TRUE` in the [Simulation](#simulation) box.

The first plot represents each iteration result asa dot in each. Remember that the model responses on the y-axis is the simulation step (i.e., days after 30 April), when the maximum value was attained:

![](../media/demo/randomiser6a.png)

Clearly, `k` had no influence, while `deltaT`  had a very clear-cut influence. Don't expect such simplicity coming out of more complex models!

The second plot shows you the increased precision of the estimates of the Sobol' indices:

![](../media/demo/randomiser6b.png)

We had sample size $N=256$. The plot shows what would have happened with smaller $N$. The estimates, shown for `deltaT`, `k` and their sum, are clearly converging and not changing much from $N=128$ top $N=256$. We conclude that we don't need to increase the sample size.

The final plot shows the Sobol' indices for each of the model responses:

![](../media/demo/randomiser6c.png)

The uncertain model parameters are sorted for each model response in decreasing order of importance. We are not surprised that the sensitivity indices for `k` are both nil, while they are both 1 for `deltaT`. This stresses that `deltaT` is not interacting with `k` to determine model uncertainty. In the cereal aphid-fungus model, you can find [a more complicated sensitivity analysis](#biocontrol-with-sensitivity-analysis).

Since the simulation and bootstrapping computations may take a very long time, you should be aware how to save the results. How to do that is described for the cereal aphid-fungus model under [Saving and restoring the analysis](#saving-and-restoring-the-analysis).

## RandomiserMonteCarlo

### Interface

See [RandomiserBase](#randomiserbase).

### Background

You can use `RandomiserMonteCarlo` for stochastic modelling and uncertainty analysis, but [RandomiserStratified](#randomiserstratified) will explore the parameter space more efficiently. For sensitivity analysis, [RandomiserSobolSequence](#randomisersobolsequence) is the preferred choice because it is the most effective of the three in covering all of the parameter space.

The `randomiser7.box` script shows the differences, subtle but important, between the three randomisers. The boxscript breaks the general rule not to put more than one randomiser box into a script. The only good reason would be for a direct comparison like here. Each of the three randomisers is the source of random numbers used to generate random numbers for two variables `x` and  `y`, both with a uniform distribution of  numbers in the interval [20;40) (see [RandomUniform](#randomuniform)).

```
// randomiser7.box
Simulation sim {
  .iterations = 32
  .steps = 1
  .silent = TRUE
  Box monteCarlo {
    RandomiserMonteCarlo randomiser {
    }
    RandomUniform x {
      .min = 20
      .max = 40
    }
    RandomUniform y {
      .min = 20
      .max = 40
    }
  }
  Box stratified {
    RandomiserStratified randomiser {
    }
    RandomUniform x {
      .min = 20
      .max = 40
    }
    RandomUniform y {
      .min = 20
      .max = 40
    }
  }
  Box sobolSequence {
    RandomiserSobolSequence randomiser {
    }
    RandomUniform x {
      .min = 20
      .max = 40
    }
    RandomUniform y {
      .min = 20
      .max = 40
    }
  }
  OutputR {
    .scripts = "randomiser7.R"
    OutputSelector {
      .final = TRUE
    }
    OutputText {
      .ports = sim/*/*[value]
    }
  }
}
```

Run the boxscript,

```
run demo/randomiser/randomiser7.box
```

and study the output:

![](../media/demo/randomiser7.png)

The three ransomisers worked like this to generate random numbers:

* `RandomiserMonteCarlo` picked 32 numbers at random from the interval [20;40) for both `x` and `y`.
* `RandomiserStratified` first divided the interval [20;40] into 32 slices (strata) of equal area for both `x` and `y`. Then it shuffled the order of these slices at random for both `x` and `y`. Then it picked a random number from within each slice (stratum).
* `RandomiserSobolSequence` picked the first 32 number from the Sobol' sequence for two variables and assigned them to `x` and `y`.

At the bottom you see the distribution of `x`. The Monte Carlo method yielded the least uniform and the Sobol' sequence the most uniform distribution.

At the top you see the joint distribution, where the Sobol' sequence stands out with the best coverage of the 2-dimensional variable space; this is what Sobol' designed his sequence for. Moreover, the good coverage of Sobol' numbers is still obtained with many variables in n-dimensional space.

The `randomiser` box is ignorant about the statistical distributions that are generated from. Behind the scenes it just produces random numbers in the interval [0;1], which are then grabbed by its sibling random boxes (in this example [RandomUniform](#randomuniform)) and transformed into the appropriate distribution. For another distribution than uniform, you can check out [RandomNormal](#randomnormal), for example.

### Usages

 See examples in [RandomiserBase](#randomiserbase).

## RandomiserSobolSequence

See [RandomiserMonteCarlo](#randomisermontecarlo).

## RandomiserStratified

See [RandomiserMonteCarlo](#randomisermontecarlo).

## RandomBase

### Interface

| Inputs           | Type   | Default       | Description                                                  |
| ---------------- | ------ | ------------- | ------------------------------------------------------------ |
| P                | double | 0.95 [0;1]    | Range of the distribution covered by the [min;max) range     |
| drawAtInitialize | bool   | *computed* [] | *ancestors&#58;&#58;\*/RandomiserBase&#58;&#58;\*[drawAtInitialize]* |
| drawAtReset      | bool   | *computed* [] | *ancestors&#58;&#58;\*/RandomiserBase&#58;&#58;\*[drawAtReset]* |
| drawAtUpdate     | bool   | *computed* [] | *ancestors&#58;&#58;\*/RandomiserBase&#58;&#58;\*[drawAtUpdate]* |


### Background

You cannot have a `RandomBase` box in your boxscript. It only serves as a virtual base class for boxes to draw random numbers from various distributions:

* [RandomBinomial](#randombinomial)
* [RandomLogNormal](#randomlognormal)
* [RandomLogUniform](#randomloguniform)
* [RandomNormal](#randomnormal)
* [RandomUniform](#randomuniform)
* [RandomUniformInt](#randomuniformint)

They all share the interface of `RandomBase`. Of the four inputs, you should only ever use `P` in your boxscripts. If you want to change when random numbers are drawn, you should do so in the [RandomiserBase](#randomiserbase) box. For this reason the three inputs (`drawAtInitialize`, `drawAtReset` and `drawAtUpdate`) will not be listed among the inputs for the boxes derived from `RandomBase` below.

### Usage

You can have as many of these different kinds of **random** boxes  as you want, but you should always keep as siblings inside one common parent box, together with one **randomiser** box, as described for [RandomiserBase](#randomiserbase).

## RandomBinomial

### Interface

| Inputs           | Type   | Default       | Description                                                  |
| ---------------- | ------ | ------------- | ------------------------------------------------------------ |
| P                | double | 0.5 [0;1]     | Probability of event                                         |
| **Outputs**      |        |               |                                                              |
| value            | bool   | []            | The most recently drawn value                                |

### Background

`RandomBinomial` produces numbers from the binomial distribution. `value` will be `TRUE` with probability `P`.

### Usage



```
// random_binomial1.box
Simulation sim {
  .steps = 100
  Box random {
    RandomiserMonteCarlo randomiser {
      .drawAtUpdate = TRUE
    }
    RandomBinomial x {
      .P = 0.6
    }
    RandomBinomial y {
      .P = 0.9
    }
  }
  Box positive {
    Accumulator x {
      .change = if (random/x[value]) then 1 else 0
    }
    Accumulator y {
      .change = if (random/y[value]) then 1 else 0
    }
  }
  OutputR {
    OutputText {
      .ports = random/*[value] | positive/*[value]
    }
  }
}
```

xx

```
run demo/random_binomial/random_binomial1.box
```

xx

```
> tail
...
random.x random.y positive.x positive.y iteration step
       1        1       53.0       88.0         1   95
       1        0       54.0       88.0         1   96
       0        0       54.0       88.0         1   97
       1        1       55.0       89.0         1   98
       0        1       55.0       90.0         1   99
       1        1       56.0       91.0         1  100
```



## RandomLogNormal

## RandomLogUniform

## RandomNormal

## RandomUniform

## RandomUniformInt

## 

## Records

### Interface

| Inputs | Type | Default | Purpose |
| ------ | ---- | ------- | ------- |
|fileName        |string  |"records.txt" []|Name of file with records; columns separated by white space|
|dateColumnName  |string  |       "Date" []|Name of column with date|
|timeColumnName  |string  |       "Time" []|Name of column with time|
|cycle           |bool    |        FALSE []|Cycle back to start at end of file? Forces ignoreYear=true|
|ignoreYear      |bool    |        FALSE []|Ignore year when synchronising with the calendar|
|calendarDateTime|datetime|*computed*|= calendar[dateTime]|
| **Outputs** | | | |
|firstDateTime   |datetime|              []|Date-time stamp of the first line in the file|
|lastDateTime    |datetime|              []|Date-time stamp of the last line in the file|
|currentDateTime |datetime|              []|Date-time stamp of the current outputs|
|nextDateTime    |datetime|              []|Date-time stamp of the next outputs|
|currentDate     |date    |              []|Date stamp of the current outputs|
|nextDate        |date    |              []|Date stamp of the next outputs|
|currentTime     |time    |              []|Time stamp of the current outputs|
|nextTime        |time    |              []|Time stamp of the next outputs|
|*additional*|       |              |One output port created for each column (except date and time)|
### Daily readings

You  use a `Records` box to read logged data, i.e., data with a time stamp, typically a weather file. Here is one example (`demo/weather/flakkebjerg 2005.txt`):

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

The input file must be a column-oriented text file with columns separated by whitespace (any number of blanks and tab stops between each column) and one row for each records. Columns must be complete; you cannot have an empty value. Rows, on the other hand, need not be complete. If some records are missing, say a few days due to a malfunctioning weather station, then the `Records` box will automatically interpolate (linearly) between neighboring records.

The `Records` box must be accompanied by a `Calendar` box. It is the `Calendar` box that keeps track of time during a simulation, not the `Records` box. In this example, the simulation runs through three days in December:

```
// records1.box
Simulation sim {
  .steps = 3
  Calendar calendar {
    .begin = 15/12/2005
  }
  Records weather { 
    .fileName = "../weather/flakkebjerg 2005.txt"
  }
  OutputR {
     OutputText {
       .ports = calendar[date] | weather[Tmin] | weather[Tmax]
     }
  }
}
```

We get this output from the simulation, as we would expect:

```
> load demo/records/records1.box
...
> run
...
> head
...
      date Tmin Tmax iteration step
2005/12/15  2.7  8.1         1    0
2005/12/16  1.3  4.5         1    1
2005/12/17 -4.2  0.7         1    2
2005/12/18 -2.8  1.6         1    3
```

Each of the columns in the input file produces an output port with the same name as the column heading. 

### Interpolation

The temperatures are daily estimates, which for interpolation purposes are assumed to have occured at midnight. We can explore this by changing the time step to 8 hours:

```
// records2.box
Simulation sim {
  .steps = 6
  Calendar calendar {
    .begin = 15/12/2005
    .timeStep = 8
    .timeUnit = "h"
  }
  Records weather { 
    .fileName = "../weather/flakkebjerg 2005.txt"
  }
  OutputR {
     OutputText {
       .ports = calendar[dateTime] | weather[Tmin] | weather[Tmax]
     }
  }
}
```

The output hits the same values as before every midnight (`head` shows only the first 6 rows of output, write `head 10` to see ten lines or fewer):

```
           dateTime      Tmin    Tmax iteration step
2005/12/15T00:00:00       2.7     8.1         1    0
2005/12/15T08:00:00   2.23333     6.9         1    1
2005/12/15T16:00:00   1.76667     5.7         1    2
2005/12/16T00:00:00       1.3     4.5         1    3
2005/12/16T08:00:00 -0.533333 3.23333         1    4
2005/12/16T16:00:00  -2.36667 1.96667         1    5
2005/12/17T00:00:00      -4.2     0.7         1    6
```

**Pitfalls** Plants and animals have a diurnal rythm. Be careful that your model makes sense, if you decrease the time step to less than 1 day. If use a daily accumulated variable like precipitation, you need to transform the data. If your time step is 1 hour, for example, then you must divide the precipitation data in the input file by 24 before using it in a `Records` box.

### Hourly readings

If you model runs on a finer time scale, you may have records at an hourly interval, or even down to seconds, corresponding to the smallest time step supported by `Calendar` (1 second). You may also have records at irregular intervals with a time stamp of date and time. In any case, you provide the time of the day in a separate column, as exemplified by the file `weather/copenhagen-may-hourly.txt`, which contains hourly data for the first half of May:

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

In our boxscript, we specify the period of the simulation in the Calendar box:

```
// records3.box
Simulation sim {
  Calendar calendar {
    .begin = 01/05/2001T00:00
    .end   = 06/05/2001T24:00
    .timeStep = 15
    .timeUnit = "m"
  }
  Records weather { 
    .fileName = "../weather/copenhagen-may-hourly.txt"
  }
  OutputR {
    PageR {
      .xAxis = calendar[dateTime]
      PlotR {
       .ports = weather[Tair] | weather[GlobRad] |weather[Windspeed]
      }
    }
  }
}
```

We let the simulation run with a 15-minute time step, which is a finer time resolution than given in the weather file. The Records box will oblige by interpolating the missing data, as you can see here:

```
           dateTime Tair GlobRad Windspeed iteration step
2001/05/01T00:00:00  4.7     0.0       2.6         1    0
2001/05/01T00:15:00 4.55     0.0       2.6         1    1
2001/05/01T00:30:00  4.4     0.0       2.6         1    2
                ...
2001/05/06T23:30:00 11.7     0.0      6.25         1  574
2001/05/06T23:45:00 11.9     0.0     6.475         1  575
2001/05/07T00:00:00 12.1     0.0       6.7         1  576
```

The plot looks like this:



![](../media/demo/records3.png)

**The two flavours of midnight** The time stamps 2001/05/06T24:00:00 and 2001/05/07T00:00:00 are equivalent because 24 hours is translated into 0 hours (midnight) on the following day.

### Ignore the year

It happens quite often that you want to use a weather file without regards to the year noted in the time stamp. This is the purpose of `ignoreYear`, which defaults to `FALSE` but here set it to `TRUE`:

```
// records4.box
Simulation sim {
  .steps = 3
  Calendar calendar {
    .begin = 15/12/2022
  }
  Records weather { 
    .fileName = "../weather/flakkebjerg 2005.txt"
    .ignoreYear = TRUE
  }
  ...
}
```

The Calendar box sets the simulation to begin on 15 December 2022. In response, the Records box will begin producing output from the first row that has a time stamp of 15 December. In this case, it wll be ignoring the time stamp's year (2005). As a result we get this output:

We do get the same output as previously, except the year now follows the `Calendar` box:

```
      date Tmin Tmax iteration step
2022/12/15  2.7  8.1         1    0
2022/12/16  1.3  4.5         1    1
2022/12/17 -4.2  0.7         1    2
2022/12/18 -2.8  1.6         1    3
```

### Extrapolation

What happens if we run past the end of the weather file? Consider this:

```
// records5.box
Simulation sim {
  .steps = 5
  Calendar calendar {
    .begin = 28/12/2005
  }
  Records weather { 
    .fileName = "../weather/flakkebjerg 2005.txt"
  }
  ...
}
```

The `Records` box responds by extrapolation the final reading:

```
      date Tmin Tmax iteration step
2005/12/28 -3.2 -1.4         1    0
2005/12/29 -3.6 -0.9         1    1
2005/12/30 -2.4  1.3         1    2
2005/12/31 -3.0 -0.8         1    3
2006/01/01 -3.0 -0.8         1    4
2006/01/02 -3.0 -0.8         1    5
```

### Recycling the calendar year

If your weather file goes from 1 January to 31 December, or if it otherwise forms a whole year from beginning to end, e.g. from 21 March one year to 20 March the following year, then you can ask the Records box to recycle the readings. In that way, you can use one year's weather data to simulate several years. All right, that's a quick and dirty way of doing it, but sometimes we just want it like that. 

You achieve this behaviour by setting the `cycle` flag `TRUE` (it defaults to `FALSE`). If `cycle` is `TRUE` then `ignoreYear` will always be forced to `TRUE` as well, since the year registered in the input file has become irrelevant. Let's try it:

```
// records6.box
Simulation sim {
  .steps = 5
  Calendar calendar {
    .begin = 30/12/2022
  }
  Records weather { 
    .fileName = "../weather/flakkebjerg 2005.txt"
    .cycle = TRUE
  }
  ...
}
```

We have set the calendar to reach past the end of the year. The output shows that we have successfully achieved to wrap around the weather file on 31 December and cycle back to its beginning on 1 January:

```
      date Tmin Tmax iteration step
2022/12/30 -2.4  1.3         1    0
2022/12/31 -3.0 -0.8         1    1
2023/01/01  0.0  4.7         1    2
2023/01/02  0.7  6.2         1    3
2023/01/03  1.4  7.0         1    4
2023/01/04  2.6  7.7         1    5
```

## SelectFile

## Simulation

## Stage

## StageAndPhase

## ThresholdSignal

