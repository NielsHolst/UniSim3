# Introduction

## Getting started

### Software installation  

**Universal Simulator** comes as an installation package. Just download [Universal Simulator](https://www.ecolmod.org/download/) and afterwards launch the downloaded installation package. It is currently a Windows-only software. Hail me with some encouragement, and I will find time to build a Mac or Linux installation package too.

You will also need **R** software, either stay with the [classic R](https://www.r-project.org/) or put [R Studio](https://www.rstudio.com/products/rstudio/download/) on top of it.

You've already got a pre-installed **text editor** on your computer. Free and not very useful. On Windows, that means Notepad. Improve on that and download [Notepad++](https://notepad-plus-plus.org/downloads/), right away. If you are an experienced power user, you can just keep using your favourite text editor.

If you are an experienced programmer, you may want to download the Universal Simulator source code and the Qt Creator development environment. You can find the details in the [BoxScript Developer](#boxscript-developer) section.

### Hello world!

BoxScript is not a language designed to put text messages on to your screen. Anyway: No programming language without a *Hello world!* example. Here goes

```
// hello_world1.box
Simulation sim {
  .steps = 3
  OutputText {
    &message = "Hello world!"
    .ports = .[message]
  }
}
```

Start up Universal Simulator and type at its prompt:

```
run demo/hello_world1.box
```

After it has successfully completed (we expect nothing but success of this exercise), follow up with this command:

```
head
```

The only surprise you will get is that you will see the happy message four, not three, times. Take it easy. That will be explained in due time:

```
     message iteration step
Hello world!         1    0
Hello world!         1    1
Hello world!         1    2
Hello world!         1    3
```

The output consists of three columns, two more than the message we asked for. Consider the output of this slightly extended boxscript:

```
// hello_world2.box
Simulation sim {
  .iterations = 2
  .steps = 3
  OutputText {
    &message = "Hello world!"
    .ports = .[message]
  }
}
```

The output has now been doubled in length. You'll see two iterations producing four lines each:

```
     message iteration step
Hello world!         1    0
Hello world!         1    1
Hello world!         1    2
Hello world!         1    3
Hello world!         2    0
Hello world!         2    1
Hello world!         2    2
Hello world!         2    3
```

If you just write `head` then you will only see the first 6 lines of the output (mimicking the R `head` function). To see up to, say, 10 lines, you write

```
head 10
```

The `iteration` column counts the iterations starting from 1, while the `step` column counts the steps within each iteration starting from 0. These two loops, steps within iterations, are central to the [computational model](#computational-model) of Universal Simulator.

### Hello R!

While we achieved a quick response from our efforts at the prompt above, it is not very useful for simulation outputs that spans hundreds (or millions, there are no hard-coded limits) of lines. To process larger outputs we make use of R:

```
// hello_world3.box
Simulation sim {
  .iterations = 5
  .steps = 100
  OutputR {
    OutputText {
      &message = "Hello R!"
      .ports = .[message]
    }
  }
}
```

Among the status messages reported by Universal Simulator you will see

```
...
Executable R snippet copied to clipboard!
...
```

So, switch to R and paste the clipboard at the R prompt. There you will get this message reported by R:

```
Simulation output available in data frame 'sim' with 505 rows and 3 columns
```

If you inspect the sim data frame you will find the expected outputs. Look into [Running simulations](#running-simulations) for a demonstration of how simulation outputs can be shown in R plots, or continue your reading here to learn more about the data structures behind the boxscripts.

### Classes

A boxscript arranges objects in the well-known **composite pattern** ([GoF  1995](https://www.pearson.com/en-us/subject-catalog/p/Gamma-Design-Patterns-Elements-of-Reusable-Object-Oriented-Software/P200000009480/9780201633610)).  For this to work, all objects need to be of a class derived from a common base class. In the [BoxScript framework](#boxscript-framework) the common base class is `Node`:

<img src="../media/getting-started-class-diagram.svg" style="zoom:50%;" />

`Box` and `Port` are classes derived from `Node`. You can use `Box` directly in a boxscript as a simple container for other nodes but, more importantly, it serves as a base class for classes with various behaviours, such as `Simulation` and `OutputText`. You can use this command to see all the standard classes derived from `Box`:

```
help boxes
```

`Port` is a class that allows information to flow in and out of boxes.

### Objects

Object-oriented design invites the use of natural language for very specific meanings. Thus we will use 'node' to mean a `Node` object, 'box' to mean a `Box` object and 'port' to mean a `Port` object. Plural forms mean many such objects, e.g., 'boxes' mean some number of `Box` objects.

In the `hello_world2.box` script above, we created one `Simulation` box called `sim` and one `OutputText` box, which we left unnamed. We consider the latter the **child** of the former, its **parent**. They are both parts of this object diagram:

<img src="../media/getting-started-object-diagram.svg" style="zoom:50%;" />

The diagram shows five children of the `sim` box and two children of the `OutputText` box. There are three types of ports:

* Input
* Output
* Auxiliary

These different types of ports were not implemented as classes derived from `Port`. There is instead a `type` field in the `Port` class defining the type of the specific port. While the basic `Box` class comes with no ports defined, derived classes will usually be equipped with **input ports** to supply whatever parameters and variables are needed for their functionality. Note that BoxScript does not discern between fixed inputs (in other languages called parameters, constants or settings) and variable inputs (variables). They are all just considered inputs. 

**Output ports** (marked by an ''>'' above), on the other hand, provide access to the state of the box. You won't find all the output ports of all boxes in the boxscript represented as a column in the simulation output. Rather, you tell the `OutputText` box (or any other box providing output, e.g. for R plots) which ports you want included (through its `ports` input port; well, maybe I should have chosen another name for that one). However, the `iteration` and `step` outputs of the `Simulator` box are obligatory in the output, which is how we ended up with three columns in the output above.

You can use the **help** command together with a class name to see the input and output ports belonging to a class:

```
help Simulation
```

`help` will list the ports together with their [type](#port-types), their default value (inputs only), their unit (in brackets, may be unitless) and description. If the port defaults to an [expression](#expressions), the expression will be shown.

If you want to change the default value of an input port, you precede its name by a period and write its value, as seen in the `hello_world2.box` script:

```
Simulation sim {
  .steps = 3
}
```

The right-hand side may be a constant as above or a more complicated [expression](#expressions). Most importantly, you can refer to the value of another port providing a [path](#port-paths) to it:

```
OutputText {
  &message = "Hello world!"
  .ports = .[message]
}
```

The code above also exemplifies how to create an **auxiliary port**. You can equip any box with auxiliary ports of your own choice by preceding the port name with an ampersand `&`.

Shortly on paths: The port name is always written in brackets. If several ports happen to be on the path, you will get a vector of values. The value(s) found on the path will be converted to the type of the receiving input port as needed. Incompatible port types will result in a runtime error. The full description of [paths](#port-paths) and [port types](#port-types) is not overly complicated.

### Models

The complete boxscript describes a  **model** but often a single class or an object in itself defines a model. The [DayDegrees](#daydegrees) class, for example, defines a simple model of physiological development. So, does a concrete object of that class. Thus we may refer to a boxscript, a class or an object as a model. On the other hand, it would be confusing to call the [Calendar](#calendar) class a model, since it only role is to keep track of time.

### Demo boxscripts

First time you run Universal Simulator (or whenever you have installed a new version and the first time you run that), it will give birth to a new folder `UniversalSimulatorHome` inside your home folder. The home folder is easily found on a Mac or in Linux but is well hidden on Windows (where it seemingly doesn't feel right at home). On Windows you will find it inside `C:\users\your_name`.

You are told as much in the Welcome that greets you whenever you start Universal Simulator, here on a Windows computer:

```text
Work folder:
  absolute path 'C:/Users/your_name/UniversalSimulatorHome'
Input folder:
  relative path 'input' resolves to 'C:/Users/your_name/UniversalSimulatorHome/input'
Output folder:
  absolute path 'C:/Users/your_name/Documents/QDev/UniSim2/output'
```

You can also retrieve this information with the  [get folders](#get-folders) command:

```text
get folders
```

For a start, `UniversalSimulatorHome` is set as your *work folder*. Inside the work folder, you've always got two sub-folders, the *input folder* and the *output folder*, aptly named `input` and `output` by default. 

The `UniversalSimulatorHome/input` folder will be filled with all the example boxscripts found on this site and referred to in any publications that used Universal Simulator. You will find them in the `input/demo` and `input/papers` folders, respectively.

### User boxscripts

It is recommended that you create your own *work folder* for your models. Create a folder, maybe as a sub-folder of your `Documents` folder. You can call it, for instance, `BoxModels`. Inside that create a folder called `input`. You've now got these two empty folders:

* `Documents/BoxModels`
* `Documents/BoxModels/input`

Next, you need to tell Universal Simulator that `BoxModels` is your work folder. First, find the absolute path to your work folder. On Windows, use File Explorer: Right-click the `BoxModels` folder, choose Properties on the pop-up menu, select and copy the Location path. Paste this path as an argument to the [set folder work](#set-folder-work) command:

```
> set folder work C:/Users/your_name/Documents/BoxModels
Work folder:
  absolute path 'C:/Users/your_name/Documents/BoxModels'
Input folder:
  relative path 'input' resolves to 'C:/Users/your_name/Documents/BoxModels/input'
Output folder:
  relative path 'output' resolves to 'C:/Users/your_name/Documents/BoxModels/output'
```

Now, you are ready to keep your own boxscripts in the `BoxModels` folder. Create sub-folders inside BoxModels to organize your boxscripts as you wish.

If you need to revert to the default work folder (maybe to find the demo boxscripts more easily), use [HOME](#set-folder-work-home) as a magic folder name:

```
> set folder work HOME
Work folder:
  absolute path 'C:/Users/your_name/UniversalSimulatorHome'
Input folder:
  relative path 'input' resolves to 'C:/Users/your_name/UniversalSimulatorHome/input'
Output folder:
  absolute path 'C:/Users/your_name/Documents/QDev/UniSim2/output'
```

## Running simulations

### Loading a boxscript

You load a boxscript like this

```text
> load demo/butterfly1.box
Constructing...
Amending...
17 boxes created
```

In response to reading the boxscript, Universal Simulator will construct and amend all the boxes needed to run the boxscript. In this case 17 boxes were created. The difference between constructing and amending only becomes important, once you get to the advanced stage of developing your own boxes in C++ (as `Box`-derived classes).

If you'd rather not type out the full path to the boxscript, you simple type `<Ctrl+space bar>`:

```text
load <Ctrl+space bar>
```

This will bring up a file picker, which will allow you to point and click all the way to the wanted boxscript. The file picker will start out in the same folder as the latest boxscript that you loaded.

When a boxscript has been loaded successfully (any lack of success will be obvious from error messages), you have in effect loaded a *model*, ready to be run. You can only have one model loaded at a time. If you load another model (i.e. if you load another boxscript), it will replace the current model.

### Running a model

When you run a model, it means you carry out a *simulation* as defined by the model. You can run the current model (i.e. the one defined by your latest loaded boxscript) like this

```text
> run
Constructing...
Amending...
17 boxes created
Initializing...
Resetting...
Updating...
Cleaning up...
Debriefing...
R script written to 'C:/Users/your_name/UniversalSimulatorHome/output/butterfly1_0010.R'
Executable R snippet copied to clipboard
Data frame written to 'C:/Users/your_name/UniversalSimulatorHome/output/butterfly1_0010.txt'
Finished after 266 msecs in step 152/152
```

From the output you can see that a `run` actually always carries out an initial `load` (the 17 boxes were created anew). This means that if you are in a hurry you can run a specific model directly:

```text
> run demo/butterfly1.box
...	
```

The simulation goes through specific stages, which reflect the logic of how box behaviour is defined in C++. Finally, you get a report of the three specific outputs produced by the simulation:

* an R script written to a file
* an R snippet copied to the clipboard
* a data frame written to a text file

Two files are written to the output folder. Every time you run a simulation, an additional two files will be written there. You should empty the output folder now and then to get rid of all these files.

The R snippet (a 'code snippet' is a few lines of code) in the clipboard is meant to be pasted directly at the *R prompt*. As you deftly switch (by `<Alt+Tab>` or `<Command+Tab>`) from the Universal Simulator prompt to the R prompt, where you paste (by`<Ctrl+V>` or `<Command+V>`), you are rewarded immediately by plots showing the simulation results:

![](../media/demo/butterfly1.png)

As you might have guessed, the R snippet makes clever use of the two output files to create the plots defined in the boxscript: The text file is read and ends up in an R data frame called `sim`. This data frame contains that data needed to produce the plots.

After celebrating your success, you can switch back to the Universal Simulator prompt to run an other simulation.

### Seeing and editing a boxscript

To have a look at the current boxscript simply type

```text
edit
```

This will show the boxscript code in the editor set up to edit boxscripts. First time off, no editor has been set up, so your operating system (Windows/OS X/Linux) will prompt you to choose an editor for this purpose. Any pure text editor will work (e.g., on Windows Notepad, or even better, [Notepad++](https://notepad-plus-plus.org/) but do not choose a word processor. Here, is the BoxScript code that you will see for the `butterfly1` boxscript:

```
// butterfly1.box
Simulation sim {
  Calendar calendar {
    .begin = 01/05/2009
    .end   = 30/09/2009
  }  
  Records weather {
    .fileName = "weather/flakkebjerg 2009.txt"
  }
  Box butterfly {
    Stage egg {
      .initial = 100 
      .duration = 140
      .timeStep = ./time[step]
      DayDegrees time {
        .T0 = 5
        .T = weather[Tavg]
      }
    }
    Stage larva {
      .inflow = ../egg[outflow]
      .duration = 200
      .timeStep = ./time[step]
      DayDegrees time {
        .T0 = 8
        .T = weather[Tavg]
      }
    }
    Stage pupa {
      .inflow = ../larva[outflow]
      .duration = 100
      .timeStep = ./time[step]
      DayDegrees time {
        .T0 = 10
        .T = weather[Tavg]
      }
    }
    Stage adult {
      .inflow = ../pupa[outflow]
      .duration = 28
      .timeStep = 1
    }
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      .ncol = 2
      PlotR {
        .ports = *[content]
        .ggplot = "scale_x_datetime(
                     breaks = date_breaks('months'), 
                     labels = date_format('%b')
                   )" 
      }
      PlotR {
        .ports = *[outflowTotal]
        .ggplot = "scale_x_datetime(
                     breaks = date_breaks('months'), 
                     labels = date_format('%b')
                   )" 
      }
    }
  }
}
```

Fell free to change the code, save the file, and  run the script again to see what change it made to the simulation output. Or, on second thought, save your changed boxscript under another name and then run *that* boxscript.

### Listing a boxscript

The list command is a powerful tool to inspect the *structure* and *state* of your currently loaded model. The structure will remain unaltered after you have loaded the boxscript but the state will have changed after you have run a simulation. 

First you can simply list all the boxes which together constitute the model:

```text
> list
Simulation sim
  Calendar calendar
  Records weather
  Box butterfly
    Stage egg
      DayDegrees time
    Stage larva
      DayDegrees time
    Stage pupa
      DayDegrees time
    Stage adult
  OutputR 
    PageR 
      PlotR 
      PlotR 
  OutputWriter outputWriter
    OutputSelector selector
```

The hierarchy of boxes is shown by identation. Each box is designated by its *class* (e.g. `Records`) and its *name* (e.g. `weather`), unless it is nameless (e.g. the `PlotR` boxes). Class names must correspond to a name defined in the Universal Simulator toolbox, while box names can be chosen freely albeit according to custom, they should start with a lowercase letter (telling those in the know that they are actually *object names*).

You can list a particular box and all its descendents by giving its name followed by the `r` option, which stands for *recurse*:

```text
> list butterfly r
Box butterfly
  Stage egg
    DayDegrees time
  Stage larva
    DayDegrees time
  Stage pupa
    DayDegrees time
  Stage adult
```

To inspect the ports, you add the `p` option:

```
> list calendar p
Calendar calendar
  .latitude = 52.0 [-90,90]
  .longitude = 11.0 [-180,180]
  .timeZone = 1 h
  .begin = 2009/05/01T00:00:00 DateTime
  .end = 2009/09/30T00:00:00 DateTime
  .timeStep = 1 int>0
  .timeUnit = "d" y|d|h|m|s
  >steps = 0
  >date =  d/m/y
  >time =  h:m:s
  >dateTime = T d/m/y h:m:s
  >timeStepSecs = 0.0 s
  >timeStepDays = 0.0 d
  >totalTimeSteps = 0 int
  >totalTime = 0 int
  >totalDays = 0.0 d
  >dayOfYear = 0 [1;366]

```

Here, the input ports are preceded by a period `.` and the output ports by a `>`. For each port you see its current value, often follow by its type or an indication of its legal values. The command above was issued just after loading the `butterfly1` boxscript. That's why the output ports carry their initial values, and the date, time and dateTime ports even appear empty (because their values are yet undefined). If we run the model and repeat the command, we get

```text
> list calendar p
Calendar calendar
  .latitude = 52.0 [-90,90] const
  .longitude = 11.0 [-180,180] const
  .timeZone = 1 h const
  .begin = 2009/05/01T00:00:00 DateTime const
  .end = 2009/09/30T00:00:00 DateTime const
  .timeStep = 1 int>0 const
  .timeUnit = "d" y|d|h|m|s const
  >steps = 152
  >date = 2009/09/30 d/m/y
  >time = 00:00:00 h:m:s
  >dateTime = 2009/09/30T00:00:00 d/m/y h:m:s
  >timeStepSecs = 86400.0 s
  >timeStepDays = 1.0 d
  >totalTimeSteps = 152 int
  >totalTime = 152 int
  >totalDays = 152.0 d
  >dayOfYear = 273 [1;366]
```

The output ports have now been updated and maintain their final values in the simulation. You will also note that all the inputs have been marked `const`. That's because they have been identified as being constant throughtout the simulation. This speeds up the execution. We can find examples of inputs that are not constant in one of the `DayDegrees` boxes:

```text
> list butterfly/egg/time p
DayDegrees time
  .T = 10.5 <- sim/weather[Tavg]
  .timeStepDays = 1.0 <- sim/calendar[timeStepDays]
  .resetTotal = FALSE const
  .isTicking = TRUE const
  .T0 = 5.0 const
  .Topt = 100.0 const
  .Tmax = 100.0 const
  >step = 5.5 PTU
  >total = 1605.7 PTU

```

First of all, you can see how to specify the *path* two a specific box `butterfly/egg/time`. Here you can read  [more about paths](#port-paths).

The two non-constant input ports are `T` and `timeStepDays`. You can see from which output ports they fetch their inputs and you can see their  current (final, if we just ran the model) values. This was set up in the boxscript like this

```
Stage egg {
  .initial = 100 
  .duration = 140
  .timeStep = ./time[step]
  DayDegrees time {
    .T0 = 5
    .T = weather[Tavg]
  }
}
```

Note: If `timeStep` is followed all the way back to its source, it turns out to remain constant during the simulation since it ultimately relies only on inputs that are all constant. Currently, Universal Simulator marks as constant only those ports that are immediately constant. More intelligent resolution of constants can be expected in future versions (to speed up the execution even more).

The options can be combined. Here are a few combinations (output not shown):

```
> list butterfly/egg pr
...
> list sim pr
...
```

Maybe after that, it will be nice to know that `clear` command clears the prompt dialog. It's so often called for that the `<Ctrl+L>` shortcut achieves the same. Bonus info: <Ctrl+L> also works on the R prompt. And, at the Linux prompt (but if you use Linux, you already knew that).

In the reference manual, you can find are even [more options](#list) for the list command.

### R integration

When you paste the clipboard at the R prompt, a snippet of pre-cooked R code is executed corresponding to your boxscript. However, you can also define you own R script (or R scripts) to be carried out. Here is an example of a boxscript, which does not carry out a proper simulation but rather demonstrates the curve shapes produced by to kinds of boxes, `DayDegrees` and `Briere`:

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

The output is stated as an `OutputText`, which simple produced an output text file. You specify the R script(s)—supposedly to work on that output but, in fact the R script(s) could do anything)—in `scripts` port of the `OutputR` box. This port takes a vector of strings, so you could also write:

```
.scripts = c("briere1.R", "briere2.R", "briere3.R")
```

The R scripts can be placed in the same folder as the boxscript, or you can put them in a folder, which you can refer to by paths relative to the folder of the boxscript, for example:

```
.scripts = c("general/setup.R", "analysis/briere.R", "figures/briere3.R")
```

The R scripts will be carried out (in the given order) when you paste the clipboard at the R prompt after having run your boxscript.

Before your R scripts, another R script has been executed providing you with useful *hooks* to the output produced by your script. This script is the `begin.R`, which is one of the standard scripts  residing in the `inputs/scripts` folder of the Universal Simulator home folder. It sets up the following *variables*:

* Colours named `red`, `blue`, `green`, `violet`, `orange`, `brown`, `pink`, `grey`. These colours can be used individually but they have also been set up to replace the default colour scales of *ggplot*.
* A data frame `sim` with all outputs from the boxscript.
* The name of the columns in the data frame containing the iteration counter (`iterationColumn`) and step counter (`stepColumn`).
* The absolute path to the folder that holds the boxscript (`box_script_folder`).
* The the full path (absolute path and file name) to the file holding the boxscript output (`output_file_name`), i.e. the text file read into the `sim` data frame. 

In addition, you will have access to these *functions*:

* `ggplot_theme(font_size)` returns a *ggplot* theme adjusted to the given `font_size`.
* `output_file_folder()` returns the absolute path to the output folder, i.e. the path part of `output_file_name`.
* `output_file_base_name()` returns the base file name of the output folder, i.e. the base file name part of `output_file_name`. The base file name is the file name without the suffix, in this case with out the `.txt` suffix.
* *open_plot_window(width=7, height=7)* opens a pop-up window to show you plot. It uses different methods to create the windows depending on the platform (Mac, Windows or Linux).

Here is the full *briere1.R* script referred to in the boxscript above. It demonstrates how to compare boxscript outputs with observations:

```R
# briere1.R
sim = sim[1:3]
colnames(sim) = c("Temperature", "DayDegrees", "Briere")
sim$DayDegrees = sim$DayDegrees/472.1

obs = data.frame(
  Temperature = c(15,17,20,25,27,30,33,35),
  L = c(Inf, 
        121.5, 
        (83.68+78.46)/2,
        (47.97+41.32)/2,
        (38.92+34.42)/2,
        (35.05+32.41)/2,
        (38.33+37)/2,
        Inf)
)
obs$DevRate = 1/obs$L

model = lm(DevRate~Temperature, data=obs[1:5,])
icept = coef(model)[1]
slope = coef(model)[2]
T0 = -icept/slope
duration = 1/slope

open_plot_window(6,3.5)
M = melt(sim, id.vars="Temperature", value.name="DevRate", variable.name="Model")
P = ggplot(M, aes(x=Temperature, y=DevRate, colour=Model)) +
  geom_abline(intercept=icept, slope=slope, colour=brown, size=3, alpha=0.6) +
  geom_line(size=1) +
  geom_point(data=obs, colour=red, size=3) +
  geom_vline(xintercept=T0) +
  geom_hline(yintercept=0) +
  scale_color_manual(values=c(green,blue)) +
  scale_x_continuous(breaks=c(T0,20,25,30,35), labels=c(round(T0,1), 20,25,30,35)) +
  labs(x="Temperature (oC)", y="1/L (per day)")
print(P)
```

Here is the resulting plot:

![](../media/demo/briere1.png)

## Commands

### clear

To clear the prompt dialog simply type:

```
> clear
```

The `<Ctrl+L>` shortcut will also evoke the `clear` command.

### clip

The [run](#run) automatically copies its output to the clipboard. If you have used the clipboard for other purposes after a `run` then you don't need to run the boxscript again you can simply use the `clip` command to re-create the clipboard with the simulation output. This is particularly useful if a `run` takes a long time.

### debug

The `debug` command works just like `run`. You cannot give it a boxscript as an argument though; it always works on the latest loaded or run boxscript:

```
> load demo/butterfly.box
...
> debug
...
```

`debug` produces a lot of output at the Universal Simulator prompt, easily hundreds or thousands of lines. It will write a line reporting whenever it is carrying out a [computational step](#computational-model) on a box. debug is useful if you program crashes and you want to pinpoint exactly where it happens.

### edit

There are two ways to open a boxscript for editing. The first one is to open the boxscript from your editor's File|Open menu. If you are uncertain where the boxscript resides, you can find the input folder with the  [get folders](#get-folders) command:

```
get folders
```

Alternatively (and much easier), you can first [load](#load) the boxscript and the invoke the `edit` command:

```
> load demo/butterfly.box
...
> edit
```

The very first time you use the `edit` command on a computer, it will not know which program to use to open a file with the `.box` type. Therefore, you will be queried which program you want to use to open the boxscript. Choose a text editor, not a word-processing program. On Windows, choose [Notepad++](https://notepad-plus-plus.org/) or Notepad.

Feel free to keep the boxscript open in the editor, while you are loading and running the boxscript in Universal Simulator. Just remember to save any changes to the boxscript in the editor before you re-load the boxscript in Universal Simulator.

### find

Use `find` to find all the nodes (i.e., boxes and ports ) in the model that match a certain [path](#port-paths). The `find` command takes a path as its single argument, for example, to find all the boxes of the `Stage` class in the model:

```
> load demo/butterfly.box
> find Stage::*
Stage sim/butterfly/egg
Stage sim/butterfly/larva
Stage sim/butterfly/pupa
Stage sim/butterfly/adult
```

For each match, you will get one line with the *class name* and the *full path* to the node. Here, we found four boxes. You would possibly like to proceed with `help Stage` to find the documentation for the Stage class, or maybe `list Stage::* p` to see the current value of all the ports for each of the four boxes. Read more about [help](#help) and [list](#list).

If you use `find` to look up ports, you will be told the type of each port:

```
> find weather[input::*]
Port string   sim/weather[fileName]
Port string   sim/weather[dateColumnName]
Port string   sim/weather[timeColumnName]
Port bool     sim/weather[cycle]
Port datetime sim/weather[calendarDateTime]
```

Here we have found five nodes of the `Port` class. Three are of type `string` (i.e. a piece of text), one is of type `bool` (i.e., with value `TRUE` or `FALSE`) and one a `datetime` (i.e. a calendar date in combination with a time of the day).

If you are wondering about the notation for the paths above (`Stage::*` and `weather[input::*]`) then read up on [paths](#port-paths). If you try out `find .`, you will find the root box (often called `sim`), because the root it in focus from start. You can move the focus of the [find](#find) and [list](#list) commands using the [go](#go) command.

### get folders

If you wonder where the boxscripts are located or where your output is going, use the `get folders` command. You will then see something like this

```
> get folders
Work folder:
  absolute path '.../UniversalSimulatorHome'
Input folder:
  relative path 'input' resolves to '.../UniversalSimulatorHome/input'
Output folder:
  relative path 'output' resolves to '.../UniversalSimulatorHome/output'
```

The `...` part will be something else on your computer. There are three folders to have in mind:

* work
* input
* output

The **work** folder is where you put files related to model development. If nothing else it serves as a folder to keep the **input** and **output** folders neatly in one place. This is the standard layout shown above. If you want to change it then consult the [set folder](#set-folder) command.

### go

The `go` command is used together with the [find](#find) and [list](#list) commands. After loading a boxscript the root node is the *focus node*. The focus node is used as the reference node, when you use `find` or `list` with a relative [path](#port-paths) (i.e. one beginning with one or two periods). You use `go` to change the focus node:

```
> load demo/butterfly.box
Constructing...
Amending...
15 boxes created
> find .
Simulation sim
> go egg
Now at Stage sim/butterfly/egg
> find ..
Box sim/butterfly
> run
...
> go time
Now at DayDegrees sim/butterfly/time
> list . x
DayDegrees time
  >step = 5.5 oD
    >> sim/butterfly/egg[timeStep]
    >> sim/butterfly/larva[timeStep]
    >> sim/butterfly/pupa[timeStep]

```

Beyond the use of `find` and `list`, it does not matter what the current focus node is. 

### head

The `head` command shows the first *n* lines of the lates simulation output (default *n*=6). It mimics the behaviour of the R head command:

```
> run demo/butterfly.box
...
> head
Showing .../UniversalSimulatorHome/output/butterfly_nnnn.txt
Tavg egg.content larva.content pupa.content adult.content iteration step
10.4       100.0           0.0          0.0           0.0         1    0
10.5       100.0           0.0          0.0           0.0         1    1
11.0       100.0           0.0          0.0           0.0         1    2
 9.4       100.0           0.0          0.0           0.0         1    3
10.1       100.0           0.0          0.0           0.0         1    4
10.0       100.0           0.0          0.0           0.0         1    5
```

Write `head n` to show up to *n* lines:

```
> head 3
Showing .../UniversalSimulatorHome/output/butterfly_nnnn.txt
Tavg egg.content larva.content pupa.content adult.content iteration step
10.4       100.0           0.0          0.0           0.0         1    0
10.5       100.0           0.0          0.0           0.0         1    1
11.0       100.0           0.0          0.0           0.0         1    2
```

See also [ht](#ht) and [tail](#tail).

### help

The `help` command gives you quick access to information on available commands (applicable a the Universal Simulator prompt) and `Box` classes (available to compose boxscripts).

The information shown by help depends on the options you give it:

* `help` or `help ?` provides an overview of the help options
* `help c`  shows all commands
* `help p`  shows all plugins
* `help <class name>`  shows specific class documentation
* `help <plugin name>` shows specific plugin documentation

#### Show all commands

The output from `help c` will list all the commands explained here plus some experimental ones.

#### Show all plugins

If a boxscript is not working, maybe it's because a [plugin](#plugins) is missing? You can check out the installed plugins with `help p`:

```
> help p
boxes   -Basic building blocks
command -Commands for the prompt
student -Student playing ground
```

#### Show class description

#### Show classes in a plugin

### ht

`ht` works the same way as if you write [head](#head) followed by [tail](#tail). It shows the first and last *n* lines of the output with *n=6* as the default value. Here we try with *n*=3:

```
> ht 3
Showing .../output/butterfly_0020.txt
Tavg egg.content larva.content pupa.content adult.content iteration step
10.4       100.0           0.0          0.0           0.0         1    0
10.5       100.0           0.0          0.0           0.0         1    1
11.0       100.0           0.0          0.0           0.0         1    2
 ...
14.9         0.0           0.0          0.0           0.0         1  150
11.2         0.0           0.0          0.0           0.0         1  151
10.5         0.0           0.0          0.0           0.0         1  152
```

### latest

Sometimes you want to review one of your recent simulations. `latest` will show you the name of the latest 10 boxscripts that you loaded or run.

### libr

You use the `libr` command to check that you've got the necessary R packages installed. Thus `libr` should be one of the very first commands that you invoke, once you've got Universal Simulator started. Use it with the `l` option to generate code that will load the necessary libraries in R:

```
> libr l
# R packages needed by Universal Simulator
library("colorspace")
library("ggplot2")
library("ggpubr")
library("gridExtra")
library("lubridate")
library("plyr")
library("reshape2")
library("scales")
library("splines")
library("stringr")

R code copied to clipboard! You may paste it into R.
```

You can paste the clipboard at the R prompt and check for any error messages that you are missing one or more packages. 

If you miss some packages, use the `libr` command again, this time with the `i` option:

```
> libr i
# R packages needed by Universal Simulator
install.packages("colorspace")
install.packages("ggplot2")
install.packages("ggpubr")
install.packages("gridExtra")
install.packages("lubridate")
install.packages("plyr")
install.packages("reshape2")
install.packages("scales")
install.packages("splines")
install.packages("stringr")

Copy and paste into R for any library you are missing.
```

This time nothing is put into the clipboard for you. You need to copy and paste into R those lines for which you are missing the library.

This whole procedure is only necessary first time you have installed Universal Simulator or if you have just (re-)installed R.

### list

You use `list` to inspect the structure of the currently loaded boxscript. You can also use it to find out the current values of ports. To try out the examples below, first do `load demo/butterfly.box`. 

In general you can use `list` in three ways:

* on its own: `list`
* with a path to one or more boxes: `list butterfly`
* with a path to one or more boxes followed by one or more one-letter options: `list butterfly pr`

Here, `butterfly` was used as a simple example of a path leading to just one box. 

The options explained further below are

* `b` (to show **b**oxes)
* `r` (to show **r**ecursively)
* `p` (to show **p**orts)
* `i` (to show **i**nput ports)
* `o` (to show **o**utput ports)
* `m` (to show i**m**ported ports)
* `x` (to show e**x**ported ports)
* `s` (to show ports in **s**hort format)

To get an overview of the options at the prompt, type `list ?`.

If you `list` after `load`, you will see the **default values** of ports. If you have `run` the script, you will see the **final values** of the ports ([see examples](#all-ports)).

The default [path](#port-paths) is `.` (i.e. the box in *focus*). You can move the focus of the [find](#find) and [list](#list) commands using the [go](#go) command. The default options are `br`. Hence, `list` on its own yields `list . br`.

#### All boxes

Use list on its own to see all the boxes. The hierarchy of boxes inside boxes (or of parent-child relationships to use another metaphore) are shown by indentation:

```
> list
Simulation sim
  Calendar calendar
  Records weather
  Box butterfly
    DayDegrees time
    Stage egg
    Stage larva
    Stage pupa
    Stage adult
  OutputR 
    PageR 
      PlotR 
      PlotR 
  OutputWriter outputWriter
    OutputSelector selector
```

For each box is shown its class name followed by its object name, if it has such.

#### Box and descendants

To focus on what box and its contents (i.e., its children, grandchildren, and so forth) follow list by the box name:

```
> list butterfly
Box butterfly
  DayDegrees time
  Stage egg
  Stage larva
  Stage pupa
  Stage adult
```

#### Recurse boxes

The default options are `br`, so you will always recurse through boxes if you don't include any options. Here, we have included the default `br` explicitly to assist the our understanding:

```
> list sim br
Simulation sim
  Calendar calendar
  Records weather
  Box butterfly
    DayDegrees time
    Stage egg
    Stage larva
    Stage pupa
    Stage adult
  OutputR 
    PageR 
      PlotR 
      PlotR 
  OutputWriter outputWriter
    OutputSelector selector
```

First, `list` found one box on the path (`sim`). It then proceded to recurse through and list all its descendant boxes.

Here, list found 5 boxes on path (allthe children of `sim`). It then proceded to list each of those children recursively:

```
> list sim/* br
Calendar calendar
Records weather
Box butterfly
  DayDegrees time
  Stage egg
  Stage larva
  Stage pupa
  Stage adult
OutputR 
  PageR 
    PlotR 
    PlotR 
OutputWriter outputWriter
```

If we leave out the `r` option, only the 5 children are listed:

```
> list sim/* b
Calendar calendar
Records weather
Box butterfly
OutputR 
OutputWriter outputWriter
```

#### All ports

To see all ports inside a box and their current value use the `p` option after the path:

```
> list butterfly/time p
DayDegrees time
  .T = 0.0 oC
  .timeStepDays = 0.0 d
  .resetTotal = FALSE
  .isTicking = TRUE
  .T0 = 5.0 oC
  .Topt = 100.0 oC
  .Tmax = 100.0 oC
  >step = 0.0 oD
  >total = 0.0 oD
```

Inputs are preceded with a period, outputs with a sharp parenthesis. Values are followed by their unit (nothing if they are unitless, or if the programmer of class  (you?) was lazy and did not specify it). Sometimes, units are not self-explanatory (`oD` in this example). If so, try out the [help](#help) avalailable on the class, here `help DayDegrees`. Oh, of course, it stands for day-degrees.

If values above are the default values; you will see those if you `load` and then `list`. If you `run` the boxscript and then `list`, you will get the final values after the simulation has finished together with additional information:

```
> list butterfly/time p
DayDegrees time
  .T = 10.5 oC <- sim/weather[Tavg]
  .timeStepDays = 1.0 d <- sim/calendar[timeStepDays]
  .resetTotal = FALSE const
  .isTicking = TRUE const
  .T0 = 5.0 oC const
  .Topt = 100.0 oC const
  .Tmax = 100.0 oC const
  >step = 5.5 oD
  >total = 1605.7 oD
```

Besides the final values, you will see `const` for those inputs that had been set to constant values. For variable inputs, you will see their expressions after the arrow `<-`.

#### Input ports

To see all the **input** ports inside a box and their current value use the `i` option after the path:

```
> list butterfly/time i
DayDegrees time
  .T = 0.0 oC
  .timeStepDays = 0.0 d
  .resetTotal = FALSE
  .isTicking = TRUE
  .T0 = 5.0 oC
  .Topt = 100.0 oC
  .Tmax = 100.0 oC
```

#### Output ports

To see all the **output** ports inside a box and their current value use the `o` option after the path:

```
> list butterfly/time o
DayDegrees time
  >step = 0.0 oD
  >total = 0.0 oD
```

#### Imported ports

To see all the **imported** ports inside a box and their current value use the `m` option after the path:

```
> list butterfly/time m
DayDegrees time
  .T = 10.5 oC <- weather[Tavg]
    << sim/weather[Tavg]
  .timeStepDays = 1.0 d <- calendar[timeStepDays]
    << sim/calendar[timeStepDays]
```

An imported port is one referenced in an expression. For each port in the box, the imported ports are listed with a `<<` prefix. Use this option to sort out a box's dependencies.  You need to `run` the boxscript before listing imports, because all expressions in the boxscript are only resolved when it is run.

#### Exported ports

To see all the **exported** ports inside a box and their current value use the `x option after the path:

```
> list butterfly/time x
DayDegrees time
  >step = 5.5 oD
    >> sim/butterfly/egg[timeStep]
    >> sim/butterfly/larva[timeStep]
    >> sim/butterfly/pupa[timeStep]
```

An exported port is one referenced in another port's expression. For each port in the box, the exported ports are listed with a `>>` prefix. Use this option to find out, which other boxes make use of this box's outputs. You need to `run` the boxscript before listing exports, because all expressions in the boxscript are only resolved when it is run.

#### Shortened ports

To shorten the format of the listed ports use the `s` option together with one or more of the options `piomx`:

```
> list butterfly/time os
DayDegrees time
  step = 0.0
  total = 0.0
```

Ports will be listed only with their name and value. 

### load

You use the `load` command to read a boxscript without running it. Other commands, such as [run](#run) (without boxscript argument), [list](#list) and [find](#find) will work on the latest loaded  boxscript. You can only have one boxscript loaded at a time; when you load one, the previous will be forgotten.

`load` on is own will re-load the latest loaded or [run](#run) boxscript. This is useful if  you just changed the boxscript in a text editor. `load <boxscript>` will load the given boxscript. You can type `load` followed by `Ctrl+spacebar` to bring up a file picker. This is the usual procedure.

 `load` will report how many boxes (i.e. `Box` [objects](#objects)) were created:

```
> load demo/butterfly.box
Constructing...
Amending...
15 boxes created
```

### location

Use this command to find out what the local decimal point character is

```
> location
Country: United Kingdom
Decimal character in output: "."
```

You can include a [OutputSelector](#outputselector) box in your boxscript to use the local decimal point character, rather then the default period `.`, in the simulation outputt.

### profile

If you wonder which boxes spends the most execution time, you invoke the `profile` command after you have [run](#run) a model:

```
> run demo/butterfly.box
...
> profile
Profile information written to '.../output/butterfly_0022.txt'
```

Open the `.txt` file referred to in a spreadsheet program. It is a column-oriented text file with columns separated by tab characters. It is sorted to show the most time-consuming tasks from the top. Here are the top lines of the profile output:
|Box|Task|msecs|Pct|
|---|----|-----|---|
|sim/OutputR::\*|debrief|36.8495|22|
|sim/weather|reset|20.1803|12|
|sim/weather|initialize|14.936|9|
|sim/outputWriter/selector|evaluatePorts|10.84|6|
|sim/outputWriter|debrief|10.2502|6|
|sim/weather|update|7.5492|4|
|sim|evaluatePorts|6.935|4|
|sim/OutputR::\*|evaluatePorts|6.7591|4|
|sim|initialize|6.3079|4|
|sim/outputWriter|evaluatePorts|6.1309|4|
|sim/OutputR::\*|initialize|5.0027|3|
|sim/OutputR::\*/PageR::\*|evaluatePorts|4.7758|3|
|sim/OutputR::\*/PageR::\*|initialize|3.1924|2|
|sim/butterfly/time|evaluatePorts|3.0911|2|
|sim|debrief|2.8052|2|
|sim/outputWriter|initialize|2.4478|1|
|sim/weather|evaluatePorts|2.4023|1|
|sim/outputWriter|update|2.2211|1|
|sim/outputWriter|reset|1.9182|1|
|sim/butterfly/pupa|update|1.7367|1|
|sim/OutputR::\*/PageR::\*/PlotR::\*|evaluatePorts|1.5676|1|
|sim/butterfly/egg|update|1.4996|1|
|sim/calendar|initialize|1.3263|1|
|sim/OutputR::\*/PageR::\*/PlotR::\*|evaluatePorts|1.2768|1|
|sim/butterfly/larva|update|1.0669|1|
|sim/butterfly/egg|reset|0.9363|1|
|sim/butterfly/adult|update|0.832|0|
|sim/butterfly/larva|evaluatePorts|0.7556|0|
|sim/butterfly/pupa|evaluatePorts|0.6975|0|
|sim/calendar|update|0.5569|0|

Each box in the boxscript appears several times in the profile, because it has been logged how much time was spent on different tasks. The task is one of the steps in the [computational model](#computational-model) or the 'evaluatePorts' task, which is time spent evaluating the expressions of the inputs to the box. All tasks of all boxes will have one line in the table telling the total time spent on that task (in milliseconds) and how much that made out of the total run time (as a percentage).

Here, the boxes taking care of simulation outputs, together with the `weather` box, take the most time. If you want to investigate further you can find the C++ source code by looking up the class definition (e.g., the `OutputR` class is defined in the file `src/plugins/boxes/output_r.cpp`) and finding the method listed under 'Task' in the profile table (e.g., the `OutputR::debrief()` method). You will need to [download](https://www.ecolmod.org/download/) the source code first.

### quit

`quit` shuts down Universal Simulator.

### run

`run` on is own will [load](#load) and run the latest loaded or run boxscript. `run <boxscript>` will load and run the given boxscript. You can type `run` followed by `Ctrl+spacebar` to bring up a file picker.

`run` first reports the number of boxes creates (well, actually `load` does that behind the scenes). It continues to report at the simulation runs throught the steps of the [computational model](#computational-model). 

```
> run demo/butterfly.box
Constructing...
Amending...
15 boxes created
Initializing...
Resetting...
Updating...
Cleaning up...
Debriefing...
R script written to 'C:/Users/au152367/Documents/QDev/UniSim3/output/butterfly_0013.R'
Executable R snippet copied to clipboard!
Data frame written to 'C:/Users/au152367/Documents/QDev/UniSim3/output/butterfly_0013.txt'
Finished after 163 msecs in step 152/152
```

After the simulation has finished it reports, where the generated text output and R script files are located; that will be in your [output folder](#output-folder). You are also reminded about the contents of the clipboard, which you are now expected to paste at the R prompt. The last line tells you how long it took and for how many steps (or iterations, if more than one) it was running.

### set folder

You can use the [get folders](#get-folders) command to find the current location of your **work**, **input** and **output** folders:

```
> get folders
Work folder:
  absolute path 'C:/Users/au152367/UniversalSimulatorHome'
Input folder:
  relative path 'input' resolves to 'C:/Users/au152367/UniversalSimulatorHome/input'
Output folder:
  relative path 'output' resolves to 'C:/Users/au152367/UniversalSimulatorHome/output'
```

You can change any of these folders with the `set folder` command. It comes in three flavours described below: `set folder work`, `set folder input` and `set foldet output`.

#### set folder work

This is how you change the work folder:

```
> set folder work D:/Documents/ipm-model
Work folder:
  absolute path 'D:/Documents/ipm-model'
Input folder:
  relative path 'input' resolves to 'D:/Documents/ipm-model/input'
Output folder:
  relative path 'output' resolves to 'D:/Documents/ipm-model/output'
>
```

You must provide an absolute path to the work folder. If you don't know what an 'absolute path', look it up. In this case it is `D:/Documents/ipm-model`. I recommend that you have no spaces in the path, as it can cause all kinds of trouble. General rule: Never put spaces in a folder name.

Here we kept the default input and output paths, which are both relative paths, aptly named `input` and `output`. These paths are interpreted as being relative to the work folder, hence they end up on the absolute paths `D:/Documents/ipm-model/input` and `D:/Documents/ipm-model/output` in this example.

There are two magic keywords you can use for the path you provide for the work folder. They are `HOME` and `DEV` as described next.

#### set folder work HOME

Set the work folder to `HOME` to restore the work folder to its default value:

```
> get folders
Work folder:
  absolute path 'D:/Documents/ipm-model'
Input folder:
  relative path 'input' resolves to 'D:/Documents/ipm-model/input'
Output folder:
  relative path 'output' resolves to 'D:/Documents/ipm-model/output'
> set folder work HOME
Work folder:
  absolute path '.../UniversalSimulatorHome'
Input folder:
  relative path 'input' resolves to '.../UniversalSimulatorHome/input'
Output folder:
  relative path 'output' resolves to .../UniversalSimulatorHome/output'
```

This is just a convenience since the your home folder may be akward to find on your computer.

#### set folder work DEV

Set the work folder to `DEV` to set the work folder to your software development folder. This might be useful if you have downloaded the source code for Universal Simulator and you want to keep your boxscripts and C++ code in the same folder:

```
> get folders
Work folder:
  absolute path '.../UniversalSimulatorHome'
Input folder:
  relative path 'input' resolves to '.../UniversalSimulatorHome/input'
Output folder:
  relative path 'output' resolves to .../UniversalSimulatorHome/output'
> set folder work DEV
Work folder:
  absolute path '.../Documents/dev/UniSim3'
Input folder:
  relative path 'input' resolves to '.../Documents/dev/UniSim3/input'
Output folder:
  relative path 'output' resolves to '.../Documents/dev/UniSim3/output'
```

### set folder input

I never needed to change the input folder but it would go like this using `set folder input`:

```
> get folders
Work folder:
  absolute path '.../Documents/dev/UniSim3'
Input folder:
  relative path 'input' resolves to '.../Documents/dev/UniSim3/input'
Output folder:
  relative path 'output' resolves to '.../Documents/dev/UniSim3/output'
> set folder input test-input
relative path 'test-input' resolves to '.../Documents/QDev/UniSim3/test-input'
```

### set folder output

I never needed to change the output folder either (see [set folder input](#set-folder-input)) but it would go like this using `set folder output`:

```
> get folders
Work folder:
  absolute path '.../UniversalSimulatorHome'
Input folder:
  relative path 'input' resolves to '.../Documents/dev/UniSim3/input'
Output folder:
  relative path 'output' resolves to .../Documents/dev/UniSim3/output'
> set folder output bucket
relative path 'bucket' resolves to '.../Documents/QDev/UniSim3/bucket'
```

### tail

The `tail` command shows the last *n* lines of the lates simulation output (default *n*=6). It mimics the behaviour of the R `tail` command:

```
> run demo/butterfly.box
...
> tail
Showing .../UniversalSimulatorHome/output/butterfly_nnnn.txt
Tavg egg.content larva.content pupa.content adult.content iteration step
14.5         0.0           0.0          0.0           0.0         1  147
15.0         0.0           0.0          0.0           0.0         1  148
15.2         0.0           0.0          0.0           0.0         1  149
14.9         0.0           0.0          0.0           0.0         1  150
11.2         0.0           0.0          0.0           0.0         1  151
10.5         0.0           0.0          0.0           0.0         1  152
```

Write `tail n` to show up to *n* lines:

```
> tail 3
Showing .../UniversalSimulatorHome/output/butterfly_nnnn.txt
Tavg egg.content larva.content pupa.content adult.content iteration step
14.9         0.0           0.0          0.0           0.0         1  150
11.2         0.0           0.0          0.0           0.0         1  151
10.5         0.0           0.0          0.0           0.0         1  152
```

See also [head](#head) and [ht](#ht).

### what

Just in case you have forgotten what you are doing, the `what` command will tell you the name of the latest loaded or run boxscript.

### Shortcuts

#### Esc

The escape key moves the cursor to the end of the dialog and offers you a fresh dialog.

#### Ctrl+C

Your usual shortcut for copying the currently selected text to clipboard.

#### Ctrl+L

Shortcut for the [`clear`](#clear) command.

#### Ctrl+V

Your usual shortcut for pasting the clipboard contents at the position of the cursor.

#### Ctrl+Q

Shortcut for the [`quit`](#quit) command.

#### Ctrl+R

Shortcut for the [`run`](#run) command.

#### Mouse right-click or shift-click

The usual copy-paste-cut menu pops up when you right-click (Windows) or shift-click (Mac) or whatever (Linux).

# The BoxScript language

## Boxscript layout

A boxscript is a text file written in the BoxScript language . BoxScript was designed as a declarative language. You use it mainly to declare how your model is constructed (which building blocks it is made of and how they are related). You do not use it to define the behaviour of the  building blocks themselves. That behaviour is coded in C++ as described in the [BoxScript Developer](#boxscript-developer) section. 

A boxscript consists of boxes. A box comes equipped with input and outport ports, which forms the interface by which it can exchange data with other boxes (through their ports). In addition, you can equip a box with auxiliary ports *in* the boxscript, which work just like the inherent input ports. The only restriction is that you can never change the value of an output port from within a boxscript.

A boxscript has the following, hierarchic structure:

* A **box** declaration contains zero or more **port** declarations followed by zero or more box declarations.
* A box declaration always begins with a box **class name** optionally followed by an **object name**.
* A port declaration begins with a **period** (for an input port) or an **ampersand** (for an auxilliary port) followed by a port name, an equal sign and an **expression**. 
* A **comment** runs from `//` to the end of the line.

The [Hello world!](#hello-world) boxscript exemplified most of BoxScript syntax:

```
// hello_world1.box
Simulation sim {
  .steps = 3
  OutputText {
    &message = "Hello world!"
    .ports = .[message]
  }
}
```

You will notice that the contents of a box is embraced by curlies `{ }` and that indeed an object name can be left out (the `OutputText` box is anonymous). Ports are assigned to an expression by an equal sign, and an expression can take many forms. 

White space (space and lines) is used only to assist your eye; it has no effect otherwise (well, except, that a `//` comment runs to the end of the line). When you refer to an input port, such as `.steps` that input port must be among the input ports defined for the class (here the `Simulation` class). You can look up the input (and output) ports belonging to a class either through help, for example,

```
help Simulation
```

or here in the documentation (e.g., [Simulation](#simulation)).

Ports have a `type`, which is either defined in the C++ class (for input and output ports), or is deduced from the expression they are assigned to in the boxscript (for auxilliary ports). In the boxscript above, for example, the `steps` port is defined to be an integer in the `Simulation` class, and the `message` port is deduced to be a string.

## Port types

A port is always typed. It holds a value of a certain type. For input and output ports, that type is determined by the C++ class. For auxiliary ports, their type is deduced from the expression they are assigned to. Most types exist both in a scalar form and a vector form. A scalar holds one value, a vector holds zero or more values of the same type. You cannot build composite types, e.g., vectors of vectors of integers. In fact, you will most of the time deal with scalar types.

The table below shows the names of the types available in BoxScript. Those are the type names used in the online BoxScript documentation (yes, that's right here) and in the response you get from the [help](#help) command. The corresponding C++ types are only interesting, if you code your own C++ `Box` classes; you will notice that the Qt libraries are used for vectors, dates and times, while `BareDate` and `Path` are classes defined in the [BoxScript Framework](#boxscript-framework). The third column shows [expressions](#expressions) that exemplify the different types. The `c()` notation refers to the BoxScript [c](#c) function used to construct vectors.

| BoxScript type name        | C++ type                             | Expression of that type                                      |
| -------------------------- | ------------------------------------ | ------------------------------------------------------------ |
| bool<br />vec_bool         | bool<br />QVector&lt;bool>           | TRUE or FALSE<br />c(TRUE, FALSE, FALSE)                     |
| int<br />vec_int           | int<br />QVector&lt;int>             | 117<br />c(-3, 10, 7)                                        |
| double<br />vec_double     | double<br />QVector&lt;double>       | 3.1412<br />c(5, 2.3, 6)                                     |
| string<br />vec_string     | QString<br />QVector&lt;QString>     | "weather.txt"<br />c("Gila", "monster", 12)                  |
| date<br />vec_dat          | QDate<br />QVector&lt;QDate>         | 24/12/2021 or 2021/12/24 or /12/24/2021<br />c(13/07/2021, 2021/8/24, /3/20/2023) |
| time<br />vec_time         | QTime<br />QVector&lt;QTime>         | 23:17 or 23:17:05 or 9:5 or 9:50:7<br />c(23:17, 13:17:05, 9:5) |
| datetime<br />vec_datetime | QDateTime<br />QVector&lt;QDareTime> | *date*  "T"  *time*: 24/12/2021T13:14<br />c(24/12/2021T13:14, 03/02/2022, 01/02/2021T14:50) |
| baredate<br />vec_baredate | BareDate<br />QVector&lt;BareDate>   | 24/12 or /12/24<br />c(24/12, 3/11, /11/7)                   |
| path                       | Path                                 | sim[step] \| sim[iteration] \| calendar[output::*]           |
| null                       | *not applicable*                     | sim[steps] (*unresolved*)Nore                                |

The simple types are `bool` for boolean (true/false) ports, `int` for integer ports (whole numbers, negative included), `double` (real numbers) and `string` (text).

The various `date` and `time` types are true types. You should not put them in apostrophes. A value in apostrophes denote a `string` type.  A  `date` can be written in three different orders: day/month/year, year/month/day or /month/day/year. The latter (American-style) format is indicated by a leading slash. A year is always written in full (with four digits), while day<10 and month<10 can be written with or without a leading zero as you please. The `baredate` type is a date without a year. You can write it as day/month or /month/day and with leading zeros or not as for a full `date`.

Ports of the `path` type must de declared so in the C++ class. You cannot create an auxiliary port of the `path` type, since the type of the port will always be deduced from the type of the expression. In this example, the `n` port will be of `int` type because that's the type of `sim[step]`.

```
Simulation sim {
  Box {
	&n = sim[step]  
  }
}
```

There is a whole [vocabulary for paths](#port-paths) which include the union operator `|` to combine separate paths into one. Because of that there is no need for a vector of paths. Such a type does not exist.

If a port of `date`, `time`, `datetime` or `baredate` type was not given an initial value in the class definition (i.e. in the C++ code), it will start out being `null`. Otherwise, the `null` type occurs if it has not yet been possible to determine the type of an auxiliary port. You can run this script for an example:

```
run demo/port_types/null.box
```

The script contains a sequence of indirections so long that it takes some steps of simulation, before all references have become disentangled. Here is the boxscript contrived for this demonstration:

```
// null.box
Simulation sim {
  .steps = 5
  Box A {
    &a = B[a] + 1
  }
  Box B {
    &a = C[a] + 1
  }
  Box C {
    &a = D[a] + 1 
  }
  Box D {
    &a = E[a] + 1
  }
  Box E {
    &a = F[a] + 1
  }
  Box F {
    &a = 10
  }
  OutputText {
    .ports = *[a]
  }
}
```

Inspect the output by the `head` command and you will see that the first couple lines contain null values, which are always shown as the text `null`:

```
> head
...
 A.a  B.a C.a D.a E.a F.a iteration step
null null  13  12  11  10         1    0
null   14  13  12  11  10         1    1
  15   14  13  12  11  10         1    2
  15   14  13  12  11  10         1    3
  15   14  13  12  11  10         1    4
  15   14  13  12  11  10         1    5
>
```

If you've got `null` values in the first few lines of output then include an [OutputSelector](#outputselector) box with an appropriate value for its `beginStep` port.

## Expressions

You write an expression on the right-hand side of the equal sign whereever you assign a value to a port. Here are some examples:

```
.aboveThreshold    = (Weather[temperature] > hibernation[threshold])
&tempInside        = Weather[temperature] + 2.5
.biomass           = 2300*mouse[density] + 6.7*rabbit[density]
&homeArea          = 3.1412*(homeRange[distance]/2)^2
.supplyDemandRatio = min(N[supply]/N[demand], P[supply]/P[demand])
&demand            = if (.[aboveThreshold] || weather[sunshine] > 500) then 7.6 else 0
```

To remind you that expressions can be assigned to both input and auxiliary ports, I have written every other line as either an input (with leading `.`) or an auxiliary (with a leading `&`) port. The expression in the first line will result in a value of `bool` type while the others are all of `double` type.

Expressions are formulated with the common arithmetic operators: `+`, `-`, `*`, and `/`. In addition, you can use `^` for exponentiation, `&&` for logical *and*, `||` for logical *or* and `!` for logical negation. Parentheses `()` work as expected. Expressions may also inclode [functions](#functions) and [conditionals](#conditionals).

### Scalar and vector operations

An expression may mix scalar and vector types. If more then one vector is present, they must be of the same size. If scalars are mixed with vectors, the result will be a vector. The principles reflect those of R. Here are examples showing the addition of scalar and vector integers:
$$
3+9=12
$$

$$
3+
\begin{bmatrix}
    7 \\
    9 \\
    13
\end{bmatrix} =
\begin{bmatrix}
    10 \\
    12 \\
    16
\end{bmatrix}
$$

$$
\begin{bmatrix}
    7 \\
    9 \\
    13
\end{bmatrix} +
\begin{bmatrix}
    -2 \\
    -11 \\
    8
\end{bmatrix} =
\begin{bmatrix}
    8 \\
    -2 \\
    21
\end{bmatrix}
$$

The other operators work likewise. 

### Mixed-type operations and conversions

==23 Sep 2022. This is how far I got. This section is imprecise and under revision.==

There are two cases where types can be mixed:

* inside an expression
* when the expression type differs from the port being assigned to

In both cases, types will be converted to the most generic of the involved types or, if such a conversion would make no sense, you will get an error message. For instance, `3` (an `int`) can be converted to `3.0` (a `double`) or `"3"` (a `string`) but you cannot convert it to a `date`.

This table lists the legal conversions:

| Type     | Conversion alternatives                                      |
| -------- | ------------------------------------------------------------ |
| bool     | bool&rarr;int (0 if false, 1 if true)                        |
| int      | int&rarr;bool (false if 0 otherwise true)<br />int&rarr;double |
| double   | double&rarr;bool (false if 0.0 *exactly* otherwise true)<br />double&rarr;int (rounded to nearest integer) <br />double&rarr;string |
| string   | "FALSE"&rarr;bool (false)<br />"TRUE"&rarr;bool (true)<br />string&rarr;int (if string contains an integer)<br />&rarr; |
| date     | Integer [1;366] *day of year*<br />Real [1;366] *day of year*<br />String |
| time     | Integer [0..23] *hour of the day*<br />Real [0;24) *fractional hour of the day*<br />String |
| datetime | Integer [1:366] *day of year*<br />Real [0; 366) *fractional day of year*<br />String |
| baredate | Integer [1;366] *day of year*<br />Real [1;366] *day of year*<br />String |

Conversions from Boolean false or true lead to values of 0 or 1 ("0" or "1" when converted to a String).

Conversions from a String to Boolean lead to 0 for "false", "FALSE" and "0" and, to 1 for "true", "TRUE" and "1".



### Operators

### Functions

#### c

#### max

#### min

#### sum

### Conditionals

## Port paths

### Nodes

 In BoxScript, a path consists of a sequence of *nodes* separated by slashes with the final node (a *port*) in brackets, for example:

```
butterfly/larva[inflow]
```

Here, we've got three nodes: two boxes (`butterfly` and `larva`) and a port (`inflow`). A path like this my match zero or more ports in the boxscript. 

### Joker

The joker can be used in place of any node in the path:

```
&eggs = *[eggs]
&birdEggs = bird/*[eggs]
```

It can be used to include all ports of a box. This is most useful when defining ports for output as in:

```
OutputR {
	Box outputs {
		&lions = lion[density]
		&antelopes = antelope[density] 
	}
	PageR {
		PlotR {
			.ports = outputs[*]
		}
	}
}
```

This is an easy way to define the labels in your output. Here the labels on the plot will be `lions` and `antelopes`.

### Anchors

A path will be matched against all nodes in the model unless it is anchored. You  anchor a path by beginning it with a slash or one or two periods. The meaning is the same as you may know from file paths:

* at the root `/`
* at the box `.`
* at the parent box `..`

A slash is a synonym for the root box, no matter what's its name. Consider this boxscript:

```
Simulation sim {
	Calendar calendar { 
	}
	Box lion {
		&date = sim/calendar[date]
		&demand = ./hunger[perCapita]
		Box hunger {
			&perCapita = 0.11
		}
	}
	Box antelope {
		&date = /calendar[date]
		&demand = ./hunger[perCapita]
		Box hunger {
			&perCapita = 0.96
		}
	}
	Box foodWed {
		&demands = ../*[demand]
	}
}
```

Here the paths for `date` in `lion` and `antelope` are equivalent. The paths `./hunger[perCapita]` are rooted to their parents, `lion` and `antelope`, respectively. Therefore they will each find only one match. The path `../*[demand]` is rooted at the parent of `foodWeb`. Hence it matches both `demand` ports (and its type will be a vector of real numbers with two elements).

### Union

You can think of a path as a set of matching references. You can merge several such sets by the union operator `|`. The merged set of references is guaranteed not to contain any duplicates. The union operator is very common when you define `ports` to show in `PlotR`. Another common use is to `sum` ports (forming a vector) to be used as a single input (a scalar):

```
.ports = weather[T] | weather[RH] | plant[biomass]
.inflow = sum(reproduction[outflow] | immigration[value])
```

### Qualifications

*Box nodes* can be qualified with a class name. This again is mostly useful in combination with a joker. This will match ports named `outflow` in boxes of the `Stage` class:

```
.ports = Stage::*[outflow]
```

Box nodes can be qualified with a namespace too with or without the class:

```
.ports = savanna::*[density]
.ports = cocoa::Plant::*[biomass]
```

Here, the first path will match ports named `density` found in boxes of a class belonging to the `savanna` namespace. The second path will match ports named `biomass` found in boxes of the `Plant` class belonging to the `cocoa` namespace. The namespace is decided by the plugin, in which the class resides. The name of the plugin and the namespace is always the same.

*Port nodes* can be qualified with `input`, `output` or `aux` to match only input, output or auxiliary ports. This is most useful in combination with a joker:

```
.ports = lion[output::*]
.ports = antelopes[input::*]
.ports = lion/kitten[aux::*]
```

### Family relations

You can specify family relations for any node in the path, in terms of `descendants` and `ancestors`.

`descendants` limit the path to all children, grand-children and so forth, while `ancestors` limit the path to parent, grandparent and so forth. Hence, `descendants` fans out to all nodes below (or 'inside', if you which) the current node, while `ancestors` search upwards along the single line of heritage:

```
> load demo/butterfly.box
...
> go butterfly
Now at Box sim/butterfly
> find descendants::Stage::*
Stage sim/butterfly/egg
Stage sim/butterfly/larva
Stage sim/butterfly/pupa
Stage sim/butterfly/adult
> go larva
Now at Stage sim/butterfly/larva
> find ancestors::Box::*
Box        sim/butterfly
Simulation sim
> find ancestors::egg
No matches
> find ancestors::*/egg
Stage sim/butterfly/egg
> 
```

# Cereal aphid-fungus model

## Background

This is a tri-trophic model of the winter wheat-cereal aphid (*Sitobion avenae*)-fungus (*Pandora neoaphidis*) system, simulating the dynamics of aphid and fungus from spring until harvest. The winter wheat model is rudimentary; the crop is only represented as a growth stage modelled by a regression model fitted to Norwegian growing conditions. Weather data (daily average temperature and maximum relative humidity) drives the model. Optionally, the winter wheat model can be left out, and the crop growth stage instead supplied as an additional column in the weather file. A scientific paper (Stéphanie Saussure et al.) presenting the model is currently (2022) under review.

Below you will find models of increasing complexity, building up to the full model found in the scientific paper. The models are composed of building blocks, implemented as C++ [classes](#classes). The functionality of model building blocks were implemented in C++. If you really want to get the depth of that, you can download the [source code](https://www.ecolmod.org/download/#source-code). The classes specific to this model is found in the `src/plugins/aphid` source code folder.

Each model building block was designed to solve a particular task. The model was constructed from building blocks included in the standard plugin ([boxes](#the-boxes-plugin)), together with building blocks developed specifically for this model (in the [aphid](#the-aphid-plugin) plugin). 

You can run the boxscripts shown in the sub-sections below yourself. For that purpose you must [install Universal Simulator](#software-installation). For example, to run the  `crop-aphid-model.box` script, you write at the Universal Simulator prompt:

```
run models/aphid/crop-aphid-model.box
```

Paste the clipboard at the R prompt to see the output as described in [Running simulations](#running-simulations).

## Crop model

The crop model can either be weather-driven (by a weather file) or data-driven (by recorded wheat growth stages).

### Weather-driven

The weather-driven crop model makes use of the [CropGrowthStage](#cropgrowthstage) and [CropIsGrowing](#cropisgrowing) models, tailored for Norwegian conditions, together with the generic [DayDegrees](#daydegrees) model. A [Calendar](#calendar) and [Records](#records) box provides the driving data:

```
// crop-model-weather-driven.box
Simulation sim {
  Calendar calendar {
    .begin = 01/04/2004
    .end   = 31/08/2004
  }
  Records weather {
    .fileName =  "weather/Aarnes_2004.txt"
  } 
  CropGrowthStage cropGrowthStage {
    .valueAtStart       = 20.0
    .dayDegreesHalfways = 800.0
    .slope              = 2.8
    .dayDegrees = ./time[total]
    CropIsGrowing isGrowing {
      .temperature = weather[Tavg] 
    }
    DayDegrees time{
      .T = weather[Tavg]
      .T0 = 0
      .isTicking = ../isGrowing[value]
    }
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      .width = 6
      .height = 3.5
      PlotR {
        .ports = cropGrowthStage[value]
      }
    }
  }
}
```

Run the model from the Universal Simulator prompt with

```
run models/aphid/crop-model-weather-driven.box
```

and paste to R to see the output:

<img src="../media/models/aphid/aphid-crop-model-weather-driven.png" style="zoom:80%;" />



### Optionally data-driven

The crop sub-model used in the final, complete model is flexible. It will pick the growth stage from the weather file, if a `CropGrowthStage` column is available. Otherwise, it uses the value provided by the weather-driven model. This feature was needed for the validation runs of the model, because the field observations were not from Norway. In those cases, the `CropGrowthStage` model cannot be expected to hold; it is more accurate to use the observed crop growth stages. Here is the boxscript to demonstrate a data-driven crop growth stage:

```
// crop-model-data-driven.box
Simulation sim {
  Calendar calendar {
    .begin = 01/04/1995
    .end   = 31/08/1995
  }
  Records weather {
    .fileName = "validation/A95-weather-gs.txt"
  } 
  Box wheat{
    CropGrowthStage cropGrowthStageModel {
      .valueAtStart       = 20.0
      .dayDegreesHalfways = 800.0
      .slope              = 2.8
      .dayDegrees = ./time[total]
      CropIsGrowing isGrowing {
        .temperature = weather[Tavg] 
      }
      DayDegrees time{
        .T = weather[Tavg]
        .T0 = 0
        .isTicking = ../isGrowing[value]
      }
    }
    Box cropGrowthStage {
      // Pick growth stage either from the weather file or else from the growth stage model
      &value = if exists(weather[GrowthStage]) then weather[GrowthStage] else ../cropGrowthStageModel[value]
    }
  }
  OutputR {
    PageR {
      .xAxis = calendar[date]
      .width = 6
      .height = 3.5
      PlotR {
        .ports = cropGrowthStage[value]
      }
    }
  }
}
```

Run the model from the Universal  Simulator prompt with

```
run models/aphid/crop-model-data-driven.box
```

and paste to R to see the output:

<img src="../media/models/aphid/aphid-crop-model-data-driven.png" style="zoom:80%;" />

## Crop-aphid model

In this model, the fungus is left out, so it is basically a model of aphid population dynamics driven bottom-up by the crop. The model is composed of these dedicated model building blocks, which is worth to look up. They each solve one particular task specific to this model:

* [CropGrowthStage](#cropgrowthstage)
* [CropIsGrowing](#cropisgrowing)
* [AphidImmigration](#aphidimmigration)
* [AphidNetReproduction](#aphidnetreproduction)
* [AphidOffspring](#aphidoffspring)
* [AphidJuvenileSurvival](#aphidjuvenilesurvival)

In addition, you will find these basic building blocks:

* [Calendar](#calendar)
* [Records](#records)
* [Stage](#stage)

After you have loaded the model:

```
load models/aphid/crop-aphid-model.box
```

The `list` command will show you the model structure:

```
> list
Simulation sim
  Box param
  Calendar calendar
  Records weather
  Box wheat
    CropGrowthStage cropGrowthStageModel
      CropIsGrowing isGrowing
      DayDegrees time
    Box cropGrowthStage
  Box aphid
    DayDegrees time
    Box density
    AphidImmigration immigration
    AphidNetReproduction netReproduction
    AphidOffspring offspring
    AphidJuvenileSurvival survival
    Box susceptible
      Box nymph
        Stage apterous
        Stage alate
      Box adult
        Stage apterous
          Stage fecundity
        Stage alate
          Stage fecundity
  OutputR 
    PageR 
      PlotR 
    PageR 
      PlotR 
  OutputWriter outputWriter
    OutputSelector selector
```

The `aphid` box is just a `Box`, which means it acts as a simple container of other boxes. Since we are anticipating a model including the fungus soon, we have put the `nymph` and `adult` stages inside a `susceptible` box ('susceptible' means 'uninfected' in epidemiologist speak). Both are further divided into sub-populations of `apterous` and `alate` morphs. The `adult` box holds boxes inside for the individuals (`apterous` and `alate`) and their respective reproduction (`fecundity`).

The `param` box holds parameter values that must be the same across several boxes, as seen in the boxscript below.

```
// crop-aphid-model.box
Simulation sim {
  Box param {
    &k = 15
    &juvenileApterousDuration = 172
    &juvenileAlateDuration = 195 
    &adultDuration = 300
    &fecundityDuration = 100
    &fecundity_k = 1
  }
  Calendar calendar {
	...
  }
  Records weather {
    ...
  } 
  Box wheat {
    ...
  }
  Box aphid {
    DayDegrees time{
      .T = weather[Tavg]
      .T0 = 3 
      .Topt = 18 
      .Tmax = 30 
    }  
    Box density {
      &nymphs      = sum(../susceptible/nymph/*[content])
      &adults      = sum(../susceptible/adult/*[content])
      &total       = .[nymphs] + .[adults]
    }
    AphidImmigration immigration {
      .cropGrowthStage = cropGrowthStage[value]
      .toCropGrowthStage = 69
      .immigrationRate = 0.02
      .propExposedImmigrants = 0
      .k = param[k]
    }
    AphidNetReproduction netReproduction {
      .Tmin = 3 
      .Topt = 16.1 
      .Tmax = 30
      .R0opt = 51.6
      .temperature = weather[Tavg]
      .cropGrowthStage = cropGrowthStage[value]
      .optimumCropGrowthStageMin = 59
      .optimumCropGrowthStageMax   = 73
      .optimumCropFactor = 1.6
      .aphidDensity = ../density[total]
      .aphidDensityThreshold = 40 
      .alateFactor = 0.67
    }
    AphidOffspring offspring {
      .offspringTotal = sum(../*/adult/*/fecundity[outflow]) 
      .aphidDensity = ../density[total]
      .cropGrowthStage = cropGrowthStage[value]
    }
    AphidJuvenileSurvival survival{
      .cropGrowthStage = cropGrowthStage[value]
      .temperature = weather[Tavg]
    }
    Box susceptible {
      Box nymph {
        Stage apterous {
          .inflow       = ancestors::*/offspring[apterous]
          .timeStep     = ancestors::*/time[step]
          .growthFactor = ancestors::*/survival[value]
          .k = param[k]
          .duration = param[juvenileApterousDuration]
        }
        Stage alate {
          .inflow = ancestors::*/offspring[alate]
          .timeStep = ancestors::*/time[step]
          .k = param[k]
          .duration = param[juvenileAlateDuration]
          .growthFactor = ancestors::*/survival[value]
        }
      }
      Box adult {
        Stage apterous {
          .inflow = ancestors::*/nymph/apterous[outflow]
          .timeStep = ancestors::*/time[step]
          .duration = param[adultDuration]
          .k = param[k]
          Stage fecundity {
            .inflow = ancestors::*/nymph/apterous[outflow]
            .timeStep = ancestors::*/time[step]
            .duration = param[fecundityDuration]
            .k = param[fecundity_k]
            .growthFactor = ancestors::*/netReproduction[apterous]
          }
        }
        Stage alate {
          .inflow = ancestors::*/immigration[susceptible]
          .timeStep = ancestors::*/time[step]
          .duration = param[adultDuration]
          .k = param[k]
          Stage fecundity {
            .inflow = ancestors::*/immigration[susceptible]
            .timeStep = ancestors::*/time[step]
            .duration = param[fecundityDuration]
            .k = param[fecundity_k]
            .growthFactor = ancestors::*/netReproduction[alate]
          }
        }
      }
    } // susceptible
  } // aphid
  OutputR {
    .width = 5
    .height = 10
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .ports = susceptible/*/*[value] | offspring[output::*]
      }
    }
    PageR {
      .xAxis = cropGrowthStage[value]
      PlotR {
        .ports = susceptible/*/*[value] | offspring[output::*]
        .ggplot = "scale_x_continuous(breaks=10*(0:10))"
      }
    }
  }
}
```

If you stumble over expressions, such as `.[nymph]` or `ancestors::*/time[step]`, please read up on [port paths](#port-paths).

When you run the model,

```
run models/aphid/crop-aphid-model.box
```

Aphid population dynamics are shown both on a date and a crop growth stage scale. Here are the two plots: 

<img src="../media/models/aphid/crop-aphid-model.png">

The alate offspring comes in two waves, the second wave due to decreasing aphid density caused by the ripening of the crop. This may or may not reflect reality. The interaction between aphid density and crop growth stage, which determines aphid fecundity, is complicated.

## Crop-aphid model with uncertainty

In this extension of the model above, we include uncertainty on weather and a few model parameters. Every time you run the model, you will get a different output. In this boxscript listing, only the additional boxes and major changes are shown (left-out parts shown as `...`):

```
// crop-aphid-model-ua.box
Simulation sim {
  .iterations = 30
  .silent = TRUE
  SelectFile weatherFiles {
    .folder = "weather"
    .filter = "*.txt"
    .showFileNames = FALSE
  }
  Box random {
    RandomiserStratified randomiser {
    }
    RandomUniformInt k {
      .min = 15
      .max = 30
    }
    RandomUniformInt fileNumber {
      .min = 1
      .max = weatherFiles[numFiles]
    }
    RandomNormal cropAtStart {
      .min = 10
      .max = 30 
    }
    RandomNormal cropHalfways {
      .min = 750 
      .max = 850 
    }
  }
  ...
  Records weather {
    .fileName =  ./files[fileNamePath]   
    .ignoreYear = TRUE
    SelectFile files {
      .folder = "weather"
      .filter = "*.txt"
      .selectFileNumber = random/fileNumber[value]
      .showFileNames = FALSE
    }
  } 
  Box wheat{
    CropGrowthStage cropGrowthStageModel {
      .valueAtStart       = random/cropAtStart[value]
      .dayDegreesHalfways = random/cropHalfways[value]
      .slope              = 2.8
      .dayDegrees = ./time[total]
      CropIsGrowing isGrowing {
        .temperature = weather[Tavg] 
      }
      DayDegrees time{
        .T = weather[Tavg]
        .T0 = 0
        .isTicking = ../isGrowing[value]
      }
    }
    Box cropGrowthStage {
      &value = if exists(weather[GrowthStage]) then weather[GrowthStage] else ../cropGrowthStageModel[value]
    }
  }
  ...
  OutputR {
    .scripts = "crop-aphid-model-ua.R"
    OutputText {
      .ports = calendar[date] | cropGrowthStage[value] | susceptible/*/*[value] | offspring[output::*] 
    }
  }
}
```

The `random` box draws numbers for the uncertain parameters of which there are four:

* `k`
* `fileNumber`
* `cropAtStart`
* `cropHalfways`

The very first child of `random` determines the method by which to draw random numbers. In this boxscript, stratified random sampling has been chosen.

The `k` parameter will achieve a number between `15` and `30` before each iteration of the simulation. It is used by `immigration` and the four aphid sub-populations. You can use [list](#list) with the `x` option to verify this:

```
> list random/k x
RandomUniformInt k
  >value = 21
    >> sim/aphid/immigration[k]
    >> sim/aphid/susceptible/nymph/apterous[k]
    >> sim/aphid/susceptible/nymph/alate[k]
    >> sim/aphid/susceptible/adult/apterous[k]
    >> sim/aphid/susceptible/adult/alate[k]
```

The `cropGrowthStageModel` box uses the random values for `cropAtStart` and `cropHalfways`.

`fileNumber` represents a bit of a hack to choose at random, a file with suffix `txt` within the `weather` sub-folder. Two `SelectFile` boxes do the trick. The first one (`weatherFiles`) is queried by `fileNumber` about the total number of candidate files. The second one (`files`) is used to pick a file by the random number (the files are considered numbered alphabetically), which ultimately is the one opened by `weather`.

`iterations` have been set to `30`, which means the model will be running 30 times. To avoid an avalanche of status messages on the screen, `silent` has been set to `TRUE`. You should try setting it to `FALSE` (its default value), just to notice the difference.

The `OutputR` box has been set to run a dedicated R script to process the simulation output:

```R
# crop-aphid-model-ua.R
densities = colnames(sim)[3:9]

M = melt(sim, id.vars=c("iteration", "date"), measure.vars=densities, variable.name="Variable", value.name="Value")
open_plot_window(5,10)
P = ggplot(M, aes(date, Value, colour=Variable, group=iteration)) +
  geom_line(alpha=0.3) +
  labs(y="") +
  theme(legend.position="none") +
  facet_wrap(~Variable, ncol=1, scales="free")
print(P)

M = melt(sim, id.vars=c("iteration", "cropGrowthStage"), measure.vars=densities, variable.name="Variable", value.name="Value")
open_plot_window(5,10)
P = ggplot(M, aes(cropGrowthStage, Value, colour=Variable, group=iteration)) +
  geom_line(alpha=0.3) +
  scale_x_continuous(breaks=10*(0:10)) +
  labs(y="") +
  theme(legend.position="none") +
  facet_wrap(~Variable, ncol=1, scales="free")
print(P)

```

When you run the simulation, 

```
run models/aphid/crop-aphid-model-ua.box
```

the R script will be executed in R, when you paste the clipboard there. The plots are similar to the one before but now show output uncertainty as 30 overlaid curves for each variable:

<img src="../media/models/aphid/crop-aphid-model-ua.png">

Aphid dynamics are seen to be less uncertain on a crop growth stage scale than on a date scale.

## Crop-aphid-fungus model

We are now ready to tackle the full, tri-trophic model. For that purpose we need to add the fungus. However, it is not represented as a fungus population directly (e.g. as fungal biomass or number of spores in various compartments, such as soil and hosts). Lack of data does not allow us that detail. Instead, the fungus is represented in

* exposed immigrants
* exposed aphids (nymps and adults, apterous and alate)
* aphid cadavers

Remember that exposed means 'infected'. Furthermore, note that we call nymphs, that will develop into alate adults, 'alate'; the correct word is 'alatiform' but we ignore that distinction in the code to make our boxscripts more readable.

Here is the complete boxscript. It has been broken into pieces to allow comments as we go:

```
// crop-aphid-fungus-model.box
Simulation sim {
  Box param {
    &k = 15
    &juvenileApterousDuration = 172
    &juvenileAlateDuration = 195 
    &adultDuration = 300
    &fecundityDuration = 100
    &fecundity_k = 1
    &lethalTime = 80
  }
  Calendar calendar {
    .begin = 01/04/2004
    .end   = 31/08/2004
  }
  Records weather {
    .fileName = "weather/Aarnes_2004.txt"
  } 
  Box wheat{
    CropGrowthStage cropGrowthStageModel {
      .valueAtStart       = 20.0
      .dayDegreesHalfways = 800.0
      .slope              = 2.8
      .dayDegrees = ./time[total]
      CropIsGrowing isGrowing {
        .temperature = weather[Tavg] 
      }
      DayDegrees time{
        .T = weather[Tavg]
        .T0 = 0
        .isTicking = ../isGrowing[value]
      }
    }
    Box cropGrowthStage {
      &value = if exists(weather[GrowthStage]) then weather[GrowthStage] else ../cropGrowthStageModel[value]
    }
  }
```

So far nothing much has changed, only a few additions to `param`. But here goes! We invoke a [Maker](#maker) box to construct two copies of the box declared in line  5 below. They will be named `withoutFungus` and `withFungus` as defined in line 4 and also commented in line 5:

```
  Maker system {
    // The system contains two aphid populations, 
    // identical except for the proportion of exposed immigrants
    .names = c("withoutFungus", "withFungus")
    Box { // withoutFungus or withFungus
      DayDegrees time{
        .T = weather[Tavg]
        .T0 = 3 
        .Topt = 18 
        .Tmax = 30 
      } 
```

In effect the whole aphid model will be replicated, the two copies having the same structure and sharing all parameter values. They are created as children of `system` and can referred to by  `system/withoutFungus` and `system/withFungus`. 

We add a `fungusTime` to keep track of infection development and a few extra ports in `aphidDensity` to count aphids by their infection status (`susceptible`,  `exposed` and `cadavers`). The `immigration` box asks if the name of its parent is `withFungus`, in which case 25% of the immigrants will arrive exposed; otherwise zero. Here, `name` is a BoxScript function. [Paths](#port-paths) in BoxScript must include a port, but it can be left empty to refer to a box. Hence, we arrive at the expression `name(..[])`. 

```
      DayDegrees fungusTime {
        .T = weather[Tavg]
        .T0 = 2 
        .Topt = 18 
        .Tmax = 30 
      }
      Box aphidDensity {
        &nymphs      = sum(../*/nymph/*[content])
        &adults      = sum(../*/adult/*[content])
        &total       = .[nymphs] + .[adults]
        &susceptible = sum(../susceptible/*/*[content])
        &exposed     = sum(../exposed/*/*[content])
        &cadavers    = sum(../infectious/cadavers[content])
      }
      AphidImmigration immigration {
        .cropGrowthStage = cropGrowthStage[value]
        .toCropGrowthStage = 69
        .immigrationRate = 0.02
        .propExposedImmigrants = if name(..[]) == "withFungus"
                                 then 0.25
                                 else 0.0
        .k = param[k]
      }
```

The two systems (`system/withoutFungus` and `system/withFungus`) do not interact but run in parallel (logically, not in terms of computer processes). This allows comparison between the two systems of the simulation results, since they have everything in common, except for the proportion of exposed immigrants, as defined in lines 19-21 above.

For reproduction and survival, nothing has changed but we need to be carefull with our references. Thus there are two ports on the path `aphidDensity[total]`, namely `withoutFungus/aphidDensity[total]` and `withFungus/aphidDensity[total]`. To get the right one, we must use the relative path `../aphidDensity[total]`:

```
      AphidNetReproduction netReproduction {
        .Tmin = 3 
        .Topt = 16.1 
        .Tmax = 30
        .R0opt = 51.6
        .temperature = weather[Tavg]
        .cropGrowthStage = cropGrowthStage[value]
        .optimumCropGrowthStageMin = 59
        .optimumCropGrowthStageMax   = 73
        .optimumCropFactor = 1.6
        .aphidDensity = ../aphidDensity[total]
        .aphidDensityThreshold = 40 
        .alateFactor = 0.67
      }
      AphidOffspring offspring {
        .offspringTotal = sum(../*/adult/*/fecundity[outflow]) 
        .aphidDensity = ../aphidDensity[total]
        .cropGrowthStage = cropGrowthStage[value]
      }
      AphidJuvenileSurvival survival{
        .cropGrowthStage = cropGrowthStage[value]
        .temperature = weather[Tavg]
      }
```

The `Stage` boxes must be extended to allow outflow in a new direction. A certain proportion, set by the model for `infectionRate`, is leaving susceptible stages to become exposed (further down in the boxscript). First for the `nymph` stages:

```
      Box susceptible {
        Box nymph {
          Stage apterous {
            .inflow       = ancestors::*/offspring[apterous]
            .timeStep     = ancestors::*/time[step]
            .growthFactor = ancestors::*/survival[value]
            .k = param[k]
            .duration = param[juvenileApterousDuration]
            .phaseOutflowProportion = ancestors::*/infectious/infectionRate[value]
          }
          Stage alate {
            .inflow = ancestors::*/offspring[alate]
            .timeStep = ancestors::*/time[step]
            .k = param[k]
            .duration = param[juvenileAlateDuration]
            .growthFactor = ancestors::*/survival[value]
            .phaseOutflowProportion = ancestors::*/infectious/infectionRate[value]
          }
        }
```

Then for the `adult` stages; here, we must remember to let the infected proportion of `fecundity` follow the adults:

```
        Box adult {
          Stage apterous {
            .inflow = ancestors::*/nymph/apterous[outflow]
            .timeStep = ancestors::*/time[step]
            .duration = param[adultDuration]
            .k = param[k]
            .phaseOutflowProportion = ancestors::*/infectious/infectionRate[value]
            Stage fecundity {
              .inflow = ancestors::*/nymph/apterous[outflow]
              .timeStep = ancestors::*/time[step]
              .duration = param[fecundityDuration]
              .k = param[fecundity_k]
              .growthFactor = ancestors::*/netReproduction[apterous]
              .phaseOutflowProportion = ancestors::*/infectious/infectionRate[value]
            }
          }
          Stage alate {
            .inflow = ancestors::*/immigration[susceptible]
            .timeStep = ancestors::*/time[step]
            .duration = param[adultDuration]
            .k = param[k]
            .phaseOutflowProportion = ancestors::*/infectious/infectionRate[value]
            Stage fecundity {
              .inflow = ancestors::*/immigration[susceptible]
              .timeStep = ancestors::*/time[step]
              .duration = param[fecundityDuration]
              .k = param[fecundity_k]
              .growthFactor = ancestors::*/netReproduction[alate]
              .phaseOutflowProportion = ancestors::*/infectious/infectionRate[value]
            }
          }
        }
      } // susceptible
```

Notice that the `inflow` to the `nymph` stages (`apterous` and `alate`) originates from the `offspring` box, while the `inflow` to the `adult` `apterous` stage originates from individuals finishing their `apterous` `nymph` stage. You would expect the same kind of `inflow` to the `adult` `alate` stage; however, alate adults are assumed to fly away immediately. Hence, alate adults originate only has immigrants (which are assumed to stay) received from the `immigration` box.

No nymphs hatch as exposed (the fungus is only laterally transmitted). Hence the only `inflow` to the `exposed` `nymph` stages is from the infection of susceptible nymphs (just calculated above):

```
      Box exposed {
        Box nymph {
          StageAndPhase apterous {
            .timeStep = ancestors::*/time[step]
            .k = param[k]
            .duration = param[juvenileApterousDuration]
            .growthFactor = ancestors::*/survival[value]
            .phaseInflow = ancestors::*/susceptible/nymph/apterous[phaseOutflow]
            .phaseTimeStep = ancestors::*/fungusTime[step]
            .phaseK = param[k]
            .phaseDuration = param[lethalTime]
          }
          StageAndPhase alate {
            .timeStep = ancestors::*/time[step]
            .k = param[k]
            .duration = param[juvenileAlateDuration]
            .growthFactor = ancestors::*/survival[value]
            .phaseInflow = ancestors::*/susceptible/nymph/alate[phaseOutflow]
            .phaseTimeStep = ancestors::*/fungusTime[step]
            .phaseK = param[k]
            .phaseDuration = param[lethalTime]
          }
        } // nymph
```

Exposed adults, on the other hand may enter through two pathways: as exposed nymphs developing into exposed adults, or as susceptible adults getting infected. They enter below as the inputs `inflow` and `phaseInflow`, respectively:

```
        Box adult {
          StageAndPhase apterous {
            .inflow = ancestors::*/nymph/apterous[outflow]
            .timeStep = ancestors::*/time[step]
            .duration = param[adultDuration]
            .k = param[k]
            .phaseInflow = ancestors::*/susceptible/adult/apterous[phaseOutflow]
            .phaseTimeStep = ancestors::*/fungusTime[step]
            .phaseK = param[k]
            .phaseDuration = param[lethalTime]
            StageAndPhase fecundity {
               // No inflow because exposed/nymphs don't reproduce as adults
              .timeStep = ancestors::*/time[step]
              .duration = param[fecundityDuration]
              .k = param[fecundity_k]
              .growthFactor = ancestors::*/netReproduction[apterousExposed] 
              .phaseInflow = ancestors::*/susceptible/adult/apterous/fecundity[phaseOutflow]
              .phaseTimeStep = ancestors::*/fungusTime[step]
              .phaseK = param[k]
              .phaseDuration = param[lethalTime]
            }
          }
          StageAndPhase alate {
            .inflow = ancestors::*/immigration[exposed]
            .timeStep = ancestors::*/time[step]
            .duration = param[adultDuration]
            .k = param[k]
            .phaseInflow = ancestors::*/susceptible/adult/alate[phaseOutflow]
            .phaseTimeStep = ancestors::*/fungusTime[step]
            .phaseK = param[k]
            .phaseDuration = param[lethalTime]
            StageAndPhase fecundity {
              // Exposed immigrants reproduce after arriving
              .inflow = ancestors::*/immigration[exposed]
              .timeStep = ancestors::*/time[step]
              .duration = param[fecundityDuration]
              .k = param[fecundity_k]
              .growthFactor = ancestors::*/netReproduction[alateExposed] 
              .phaseInflow = ancestors::*/susceptible/adult/alate/fecundity[phaseOutflow]
              .phaseTimeStep = ancestors::*/fungusTime[step]
              .phaseK = param[k]
              .phaseDuration = param[lethalTime]
            }
          }
        } // adult
```

The `exposed` box contains one extra child to keep track of cadavers:

```
        CadaverConversion succumbed {
          .succumbedApterousNymphs = sum(ancestors::*/nymph/apterous[phaseOutflow])
          .succumbedAlateNymphs    = sum(ancestors::*/nymph/alate[phaseOutflow])
          .succumbedApterousAdults = sum(ancestors::*/adult/apterous[phaseOutflow])
          .succumbedAlateAdults    = sum(ancestors::*/adult/alate[phaseOutflow])
        }
      } // exposed
```

Our model in the taxonomy of epidemiological models is an SEI (susceptible-exposed-infectious) model. Hence, to our collection of system boxes (`system/susceptible` and `system/exposed`) we now add `system/infectious`, which contains child boxes to hold `cadavers` and to calculate `infectionRate`:

```
      Box infectious {
        OnOff isSporulating {
          .x = weather[RHmax]
          .xOn = 95
          .xOff = 999 
        }
        CadaverTime time {
          .isSporulating = ../isSporulating[isOn]
          .timeStep = ancestors::*/fungusTime[step]
          .rhAccelaration = 2
        }
        Stage cadavers {
          .inflow = ancestors::*/exposed/succumbed[cadavers]
          .timeStep = ../time[step]
          .duration = 100
          .k = param[k]
        }
        InfectionRate infectionRate {
          .isSporulating = ../isSporulating[isOn]
          .cadavers = ../cadavers[content]
          .transmissionEfficiency = 0.2
        }
      } // infectious
```

The final child of `system` contains summary statistics, including `yield`:

```
      Box diagnostics {
        Accumulator aphidDays {
          .change = ancestors::*/aphidDensity[total]
        }
        Accumulator cadaverDays {
          .change = ../../infectious/cadavers[content]
        }
        Prevalence prevalence {
          .aphidDensity   = ancestors::*/aphidDensity[total]
          .exposedDensity = ancestors::*/aphidDensity[exposed]
          .cadaverDensity = ancestors::*/aphidDensity[cadavers]
        }
        AphidIndex aphidIndex {
          .nymphs = ancestors::*/aphidDensity[nymphs]
          .adults = ancestors::*/aphidDensity[adults]
        }
        aphid::Yield yield {
          .aphidIndex = ../aphidIndex[value]
          .cropGrowthStage = cropGrowthStage[value]
        }
      } // diagnostics
    } // withoutFungus or withFungus
  } // system
```

The two pages of output are both shown as plots in two colums, filled in column-first (that's what `direction` is for; change it to `"row"` and notice the difference):

```
  OutputR {
    .width = 7
    .height = 7
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .ports = system/*/*/*/Stage::*[value] | Stage::cadavers[value]
        .ncol = 2
        .direction = "col"
        .ggplot = "scale_colour_manual(values=c(rep(red,5), rep(blue,5)))"
      }
    }
    PageR {
      .xAxis = calendar[date]
      PlotR {
        .ports = diagnostics/aphidDays[value] | diagnostics/cadaverDays[value] | diagnostics/prevalence[output::*] |
                 diagnostics/yield[yield]
        .ncol = 2
        .direction = "col"
        .ggplot = "scale_colour_manual(values=c(rep(red,5), rep(blue,5)))"
      }
    }
  }
}
```
When we run the model, 

```
run models/aphid/crop-aphid-fungus-model.box
```

as expected, the fungus is shortening the duration of the aphid outbreak. Cadavers are only present in the system with fungus:

<img src="../media/models/aphid/crop-aphid-fungus-model-aphids.png">

Despite the obvious effect of the fungus on the density of aphids, `yield` does not seem much improved:
<img src="../media/models/aphid/crop-aphid-fungus-model-diagnostics.png">

## Biocontrol model with uncertainty analysis

The `biocontrol-model.box` script adds uncertain parameters, as we already saw in [Crop-aphid model with uncertainty](#crop-aphid-model-with-uncertainty), but with even more uncertain parameters, most of them pertaining to the fungus. These are the exact same uncertain 11 parameters that also appear in the scientific paper on the model (Saussure et al., submitted 2022). Here is the `random` box from the `biocontrol.box` script:

```
  Box random {
    RandomiserStratified randomiser {
    }
    RandomUniformInt k {
      .min = 15
      .max = 30
    }
    RandomUniformInt fileNumber {
      .min = 1
      .max = weatherFiles[numFiles]
    }
    RandomNormal cropAtStart {
      .min = 10
      .max = 30 
    }
    RandomNormal cropHalfways {
      .min = 750 
      .max = 850 
    }
    RandomNormal propExposedImmigrants {
      .min = 0.1
      .max = 0.7
    }
    RandomNormal lethalTime {
      .min = 50 
      .max = 115
    }
    RandomNormal immunityCost {
      .min = 0
      .max = 0.4
    }
    RandomNormal sporulationOn {
      .min = 80 
      .max = 99 
    }
    RandomNormal cadaverDuration {
      .min = 48 
      .max = 112 
    }
    RandomNormal timeAcceleration {
      .min = 1
      .max = 3
    }
    RandomNormal transmissionEfficiency {
      .min = 0.05
      .max = 0.5 
    }
  }
```

You can open the boxscript yourself and find how each random parameter is used in the model. The [list](#list) command with the `x` option comes in handy (some output abbreviated `...`):

```
> run models/aphid/biocontrol-model.box
...
> list random/RandomBase::* x
RandomUniformInt k
  >value = 30
    >> sim/system/withoutFungus/immigration[k]
    >> sim/system/withoutFungus/susceptible/nymph/apterous[k]
    >> sim/system/withoutFungus/susceptible/nymph/alate[k]
    ...
    >> sim/system/withFungus/immigration[k]
    >> sim/system/withFungus/susceptible/nymph/apterous[k]
    >> sim/system/withFungus/susceptible/nymph/alate[k]
    ...
RandomUniformInt fileNumber
  >value = 6
    >> sim/weather/files[selectFileNumber]
RandomNormal cropAtStart
RandomNormal cropHalfways
RandomNormal propExposedImmigrants
  >value = 0.508888
    >> sim/system/withoutFungus/immigration[propExposedImmigrants]
    >> sim/system/withFungus/immigration[propExposedImmigrants]
RandomNormal lethalTime
  >value = 88.8232
    >> sim/system/withoutFungus/exposed/nymph/apterous[phaseDuration]
    >> sim/system/withoutFungus/exposed/nymph/alate[phaseDuration]
    >> sim/system/withoutFungus/exposed/adult/apterous/fecundity[phaseDuration]
    ...
	>> sim/system/withFungus/exposed/nymph/alate[phaseDuration]
    >> sim/system/withFungus/exposed/adult/apterous/fecundity[phaseDuration]
    >> sim/system/withFungus/exposed/adult/apterous[phaseDuration]
    ...
RandomNormal immunityCost
  >value = 0.0987229
    >> sim/system/withoutFungus/netReproduction[immunityCost]
    >> sim/system/withFungus/netReproduction[immunityCost]
RandomNormal sporulationOn
  >value = 84.5715
    >> sim/system/withoutFungus/infectious/isSporulating[xOn]
    >> sim/system/withFungus/infectious/isSporulating[xOn]
RandomNormal cadaverDuration
  >value = 86.299
    >> sim/system/withoutFungus/infectious/cadavers[duration]
    >> sim/system/withFungus/infectious/cadavers[duration]
RandomNormal timeAcceleration
  >value = 1.90676
    >> sim/system/withoutFungus/infectious/time[rhAccelaration]
    >> sim/system/withFungus/infectious/time[rhAccelaration]
RandomNormal transmissionEfficiency
  >value = 0.305888
    >> sim/system/withoutFungus/infectious/infectionRate[transmissionEfficiency]
    >> sim/system/withFungus/infectious/infectionRate[transmissionEfficiency]
> 
```

A `Biocontrol` box has been added near the end of the boxscript, and the output is now sent to a dedicated R script called `biocontrol.R`:

```
  Biocontrol biocontrol {
    .aphidPressureWithoutF = withoutFungus/diagnostics/aphidDays[value] 
    .aphidPressureWithF = withFungus/diagnostics/aphidDays[value] 

    .yieldWithoutF = withoutFungus/diagnostics/yield[yield] 
    .yieldWithF = withFungus/diagnostics/yield[yield] 
    
    .cropGrowthStage = cropGrowthStage[value]
    .prevalence = withFungus/diagnostics/prevalence[exposed]
    .cadaverPrevalence = withFungus/diagnostics/prevalence[cadavers]
  }
  OutputR {
    .scripts = "biocontrol.R"
    OutputText {
      .ports = calendar[date] | biocontrol[output::*]
    }
  }
```

The `biocontrol` box compares the outputs of `system/withoutFungus` and `system/withFungus`. When you run the model,

```
run models/aphid/biocontrol-model.box
```

it will run for 30 iterations. However, here I have changed it to just one iteration

```
// biocontrol-model.box
Simulation sim {
  .iterations = 1
```

to approach the upcoming complexity in baby steps. Here is the output:
<img src="../media/models/aphid/biocontrol-1.png">

What we see changing with time, as an effect of the fungus, are

* the reduction in aphid pressure (area under the aphid density curve, so-called aphid-days)
* the improvement in yield (in %-points)
* the maximum prevalence of cadavers and the growth stage when that occurred
* the maximum prevalence (of exposed aphids) and the growth stage when that occurred

We also see the percentage of cadavers at  growth stage 43, 61 and 73.

Since the model contains uncertain parameters, the course of these nine curves (which we will call model **outcomes**) will be different for every iteration we run the model. But notice, that the course of the curves is not that important, in fact, we are just interested in the **final value** for each of the outcomes. They tell us everything (or, at least, they summerise the salient points) about the effects of biocontrol.

When *you* run the model, you will see 30 curves overlaid for each model outcome:

<img src="../media/models/aphid/biocontrol-30.png">

Imagine now that we picked just the final values for all of these curves. We could then show a histogram for each of the nine outcomes. These histograms would should how uncertain model outcomes are, all due to the uncertainty of the 11 model parameters. If you change iterations to 100, or any number higher then that, you will be rewarded by pastel-coloured histograms:
<img src="../media/models/aphid/biocontrol-100.png">

An obvious question, once you have celebrated your success and posted this beautiful figure (which illustrates an **uncertainty analysis**) on the wall, is, which of the 11 uncertain parameters are most influential on each of these nine model outcomes? That is the purpose of **sensitivity analysis**, which we turn to next.

## Biocontrol with sensitivity analysis

### Setting up the analysis

It will take us only a little editing of the `biocontrol-model.box` script to add sensitivity analysis (SA). First, you should realise that SA is very computing intensive, so we need to optimise our sampling strategy for the random numbers. We will shift from stratified random sampling (which optimises the random sampling inside the distribution of each model parameter)  to Sobol' quasi-random numbers (which optimise the random sampling inside the 11-dimensional distribution across all 11 model parameters). This is set up at the top of the boxscript, where we replace `RandomiserStratified` with `RandomiserSobolSequence`:

```
// biocontrol-model-sa.box
Simulation sim {
  .iterations = 832 // 832 (for demonstration) or 1703936 (for analysis)
  .silent = TRUE
  .unattended = TRUE
  .stopIteration = !question[accepted]
  PopUp question {
    .title = "You are about to run a simulation that will potentially last many hours (at ~40,000 simulations per hour)."
    .text = "Do you want to continue?"
    .icon = "question"
    .buttons = c("Yes", "No")
  }
  SelectFile weatherFiles {
    .folder = "weather"
    .filter = "*.txt"
    .showFileNames = FALSE
  }
  Box random {
    RandomiserSobolSequence randomiser {
      .doSensitivityAnalysis = TRUE
      .bootstrapSize = 100 // 100 (for demonstration) or 10000 (for analysis)
    }
    RandomUniformInt k {
      .min = 15
      .max = 30
    }
    ...
}
```

Since your computer is likely to lock up during this lengthy simulation, `unattended` has been set `TRUE`. Otherwise, you will get an (unharmful) error message when the simulation has finished, because the clipboard would have been locked as well.

When we are using Sobol' numbers we should be careful to make a balanced sampling of parameter space. To obtain that, the number of simulation `iterations` must equal  $2^n(p+2)$ , where  $n$ is a whole, positive number, and  $p$ is the number of parameters sampled (we've got 11). Hence, these are all valid options:
$$
\begin{align*}
2^1\cdot(11+2) &= 26 \\
2^2\cdot(11+2) &= 52 \\
2^3\cdot(11+2) &= 104 \\
\ldots \\
2^{17}\cdot(11+2) &= 1,703,936 \\
\ldots
\end{align*}
$$
The last number listed is the one used in the published paper. That's a weekend job! For a quick demonstration we will stick to 832 iterations but you will also be shown, what came out of the weekend job. If you set `iterations` to an invalid number, the simulation will stop immediately if you try to run it. As a service, you will get suggestions to valid values for `iterations` near the number you picked. So, if you want in the range of, say, 50,000 iterations, write that, and you will be told nearby valid numbers.

To carry out the sensitivity analysis on the simulation output, we must set `doSensitivityAnalysis` to `TRUE` and choose a reasonable `bootstrapSize` to do statistics on the sensitivity indices. A `bootstrapSize` of 10,000 seems to be a standard choice in literature but here we'll use 100 for demonstration.

The [Popup](#popup) box is there to warn the user and give the option of regretting ever having started the simulation. The answer is used by `sim` to stop the simulation in the first step, if `stopIteration` is `TRUE`.

### Showing the results

The [OutputR](#outputr) box contains four child boxes. Here are the first two:

```
OutputR {
  .saveDataFrame = TRUE
  OutputSelector {
    .final = TRUE
  }
  PageR {
    .xAxis = random/*[value]
    PlotR {
      .ports = biocontrol[output::*]
      .maxData = 1000
      .ggplot = "geom_smooth(colour='yellow')"
    }
  }
  ...
}
```

We ask for the output to be saved in a data frame (`saveDataFrame` is `TRUE`) to keep it for later; the simulation may have taken many hours and we don't want to lose it. We invoke the [OutputSelector](#outputselector) to write only the `final` values of each iteration. We are going to use the outputs generated by the [Biocontrol](#biocontrol) box, and for those the final values nicely summarise the effect of biocontrol.

The first [PageR](#pager) box puts all 11 uncertain parameters on the x-axis and all 9 biocontrol outputs on the y-axis. Let's decipher those two paths for `xAxis` and `ports`. But first we need to run the model (this took 20 seconds on my computer),

```
run models/aphid/biocontrol-model-sa.box
```

We can use the [find](#find) command to find out what's on the path specified for the `xAxis` above (`random/*[value]`):

```
> find random/*[value]
Port int    sim/random/k[value]
Port int    sim/random/fileNumber[value]
Port double sim/random/cropAtStart[value]
Port double sim/random/cropHalfways[value]
Port double sim/random/propExposedImmigrants[value]
Port double sim/random/lethalTime[value]
Port double sim/random/immunityCost[value]
Port double sim/random/sporulationOn[value]
Port double sim/random/cadaverDuration[value]
Port double sim/random/timeAcceleration[value]
Port double sim/random/transmissionEfficiency[value]
```

Check. Those are the 11 parameters we want on the x-axis. Now for `biocontrol[output::*]`:

```
> find biocontrol[output::*]
Port double sim/biocontrol[aphidPressureDifference]
Port double sim/biocontrol[yieldImprovement]
Port double sim/biocontrol[percentageCadaversGs43]
Port double sim/biocontrol[percentageCadaversGs61]
Port double sim/biocontrol[percentageCadaversGs73]
Port double sim/biocontrol[maxCadaverPrevalence]
Port double sim/biocontrol[maxCadaverPrevalenceGS]
Port double sim/biocontrol[maxPrevalence]
Port double sim/biocontrol[maxPrevalenceGS]
```

Check. We've got nine of those.

Since we were running the simulation `unattended` (see above), you must type in the [clip](#clip) command to fill the clipboard after the simulation has finished. When you paste that into R, R will do the bootstrapping on the sensitivity indices. This  took 15 seconds on my machine. The first thing, you will see, is this impressive 11 by 9 plot:

<img src="../media/models/aphid/biocontrol-sa-1.png">

Notice that in the [PlotR](#plotr) box, we set `maxData` to 1000 to show only the first 1000 lines of output; we don't want thousands of dots in those tiny plots. The R code given to the `ggplot` input is added to the plot in R (where it is produced by R's `ggplot` function), which is why the yellow trend lines appear on top of the dots.

The other two plots are defined like this:

```
PageR {
  .xAxis = random/*[value]
  PlotR {
    .ports = biocontrol[output::*]
    .type = "SobolConvergence"
  }
}
PageR {
  .xAxis = random/*[value]
  PlotR {
    .ports = biocontrol[output::*]
    .type = "SobolIndices"
  }
}
```

What sets them apart is their `type`. These are specialised plots meant to be used only in connection with a sensitivity analysis. For these plots, you will need plenty of iterations to give a correct picture of model sensitivities. Here, we are showing how the plots look after 1,703,936 iterations.

The first plot is a convergence plot:

<img src="../media/models/aphid/biocontrol-sa-2.png">

You use it to judge whether the two Sobol' sensitivity indices (total and first-order) have stabilised, or if you need to increase the number of iterations. The number on the x-axis is the so-called sample size ($N$) of the analysis. It is  $N=2^n$ defined by the number of iterations =  $2^n(p+2)$ (see [Setting up the analysis](#setting-up-the-analysis)). The first column shows the sums across all 11 parameters for both indices. They are still changing ever so slightly going from $N=32,768$ to $N=65,536$ for some of the nine outcome variables (arranged in rows). We judge that $N=131,072$ is a sufficient sample size for our analysis.

The second plot shows the total and first-order sensitivity indices for each of the nine outcome variables:

<img src="../media/models/aphid/biocontrol-sa-3.png">

The 11 parameters have been sorted for each of the nine outcome variable to show the most influential ones at the top. The error bars show the 95% confidence limits estimated by bootstrapping. You can tell a long story based on this sensitivity analysis. For that we refer to our paper (Saussure et al., submitted 2022).

### Saving and restoring the analysis

We already saved the data frame by setting `saveDataFrame` to `TRUE` in the `OutputR` box. Since the bootstrap estimates of the confidence limits on the sensitivity indices can be somewhat time consuming to compute, those estimates are always, automatically, saved as well. This allows you to restore the analysis quickly later on.

The two files are both saved as R data frames written to your [output folder](#output-folder). To find out where that is use the  [get folders](#get-folders) command:

```
get folders
```

Visit that folder and you will find these two, latest files:

* `biocontrol-model-sa_nnnn.Rdata`
* `biocontrol-model-sa_nnnn-S.Rdata`

where `nnnn` is some running number. The output folder is not a safe place for files (you should empty it now and then). Copy those two files to another folder for safe-keeping.

You must write your own R script to process the two files but there is an R script to get you started residing in the same folder as the `biocontrol-model-sa.box` script:

```
# biocontrol-model-sa-restore.R
rm(list=ls(all=TRUE))

# Load standard scripts
source("<path to your input folder>/scripts/begin.R")
source("<path to your input folder>/scripts/begin-sobol.R")

# Load your data
setwd("<path to your own folder>")
load("biocontrol-model-sa_nnnn.Rdata")
load("biocontrol-model-sa_nnnn-S.Rdata")
```

The first line clears R memory just to be certain that we are working on the restored data. The scripts `begin.R` and `begin-sobol.R` are standard scripts with various useful functions found in the `scripts` sub-folder inside your input folder. Find out where your input folder is by the  [get folders](#get-folders) command:

```
get folders
```

The two calls of `load` will create the data frames `sim` and `S` in R. You can inspect them and find them pretty self-explanatory, except that in the `S` data frame, we recommend using the bootstrap-estimated confidence limits (`LowerPercentile` and `HigherPercentile`) rather than the estimated standard error (`EffectSE`).

You can treat these data however you want but the final lines in the `biocontrol-model-sa-restore.R` script shows you how to re-create the three plots produced by your original `biocontrol-model-sa.box` script:

```
# Prepare analysis
sobol_k = length(unique(S$Input)) - 1
sobol_N = nrow(sim)/(sobol_k+2)
iterationColumn = "iteration"
stepColumn      = "step"

# Plot trends
plot_facetted(
  df   = sim[1:min(nrow(sim),1000),], 
  x    = input_names(), 
  y    = output_names(), 
  ncol = 1, 
  dir  = 'h') + geom_smooth(colour='yellow')

# Plot convergence
plot_sobol_convergence()

# Plot indices
ggarrange(
  plotlist=plot_sobol_indices(S)
)
```

## Saussure et al. (submitted)

### Reproducing figures from saved data 

Some of the figures below rely on the `sim` or `S` data frames, generated by the big (1,703,936 iterations) sensitivity analysis. You can generate those two data frames yourself, using the `biocontrol-model-sa.box` script with iterations changed to `1703936` (see [Setting up the analysis](#setting-up-the-analysis)). Just let your computer work it out over a weekend. Alternatively, you can download them ([download sim](https://tildeweb.au.dk/au152367/download/aphid-biocontrol-sim.Rdata) | [download S](https://tildeweb.au.dk/au152367/download/aphid-biocontrol-S.Rdata)). Either way, you will end up with two files with suffix (or 'file type') `.Rdata` . Put those two files in a folder of your choice.

If an `.Rdata` file is needed to produce a figure, it will be noted below, together with the name of the R script producing the figure. In that R script, you must change the file path at the top to make it read *your* `.Rdata` file. Subsequently run the script in R to generate the figure. As an example, below are the steps to generate [Figure 3](#figure-3).

First, find the location of the input folder using the  [get folders](#get-folders) command:

```
get folders
```

and open the `figure-3.R` script found there.

You will have to change three variables, all clearly marked at the top of the R script, designating

* the file name and path of either your `sim` or `S` data frame
* the name and path leading to the `input/scripts/begin.R` script, included with your installation of Universal Simulator
* the path to the folder where you want the graphics files written (the figures will be shown both on the screen in R and written to files)

Here is an example:

```
# Load your sim data
sim_data_file = "~/ipm/aphid-biocontrol-sim.Rdata"

# Load standard script
source("C:/users/joe_cool/UniversalSimulatorHome/input/scripts/begin.R")

# Output folder
output_folder = "~/ipm/figures"
```

Replace `...` and other parts as needed to reflect your folder structure. Note for the nerdy: The tilde `~` is a shorthand for your `Documents` folder. It works even on Windows!

Save and run the R script.

### Figure 2

You create this figure by running a script that is nearly identical to the `biocontrol.box` script. Here goes

```
run models/aphid/figure-2.box
```

The script creates four figures, two on the screen (one in colours, one in grey tones) and two in graphics files (one PNG and one EPS). The `figure-2.R` script creates the figures, writing the graphics files to your Universal Simulator output folder; the precise location is reported by the R script, so that you can easily retrieve them. Here is the PNG file:

<img src="../media/models/aphid/fig-2-bw.png" style="zoom:10%;" />

Since the boxscript contains uncertain parameter values, you will get different results every time you run the script.

### Figure 3

This figure is generated by the `figure-3.R` script. Follow the instructions in [Reproducing figures from saved data](#reproducing-figures-from-saved-data) and watch the result:

<img src="../media/models/aphid/fig-3-bw.png" style="zoom:10%;" />

### Figure 4

This figure is generated by the `figure-4.R` script. Follow the instructions in [Reproducing figures from saved data](#reproducing-figures-from-saved-data) and watch the result:

<img src="../media/models/aphid/fig-4-bw.png" style="zoom:10%;" />

### Figure 5

This figure is generated by the `figure-5.R` script. Follow the instructions in [Reproducing figures from saved data](#reproducing-figures-from-saved-data) and watch the result:

<img src="../media/models/aphid/fig-5-bw.png" style="zoom:100%;" />



Hey, wait. That didn't quite work out like that. The figure you generated is missing all the labels. That's because we handicrafted them afterwards, as we found it impossible to get these nice-looking labels in R. For proper labelling, you will find a text file with all figure data in `fig-5-bw-table.txt`, which was written to the output_folder by the `figure-5.R` script.

The figure depicts Sobol’ indices ($S_{Ti}$ : dark grey; $𝑆_𝑖$ : light grey) showing model sensitivity to the nine uncertain model parameters, in terms of three model outcomes: (a) peak prevalence of exposed aphids, (b) peak prevalence of cadavers, and (c) yield improvement due to biocontrol. The model parameters are listed in order of importance for each outcome according to $S_{Ti}$.

### Figure 6

This figure is generated by the `figure-6.R` script. You need to have the `mgcv` package installed in R before you run the script. Otherwise,  follow the instructions in [Reproducing figures from saved data](#reproducing-figures-from-saved-data) and watch the result:

<img src="../media/models/aphid/fig-6-bw.png" style="zoom:100%;" />

If you study the `figure-6.R` script, you will find that many a trick was pulled to produce this figure! The mathematical symbols were added outside R. 

The figure depicts the response of three model outcomes to variation in the seven most influential model parameters (excluding weather). 

### Figure 7

This figure is generated by the `figure-7.R` script. Follow the instructions in [Reproducing figures from saved data](#reproducing-figures-from-saved-data) and watch the result:

<img src="../media/models/aphid/fig-7-bw.png" style="zoom:10%;" />

### Figure 8

This figure is generated by the `figure-8.R` script. Follow the instructions in [Reproducing figures from saved data](#reproducing-figures-from-saved-data) and watch the result:

<img src="../media/models/aphid/fig-8-colour.png" style="zoom:8%;" />

### Figure 9

This figure is generated by the `figure-9.R` script. Follow the instructions in [Reproducing figures from saved data](#reproducing-figures-from-saved-data) and watch the result:

<img src="../media/models/aphid/fig-9-colour.png" style="zoom:40%;" />

### Figure 10

You create this figure by running the boxscript,

```
run models/aphid/figure-10.box
```

to get the result:

<img src="../media/models/aphid/fig-10-colour.png" style="zoom:40%;" />

### Figure 11

You create this figure by running the boxscript,

```
run models/aphid/figure-11.box
```

to get the result:

<img src="../media/models/aphid/fig-11-colour.png" style="zoom:40%;" />

# Virtual greenhouse model

## Greenhouse geometry

The greenhouse consists of $n$ spans each of width $w$ (m), length $\ell$ (m) and wall height $h$ (m). Each span has a roof on top with a pitch $u$ (&deg;). From these parameters we can derive several measures of greenhouse geometry. The area of a single face, either side, end or roof, is
$$
\begin{align}
A_{side}' &= \ell h \\[6pt]
A_{end}'  &= wh + \frac{w^2}{4}sin(u)\\[6pt]
A_{roof}' &= \frac{w\ell}{2}\,\sqrt{1+sin^2(u)}
\end{align}
$$
from which follows the total area of side, end and roof faces, and the total cover area (all in m^2^),
$$
\begin{align}
A_{side}  &= 2A_{side}' \\[6pt]
A_{end}   &= 2nA_{end}' \\[6pt]
A_{roof}  &= 2nA_{roof}' \\[6pt]
A_{cover} &= A_{side} + A_{end} + A_{roof}
\end{align}
$$
The ground area (m^2^) covered by the greenhouse is
$$
A_{gh}=n\ell w
$$
and the volume contained (m^3^) is
$$
\begin{equation}
V_{gh} = n\ell A_{end}'
\label{greenhouse_volume}
\end{equation}
$$
Further useful measures are the cover/ground ratio (m^2^ cover/m^2^ ground)
$$
\begin{equation}
\frac{A_{cover}}{A_{gh}}
\label{cover_ground_ratio}
\end{equation}
$$
and the average height (m)
$$
\begin{equation}
\frac{V_{gh}}{A_{gh}}
\label{average_height}
\end{equation}
$$

The `greenhouse-geometry.R` script exemplifies the calculation. Here are the parameter settings and the resulting measures:

```
n = 25
w = 4
L = 100
h = 3.5
u = 26

A_side             350.000
A_end               15.753
A_roof             218.373
A_cover          12406.325
A_gh             10000.000
V_gh             39383.711
cover_per_ground     1.241
avg_height           3.938
```

### Layers

The greenhouse and its environment is conceptualised as a stack of $i=1..n$ horizontal layers. For example,

1. sky
2. cover (e.g., glass)
3. screen 
4. growth light 
5. plant canopy
6. heat pipe
7. floor

Depending on the scenario, layers can be removed or added (e.g. a table layer would often make sense, or additional screen layers). All layers, whether they are horisontal or inclined, uniform or heterogeneous, thick or thin, are simplified to describe an average 1m^2^ of horisontal greenhouse area.

The radiative properties of each layer is parameterised for three different intervals of the spectrum:

* **PAR**  (&#956;mol/m^2^/s) 
* **short-wave radiation** (W/m^2^) 
* **long-wave radiation** (W/m^2^)

PAR is a subset of the short-wave radiation. The PAR budget is resolved only to compute how much is absorbed by the plant canopy layer; it is subsequently transformed to plant growth (g/m^2^/h). Its quantum units reflect this purpose. In contrast, the short- and long-wave radiation both contribute to the radiative exchange of heat among layers. Remember that W = J/s. 

<img src="..\media\models\vg\energy-budget-layers.png" alt="energy-budget-layers" style="zoom:50%;" />



Each layer $i$ is characterised by its radiative properties: **absorptivity** $(\alpha^r_i$), **reflectivity** $(\rho^r_i)$ and **transmissivity** $(\tau^r_i)$ for radiation coming from **above** and similarly for radiation coming from **below** $(\alpha{^r_i}',\rho{^r_i}',\tau{^r_i}')$, all for each type of radiation $r$ (*PAR*, *sw*, *lw*). The radiative parameters always add up to unity,   $\alpha^r_i+\rho^r_i+\tau^r_i=1$ and  $\alpha{^r_i}'+\rho{^r_i}'+\tau{^r_i}'=1$. We apply the common engineering assumption that absorptivity equals emissivity, i.e. for all layers $\epsilon_i^r=\alpha_i^r$ and ${\epsilon_i^r}'={\alpha_i^r}'$. 

The superscript $r$ was left out of the figure above to lessen clutter but it is important, as it allows different parameter settings for different radiation intervals. This is important, e.g. for the distinct difference in transmissivity of glass for short- *vs.* long-wave radiation. The separate parameter sets for down- and upgoing radiation allow for asymmetry, e.g., of screens.

Layers are further described by their area-specific heat capacity ($C_i$; J/K/m^2^ ground), and time-varying variables describing temperature ($T_i$; &deg;C) and the emission of radiation down- and upwards for PAR ($E_i^{par},\,{E_i^{par}}'$; &#956;mol/m^2^/s) and short- and long-wave radiation ($E_i^{sw},\,{E_i^{sw}}',\, E_i^{lw},\,{E_i^{lw}}'$; W/m^2^). For most layers (growth light and heat pipe layers are exceptions), the emission of long-wave radiation is calculated from their temperature according to the [Stefan-Boltzmann Law](https://en.wikipedia.org/wiki/Stefan%E2%80%93Boltzmann_law),
$$
\begin{equation}
\begin{split}
E_{i}^{lw} & =\sigma\epsilon_i(T_i+T_0)^4 \\[6pt]
{E_{i}^{lw}}' & =\sigma\epsilon_i'(T_i+T_0)^4
\end{split}
\label{stefan_boltzmann}
\end{equation}
$$
where $T_0=273.15$ &deg;C converts to absolute temperature.

### Volumes

Layers exchange heat not only between them but also with the neighbouring two volumes. For layers inside the greenhouse, both volumes are  greenhouse air. For the cover, there is outside air on the outer surface (the upper surface in our layer model). For the floor, there is soil below. The outdoors ambient temperature ($T_{out}$; &deg;C) and the soil temperature ($T_{soil}$; &deg;C) are both driving variables, while the indoors temperature ($T_{in}$; &deg;C) is a model state variable. The exchange of heat between layer $i$'s upper and lower surface and the confronting volume is described by $U_i$ on the upper side and $U_{i}'$ on the lower side (both in W/K/m^2^). The mechanism of heat exchange is convection for air volumes and conduction for the soil volume.

The heat absorbed by layer $i$ (W/m^2^) from the volume above and below is, respectively,
$$
\begin{equation}
\begin{split}
A_i^{heat} &= U_i(T_{Vi}-T_i) \\[6pt]
{A_i^{heat}}' &= U_i'({T_{Vi}}'-T_i)
\end{split}
\label{u_heat}
\end{equation}
$$
where the temperature of the volume above and below layer $i$ is denoted by $T_{Vi}$ and $T_{Vi}'$, respectively.

The $U$-values over both inner and outer surface can be expected to increase with the wind; immobile air is a very good insulator. For the outer surface of the cover, we use the empirical relation,
$$
U_{cover} = 2.8+1.2\cdot w^{0.8}
$$
where $w$ is the outdoors wind speed (m/s). The $U$-value of all other inner surfaces is set to $1.2$ W/K/m^2^ but we assume a good insulation of the floor against the soil setting  $U_{floor}'=0.1$ W/K/m^2^.

Note: All measures are per m^2^ ground. Surface areas, for instance, must be transformed to this standard, e.g. by eq. $\ref{cover_ground_ratio}$.

### Ventilation

Advective heat transfer occurs when outdoors air replaces indoors air. This is due to ventilation intended to cool the greenhouse or to reduce air humidity, as well as the inadvertent leakage. We formulate the ventilation rate as, how many times greenhouse air is replaced per hour $v$ ($\text{h}^{-1}$) .

Assuming perfect mixture of air inside the greenhouse, the advective heat gained by the greenhouse air ($P_{adv}$; W/m^2^ ground) is then 
$$
P_{adv} = \frac{v}{3\,600\text{ s/h}} \cdot (T_{out}-T_{in}) h_{gh} C_{air}
$$
where $C_{air} = 1020 \frac{\text{J}}{\text{K}\cdot\text{kg}} 
\cdot 1.19\frac{\text{kg}}{\text{m}^3} = 1\,124\;\frac{\text{J}}{\text{K}\cdot\text{m}^3}$ is the heat capacity of air. 

As an example (see the `greenhouse-ventilation.R` script) with $v=1$ or $4 \text{ h}^{-1}$, outdoor temperature in the range $T_{out}=-5..25$ &deg;C, indoors temperature $T_{in}=20$ &deg;C and average greenhouse height $h_{gh}=3.938$ m (eq. $\ref{average_height}$), we get an advective heat gain as illustrated here:

![greenhouse-ventilation](D:\Documents\sites\ecolmod3\media\models\vg\greenhouse-ventilation.png)

## Layer characterization

### Sky layer

The irradiation reaching the cover from above includes $E_{sky}^{par}$, $E_{sky}^{sw}$ and $E_{sky}^{lw}$. If sunlight PAR is not measured directly, it can be approximated by  $E_{sky}^{par}=0.5E_{sky}^{sw}$. If sky temperature $T_{sky}$ and emissivity $\epsilon_{sky}$ are not measured directly, they can be calculated from other meteorological measurements ([Karn et al. 2019](https://onlinelibrary.wiley.com/doi/full/10.1002/htj.21459)). Sky transmissivity $\tau_{sky}$ is zero and consequently reflectivity is $\rho_{sky}=1-\epsilon_{sky}$.

The sky emits long-wave radiation $E_{sky}^{lw}$ downwards according to the Stefan-Boltzmann Law (eq. $\ref{stefan_boltzmann}$).

### Cover layer

The cover layer is commonly made out of glass with the following typical characteristics:

| Glass properties                       | PAR  | Short-wave | Long-wave |
| -------------------------------------- | ---- | ---------- | --------- |
| $\alpha, \alpha', \epsilon, \epsilon'$ | 0.1  | 0.1        | 0.9       |
| $\rho, \rho'$                          | 0.1  | 0.1        | 0.1       |
| $\tau, \tau'$                          | 0.8  | 0.8        | 0.0       |


In general, the six faces of the greenhouse my carry different covers with different radiative properties. For the cover layer as a whole, each radiative parameter is summarised as the average over all faces, weighed by face area.

The density of window glass is typically $2.5$ kg/m^2^/mm, which with a thickness of $4$ mm gives a density of $10$ kg/m^2^ cover. The heat capacity of glass is $840$ J/K/g. If we furthermore assume a cover/ground ratio of 1.24 m^2^ cover/m^2^ ground (eq. $\ref{cover_ground_ratio}$), we get a heat capacity of the cover of
$$
\begin{equation}
C_{cover}= 10 \text{ kg/m}^2\text{ cover} \cdot
1.24\text{ m}^2\text{ cover/m}^2\text{ ground} \cdot
840 \text{ J/K/g}\cdot10^3 \text{g/kg}=
10\,416 \text{ kJ/m}^2\text{ ground}
\label{c_glass}
\end{equation}
$$
The cover emits long-wave radiation symmetrically down and up according to the Stefan-Boltzmann Law (eq. $\ref{stefan_boltzmann}$).

### Screen layer
Screens are often constructed with asymmetric radiative properties. Here, for example, are the radiative parameters of a polyester screen with an aluminium surface on the upper side (from [Bailey 1981](https://www.sciencedirect.com/science/article/pii/0021863481901062)): 

| Screen properties                | Long-wave radiation |
| -------------------------------- | ------------------- |
| $\alpha', \epsilon'$ (polyester) | 0.57                |
| $\rho'$ (polyester)              | 0.43                |
| $\alpha, \epsilon$ (aluminium)   | 0.07                |
| $\rho$ (aluminium)               | 0.93                |
| $\tau', \tau$                    | 0.0                 |

The values in the table are for long-wave radiation only. They are most likely different for short-wave radiation but documentation for the physical characteristics of screens are hard to come by; manufacturers seem reluctant to provide the information.

Screens may be more or less drawn . If we denote how much a curtain is drawn by $p\in[0;1]$ then the effective radiation parameters for the greenhouse face with the partly drawn screen is assumed to be, simply, $p\alpha$,  $p\rho$ and $1-p\alpha-p\rho$, and likewise for $\alpha'$, $\rho'$ and $\tau'$. For the screen layer of the greenhouse as a whole, each radiative parameter is summarised as the average over all the six faces, weighed by face area, as for the cover layer. If screens are kept half-drawn for extended periods, or if faces differ much in screen material, this averaging could turn out highly unrealistic. If the greenhouse is eqipped with more than one layer of screens, the screens are handled as separate, independent layers.

Screens may be constructed of composite materials which complicates the calculation of their heat capacity. For the screen exemplified here, we found quite close values of $C_{polyester}=1.1$ J/K/g and $C_{Al}=0.89$ J/K/g. Assuming that polyester is the dominant material and a screen density of $80$ g/m^2^ screen, we get a heat capacity of the screen of
$$
C_{screen}= 80 \text{ g/m}^2\text{ screen} \cdot
1.24\text{ m}^2\text{ screen/m}^2\text{ ground} \cdot
1.1 \text{ J/K/g}=
109 \text{ J/m}^2\text{ ground}
$$
This calculation implies that all six greenhouses faces are equally covered  with the same screen. If that is not the case then $S_{screen}$ must be found by calculating the heat capacity of each face separately and adding them up to give $S_{screen}$. Note that the calculated heat capacity of the screen is ~100 times less that of a glass cover (eq. $\ref{c_glass}$).

The screen emits long-wave radiation according to the Stefan-Boltzmann Law (eq. $\ref{stefan_boltzmann}$). With the example above, the radiation would be asymmetric because downwards and upwards emissivity differ; this screen was designed to keep the heat indoors.

### Growth light layer 

Lamps are installed to provide growth light, traditionally emitted downwards. Yet, the heat they are providing as is a significant side effect. If the installed lamp power is $P_{lamp}$ (W/m^2^) and their PAR efficiency is $e_{lamp}$ (&mu;mol PAR/J) then the downwards emission of PAR ($E_{lamp}^{par}$; &mu;mol PAR/m^2^) is
$$
E_{lamp}^{par} = e_{lamp}P_{lamp}
$$
Downwards radiation is emitted both in the short- and long-wave spectrum; we denote the proportion of long-wave radiation $p_{lamp}^{lw}\in[0;1]$.  Hence the downwards, short-wave emission ($E_{lamp}^{sw}$; W/m^2^) is
$$
E_{lamp}^{sw} = (1-p_{lamp}^{lw})P_{lamp}
$$
while the upwards, short-wave emission is ${E_{lamp}^{sw}}'=0$ .

Many lamps require a ballast ($P_{ballast}$; W/m^2^) which contributes to the long-wave radiation. We assume this radiation is emitted equally up- and downwards. Thus we arrive at the downwards and upwards long-wave radiation (both in W/m^2^), respectively,
$$
\begin{align}
E_{lamp}^{lw}    &= p_{lamp}^{lw}P_{lamp} + P_{ballast}/2 \\[6pt]
{E_{lamp}^{lw}}' &= P_{ballast}/2
\end{align}
$$
The growth light layer has a 100% transmissivity, $\tau=\tau'=1$, and is attributed neither a temperature nor a heat capacity.

As an example, with $P_{lamp}=100$ W/m^2^, $P_{ballast}=10$ W/m^2^,  $e_{lamp}=2.5$ &mu;mol PAR/J and $p_{lamp}^{lw}=0.3$, we get
$$
\begin{align}
E_{lamp}^{par} &= 2.5\; \mu\text{mol PAR/J} \cdot 100 \text{W/m}^2 & &= 250\; \mu\text{mol PAR/m}^2 \\[6pt]
E_{lamp}^{sw} &= (1-0.3)\cdot 100 \text{ W/m}^2 & &= 70 \text{ W/m}^2 \\[6pt]
E_{lamp}^{lw} &= 0.3\cdot 100 \text{ W/m}^2 + 10 \text{ W/m}^2/2 & &= 35 \text{ W/m}^2 \\[6pt]
{E_{lamp}^{lw}}' &= 10 \text{ W/m}^2/2 & &= 5 \text{ W/m}^2
\end{align}
$$

### Plant canopy layer

[Goudriaan (1977)](https://library.wur.nl/WebQuery/wurpubs/70980) provides equations (his eqs. 2.21 and 2.26) to calculate the reflectivity of a plant canopy with leaf area index $L_{ai}$ and extinction coefficient $k$. The reflection coefficient $\rho_h$ depends on the scattering coefficient $\sigma$,
$$
\rho_h=\frac{1-\sqrt{1-\sigma}}{1+\sqrt{1-\sigma}}
$$
$\sigma$ is a species-dependent parameter commonly set to $\sigma=0.2$ ([Kropff and Laar 1993, p.38](https://research.wur.nl/en/publications/modelling-crop-weed-interactions)), which gives $\rho_h=0.0557$.

The original equation 2.26 includes the reflectivity of the surface underneath the canopy (the floor in our case). However, we resolve the distribution of radiation among the greenhouse layers otherwise (as explained further down) and consequently set the reflectivity of the under lying surface in the original equation to zero. Thus we get the reflectivity of the canopy layer, which is symmetric for up- and down-going radiation,
$$
\rho_{plant}^{r} = {\rho_{plant}^{r}}' =
\frac{exp(-k^{r}L_{ai})-exp(k^{r}L_{ai})}{\rho_h exp(-k^{r}L_{ai})-exp(k^{r}L_{ai})/\rho_h} \\[6pt]
$$
where superscript $r$ denotes $par$, $sw$ or $lw$ radiation. Commonly found values for $k^{par}$ and $k^{sw}$ are in the range $0.7$ to $0.8$, while leaves are impenetrable to long-wave radiation resulting in $k^{lw}=1$.

The absorptivity of the canopy is simply
$$
\alpha_{plant}^r = {\alpha_{plant}^r}' = 1-exp(-k^rL_{ai})
$$
and the transmissivity
$$
\tau_{plant}^r = {\tau_{plant}^r}' = 1- \rho_{plant}^r - \alpha_{plant}^r
= 1- {\rho_{plant}^r}'- {\alpha_{plant}^r}'
$$
The `plant-canopy-layer.R` script shows how the parameters change with increasing leaf area index for short-wave ($k=0.8$) vs. long-wave radiation ($k=1$):

![](..\media\models\vg\plant-canopy-layer.png)

The canopy emits long-wave radiation symmetrically down and up according to the Stefan-Boltzmann Law (eq. $\ref{stefan_boltzmann}$).

### Heat pipe layer

Heat pipes are installed to heat the greenhouse by convection and long-wave radiation. Its transmissivity is $\tau_{pipe}=\tau_{pipe}'=1$ for all wave lengths.

The drop in temperature ($\Delta T_{pipe}$; K) from the inlet to the outlet is modelled by the empirical equation
$$
\Delta T_{pipe} = \left( T_{pipe}^{inlet} - T_{in}\right) -
\left[k(b-1)\Delta t_{pipe} + \left(T_{pipe}^{inflow} - T_{in} \right)^{1-b}\right]^\frac{1}{1-b}
$$
where $k$ and $b$ are parameters calibrated to the greenhouse, $\Delta t_{pipe}$ (s) is the transit time of pipe water, $T_{pipe}^{inlet}$ (&deg;C) is the water temperature at the pipe inlet, and $T_{in}$ (&deg;C) is greenhouse air temperature. In case the inlet temperature is not above the greenhouse temperature $\Delta T_{pipe}=0$.

The transit time is related to the flow rate $\dot{v}_{pipe}$ (m^3^/h) and pipe volume $V_{pipe}$ (m^3^) as
$$
\Delta t_{pipe} = \frac{V_{pipe}}{\dot{v}_{pipe}}
$$
As an example, a pipe with an inner diameter of 30 mm installed at a density of 2 m/m^2^ in a greenhouse area of 10 000 m^2^ holds the volume,
$$
\begin{equation}
V_{pipe} = \frac{\pi}{4}\cdot\left(30\text{ mm}\cdot\frac{1\text{ m}}{1\,000\text{ mm}} \right)^2 \cdot 
2 \frac{\text{m}}{\text{m}^2}\cdot10\,000\text{ m}^2 = 14.14\text{ m}^3
\label{eq_Vpipe}
\end{equation}
$$
The total energy emitted by the heat pipe ($E_{pipe}^*$;  W/m^2^ ground) is
$$
E_{pipe}^* = \frac{C_{water}\Delta T_{pipe}V_{pipe}}{A_{gh}\Delta t_{transit}}\cdot1000\frac{\text{kg}}{\text{m}^3}
$$
where $C_{water}=4\,184$ J/K/kg is the heat capacity of water and $A_{gh}$ (m^2^ ground) is the ground area of the greenhouse.

To continue the example from eq. $\ref{eq_Vpipe}$, we set $k=0.00629$ and $b=1.25$ with an indoors temperature $T_{in}=20$ &deg;C, a range of inlet temperatures $T^{inlet}_{pipe}=20..30$  &deg;C and two different flow rates $\dot{v} = (5,10)$ m^3^/h. We then get the following drop in water temperature reached at the pipe outlet and the associated energy $E_{pipe}^*$ emitted to the greenhouse:

![](..\media\models\vg\heat-pipe.png)

Heat pipe energy is emitted as a combination of radiant and convective heat.  We denote the proportion of long-wave radiation $p_{pipe}^{lw}\in[0;1]$ and, assuming that it is raddiated equally down- and upwards, we get
$$
E^{lw}_{pipe} = {E^{lw}_{pipe}}' = p^{lw}_{pipe}E^*_{pipe}/2
$$
The other part is conveyed to the greenhouse air. Since convective heat is defined relative to the layers (eq. $\ref{u_heat}$), the convective heat flow from the heat pipes is negative,
$$
A^{heat}_{pipe} = -\left(1-p^{lw}_{pipe} \right)E^*_{pipe}
$$
Since the volume above and below the heat pipes is the same, i.e. the greenhouse air, we let $A^{heat}_{pipe}$ account for the whole heat exchange and set ${A^{heat}_{pipe}}'=0$ (see eq. $\ref{u_heat}$).

### Floor layer

A floor made of concrete has typical values $\alpha_{floor}=0.6$ and $\rho_{floor}=0.4$. 



## Energy budget

At any time a layers is emitting radiation downwards ($E_i^r$) and upwards (${E_i^r}'$) in units of &#956;mol/m^2^/s for $E_i^{par}$ and ${E_i^{par}}'$ and in W/m^2^ for the others ($sw$ and $lw$).

### Short-wave radiation and PAR

As radiation is emitted, absorbed, reflected and transmitted by the layers, a net radiation flux will result downwards $(F^{sw}_i, F^{par}_i, F^{lw}_i)$ and upwards $(F{^{sw}_i}', F{^{par}_i}', F{^{lw}_i}')$  for each layer $i$ and in each range of radiation, superscripted as short-wave $(sw)$, PAR $(par)$ and long-wave $(lw)$. 

The net radiation fluxes throughout the stack of layers are resolved by resolving them two layers at a time. Consider layer $i$ on top of the layer   $i+1$ below. Our first aim is to find out, how much of the downwards radiation from the upper layer (e.g, short-wave radiation $F_i^{sw}$) is absorbed by the lower layer? For this net absorption from above  $\hat{\alpha}_{i+1}$, we immediatly have
$$
\hat{\alpha}_{i+1} = \alpha_{i+1}
$$
However, a fraction of the light will be reflected by the upperside of the lower layer and then re-reflected from the underside of the upper layer. We must add that bit,
$$
\hat{\alpha}_{i+1} = \alpha_{i+1} + \alpha_{i+1}\rho_{i+1}\rho_i'
$$
and so forth as photons keep bouncing between the two layers,
$$
\hat{\alpha}_{i+1} = \alpha_{i+1} + \alpha_{i+1}\left(\rho_{i+1}\rho_i'\right)^1 + \alpha_{i+1}\left(\rho_{i+1}\rho_i'\right)^2 + \ldots
=\alpha_{i+1}\sum_{j=0}^\infty \left(\rho_{i+1}\rho_i'\right)^j
$$
Since the reflectivies are less than one, the infinite series will converged as
$$
r_i = \sum_{j=0}^\infty \left(\rho_{i+1}\rho_i'\right)^j=\frac{1}{1-\rho_{i+1}\rho_i'}
$$
The net transmission through the lower layer $\hat\tau_{i+1}$ follows the same logic and we arrive at
$$
\begin{split}
\hat\alpha_{i+1} &= r_i\alpha_{i+1} \\[6pt]
\hat\tau_{i+1} &= r_i\tau_{i+1}
\end{split}
$$
On every bounce of the radiation, fractions will be lost to absorption by and transmission through the upper layer. Thus the calculations for the underside of the upper layer are the same, except that the we start out with what's first reflected upwards from the lower layer. Hence, we get
$$
\begin{split}
\hat\alpha_i' &= r_i'\alpha_i' \\[6pt]
\hat\tau_i &= r_i'\tau_i'
\end{split}
$$
where
$$
r_i' = \rho_{i+1}r
$$
Let's check that we have not accounted for the fate all radiation downwards from layer $i$. We have split it into radiation absorbed by and transmitted through either of the to layers. In total we get
$$
\begin{split}
\hat\alpha_{i+1}+\hat\tau_{i+1}+\hat\alpha_i'+\hat\tau_i' &= \\[6pt]
\frac{\alpha_{i+1}+\tau_{i+1}+\rho_{i+1}(\alpha_i'+\tau_i')}{1-\rho_{i+1}\rho_i'}&= \\[6pt]
\frac{(1-\rho_{i+1})+\rho_{i+1}(1-\rho_i')}{1-\rho_{i+1}\rho_i'}&= \\[6pt]
\frac{1-\rho_{i+1}+\rho_{i+1}-\rho_{i+1}\rho_i'}{1-\rho_{i+1}\rho_i'}&= 1
\end{split}
$$
Thus we have accounted for it all.

Having accounted for the fate of the downwards radiation from layer $i$, we can now repeat the calculation for the next pair of layers, $i+1$ and $i+2$, and so forth until the bottom of the stack. The downwards flow from any layer $i$ is the inherent emission $E_i^{sw}$ (i.e. for a layer of growth lights) plus the transmission from the layer above $F_{i-1}^{sw}$,
$$
F_i^{sw}=E_i^{sw} + F_{i-1}^{sw}
$$
After we have finished the distribution downwards radiation, we repeat the procedure this time going from the bottom to the top following the upwards radiation,
$$
{F_i^{sw}}' = {E_i^{sw}}' + {F_{i-1}^{sw}}'
$$
The same logic applies. We repeat the calculations downwards-upwards-downwards-upwards-*etc.*, on each pass accumulating in each  layer the radiation absorbed from above $A{^{sw}_i}$ and below $A{^{sw}_i}'$. The calculation stops once until all nearly radiation has been absorbed, striving for a certain numerical accuracy, e.g. until less than $10^{-4}$ W/m^2^ remains unabsorbed. Note that the emissions downwards $E_i^{sw}$ and upwards $E_i{^{sw}}'$ are only added to the flows (downwards $F_i^{sw}$ and upwards $F_i{^{sw}}'$, respectively) on the first pass through the calculations.

The total, absorbed short-wave radiation $A{^{sw}_i}+A{^{sw}_i}'$ is used to update the temperature of each layer $i$, taking the absorbed long-wave radiation and the heat conveyed by convection into account as well, as explained further down.

The absorption of PAR among layers is computed just as the short-wave radiation (PAR *is* a part of the short-wave spectrum) but the emission from some layers (e.g., sun, growthligts) and the radiative properties of some layers (e.g., plant canopy) will differ between the the short-wave and the PAR spectrum. The calculation of absorbed PAR $A{^{PAR}_i}+A{^{PAR}_i}'$  is used only for the plant canopy layer, namely to compute the resulting photosynthesis.

#### Computation example

The `energy-budget-sw.R` script demonstrates the calculations. Here the light layer emits 100 W/m2 downwards (or 100 &#956;mol/m^2^/s; the calculations are the same) with Greek letters represented by their Latin equivalent and primes replaced by underscores:

```
        Layer    a    r    t   a_   r_   t_   E E_   F F_ A A_
1         Sky 1.00 0.00 0.00 1.00 0.00 0.00   0  0   0  0 0  0
2       Glass 0.03 0.08 0.89 0.03 0.08 0.89   0  0   0  0 0  0
3      Screen 0.10 0.60 0.30 0.10 0.60 0.30   0  0   0  0 0  0
4       Light 0.00 0.00 1.00 0.00 0.00 1.00 100  0 100  0 0  0
5       Plant 0.71 0.05 0.24 0.71 0.05 0.24   0  0   0  0 0  0
6     Heating 0.00 0.00 1.00 0.00 0.00 1.00   0  0   0  0 0  0
7       Floor 0.60 0.40 0.00 1.00 0.00 0.00   0  0   0  0 0  0
```

After the first distribution of radiation downwards, we have

```
        Layer    a    r    t   a_   r_   t_   E E_ F  F_    A A_
1         Sky 1.00 0.00 0.00 1.00 0.00 0.00   0  0 0 0.0  0.0  0
2       Glass 0.03 0.08 0.89 0.03 0.08 0.89   0  0 0 0.0  0.0  0
3      Screen 0.10 0.60 0.30 0.10 0.60 0.30   0  0 0 0.0  0.0  0
4       Light 0.00 0.00 1.00 0.00 0.00 1.00 100  0 0 5.0  0.0  0
5       Plant 0.71 0.05 0.24 0.71 0.05 0.24   0  0 0 0.0 71.0  0
6     Heating 0.00 0.00 1.00 0.00 0.00 1.00   0  0 0 9.6  0.0  0
7       Floor 0.60 0.40 0.00 1.00 0.00 0.00   0  0 0 0.0 14.4  0
```

We can see that $71.0 + 14.4 = 85.4\%$ of the radiation has been absorbed by plants and floor, respectively, while $5.0+9.6=14.6\%$ has been reflected upwards by the same layers. We carry on distributing these $14.6\%$ upwards and get

```
        Layer   a   r   t  a_  r_  t_   E E_   F F_    A  A_
1         Sky 1.0 0.0 0.0 1.0 0.0 0.0   0  0 0.0  0  0.0 2.0
2       Glass 0.0 0.1 0.9 0.0 0.1 0.9   0  0 0.0  0  0.0 0.1
3      Screen 0.1 0.6 0.3 0.1 0.6 0.3   0  0 0.1  0  0.0 0.7
4       Light 0.0 0.0 1.0 0.0 0.0 1.0 100  0 4.4  0  0.0 0.0
5       Plant 0.7 0.0 0.2 0.7 0.0 0.2   0  0 0.0  0 71.0 6.8
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0   0  0 0.5  0  0.0 0.0
7       Floor 0.6 0.4 0.0 1.0 0.0 0.0   0  0 0.0  0 14.4 0.0
```

A total of $2.0+0.1+0.7+6.8=9.6\%$ has been absorbed on the underside of various layers (sky, glass, screen, plant), while reflection downwards has led to the remaining radiation of $0.1+4.4+0.5=5.0\%$ hitting some layers (screen, light, heating) from above.

If we set the precision, so that the calculations down and up will be repeated until $\sum F_i+\sum F_i'<0.1$, it turns out that three iterations are needed, and we get the final distribution,

```
        Layer   a   r   t  a_  r_  t_   E E_ F F_    A  A_
1         Sky 1.0 0.0 0.0 1.0 0.0 0.0   0  0 0  0  0.0 2.2
2       Glass 0.0 0.1 0.9 0.0 0.1 0.9   0  0 0  0  0.0 0.1
3      Screen 0.1 0.6 0.3 0.1 0.6 0.3   0  0 0  0  0.0 0.8
4       Light 0.0 0.0 1.0 0.0 0.0 1.0 100  0 0  0  0.0 0.0
5       Plant 0.7 0.0 0.2 0.7 0.0 0.2   0  0 0  0 74.3 7.3
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0   0  0 0  0  0.0 0.0
7       Floor 0.6 0.4 0.0 1.0 0.0 0.0   0  0 0  0 15.4 0.0
```

Thus $74.3+7.3=81.6\%$ of the radiation ended up in the plant canopy (which is the intended target of growth lights). Most of the remaining light was absorbed by the flow $(15.4\%)$, where it will be converted to heat (see further down), like what little ended up in the glass and screen. Only $2.2\%$ escaped to the sky; this is the light that you can see escaping obviously from greenhouses at night. 

### Long-wave radiation

Each layer (with the exception of lights and heating) emits long-wave radiation according to Boltzman's Law, both downwards $E_i^{lw}$ and upwards $E_i{^{lw}}'$,
$$
E_i^{lw}=\sigma\epsilon_iT_i^4\;\;\;\text{and}\;\;\;{E_i^{lw}}'=\sigma\epsilon_{i+1}'(T_i')^4
$$
The emitted long-wave radiation is absorbed by the neighbouring layers according to their absorptivity,
$$
{A_{i-1}^{lw}}' = \alpha_{i-1}'{E_{i}^{lw}}'\;\;\;\text{and}\;\;\;A_{i+1}^{lw} = \alpha_i E_{i+1}^{lw}
$$
The long-wave radiation of lights and heating is computed from specific models that do not involve Boltzman's Law.

The same algorithm as for short-wave radiation is used to resolve the fate of long-wave radiation, how the absorption is distributed among the layers. However, the absorption of radiation (both short-wave and long-wave radiation) will cause a change in the temperature of the layer according to its heat capacity. This problem is solved numerically by repeating the distribution of radiation among layers and updating their temperatures until a certain precision is reached. Note that the distribution of short-wave radiation remains constant because it is temperature-independent. Only the long-wave emission of the layers will change and converge as a solution is found in which all temperatures are stable and by that, the heat exchange by radiation is in a steady state.

In the stead-state situation, absorptivity equals emissivity, i.e. for all layers $\epsilon_i=\alpha_i$ and $\epsilon_i'=\alpha_i'$.

#### Computation example

The `energy-budgel-lw.R` script demonstrates the calculations, which follow the same logic as for short-wave radiation above but with some extra parameters and calculation steps. The extra parameters for each layer $i$ are

* Temperature $T_i$ (&deg;C)
* Area-specific heat capacity $C_i$ (J/K/m^2^)

Here is the initial set-up before the long-wave radiation budget has been resolved:

```
        Layer   a   r   t  a_  r_  t_   T     C     E    E_     F    F_    A  A_
1         Sky 0.0 0.0 1.0 1.0 0.0 0.0 -10   Inf 271.9   0.0 271.9   0.0  0.0 2.2
2       Glass 0.9 0.1 0.0 0.9 0.1 0.0  15  8400 344.0 344.0 344.0 344.0  0.0 0.1
3      Screen 0.1 0.9 0.0 0.6 0.4 0.0  18  2280 232.3  28.5 232.3  28.5  0.0 0.8
4       Light 0.0 0.0 1.0 0.0 0.0 1.0  NA   Inf   0.0   0.0  10.0  10.0  0.0 0.0
5       Plant 0.8 0.0 0.2 0.8 0.0 0.2  24   Inf 349.3 349.3 349.3 349.3 74.3 7.3
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0  NA   Inf   0.0   0.0 100.0 100.0  0.0 0.0
7       Floor 0.6 0.4 0.0 1.0 0.0 0.0  12 42000 374.9 224.9 374.9 224.9 15.4 0.0
```

We have transferred the absorbed short-wave radiation from the example above (columns `A` and `A_` has been initialised to $A_i^{sw}$ and ${A_i^{sw}}'$, respectively). The emissions (`E` and `E`) have been set according to layer temperatures and emissivities applying Boltzman's Law. Note, again, that we can use absorptivity as a stand-in for emissivity. It also worth noting that screens typically have asymmetric emissivity ($\epsilon_i \neq \epsilon_i'$). A sky emissivity of 1 suggest a sky with no cloud cover.

The emission from light (10 W/m^2^ both up and down) and heating (100 W/m^2^ both up and down) layers are not calculated using Boltzman's law but enter directly (as `F` and `F_`); they have no temperature associated with them for these calculations.

The sky, plant and soil layers are all given an infinite heat capacity. Consequently, their temperatures will not change. Sky and soil temperature play the role of driving variables for the energy budget, while plant temperature is updated according to a plant physiological sub-model afthet the physical energy budget has been resolved.

We use the same algorithm as demonstrated for short-wave algorihm to distribute the flows of long-wave radiation. Note that we keep the temperatures constant and that the flows (`F` and `F_`) are distributed among the absorptions (`A` and `A_`) being added to the already absorbed short-wave radiation. We arrive at

```
        Layer   a   r   t  a_  r_  t_   T     C     E    E_ F F_     A    A_
1         Sky 0.0 0.0 1.0 1.0 0.0 0.0 -10   Inf 271.9   0.0 0  0   0.0 378.8
2       Glass 0.9 0.1 0.0 0.9 0.1 0.0  15  8400 344.0 344.0 0  0 239.3 345.2
3      Screen 0.1 0.9 0.0 0.6 0.4 0.0  18  2280 232.3  28.5 0  0  27.4 269.9
4       Light 0.0 0.0 1.0 0.0 0.0 1.0  NA   Inf   0.0   0.0 0  0   0.0   0.0
5       Plant 0.8 0.0 0.2 0.8 0.0 0.2  24   Inf 349.3 349.3 0  0 426.1 435.8
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0  NA   Inf   0.0   0.0 0  0   0.0   0.0
7       Floor 0.6 0.4 0.0 1.0 0.0 0.0  12 42000 374.9 224.9 0  0 341.6 374.9
```

We can now update the layer temperatures according to the net absorbed radiation. For example, we have for the glass layer ($i=2$),
$$
\Delta T_2=\frac{A_2+A_2'-E_2-E_2'}{C_2}\Delta t_s
$$
If the change in temperature ($T_i$) of any layer is too large, it makes the calculation imprecise since $E_i$ depends on $T_i$. If $\Delta t$ is the simulation time step, we can sub-divide this into $n_s$ time steps of size $\Delta t_s=\Delta t/n_S$ to achieve sufficiently small $|\Delta T_i|$ for all $i$. For this example, we'll set $\Delta t=180 \text{ s}$ and $n_s=6$. To complete the calculation for the glass layer, we get
$$
\Delta T_2=\frac{239.3+345.2-344.0-344.0 \text{ W}}{8400 \text{ J/K/m}^2}\cdot30\text{ s} = -0.37 \text{ }^\circ\text{C}
$$
We update the temperature of screen and floor likewise and get

```
        Layer   a   r   t  a_  r_  t_     T     C     E    E_ F F_ A A_
1         Sky 0.0 0.0 1.0 1.0 0.0 0.0 -10.0   Inf 271.9   0.0 0  0 0  0
2       Glass 0.9 0.1 0.0 0.9 0.1 0.0  14.6  8400 344.0 344.0 0  0 0  0
3      Screen 0.1 0.9 0.0 0.6 0.4 0.0  18.5  2280 232.3  28.5 0  0 0  0
4       Light 0.0 0.0 1.0 0.0 0.0 1.0    NA   Inf   0.0   0.0 0  0 0  0
5       Plant 0.8 0.0 0.2 0.8 0.0 0.2  24.0   Inf 349.3 349.3 0  0 0  0
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0    NA   Inf   0.0   0.0 0  0 0  0
7       Floor 0.6 0.4 0.0 1.0 0.0 0.0  12.1 42000 374.9 224.9 0  0 0  0
```

In summary, the glass was cooled by 0.37 &deg;C, while screen and floor was heated by 0.48 &deg;C and 0.08 &deg;C, respectively, during the 30 s time step. The net absorption by sky and plant are both ignored for now. The former is a sink, while the plant net radiative absorption is used in the plant physiological model. The soil exhanges no heat with the floor through conduction only (see further down).

We repeat the calculation above $n_s$ times to get the end results after $\Delta t=180$ s,

```
        Layer   a   r   t  a_  r_  t_     T     C     E    E_ F F_ A A_
1         Sky 0.0 0.0 1.0 1.0 0.0 0.0 -10.0   Inf 271.9   0.0 0  0 0  0
2       Glass 0.9 0.1 0.0 0.9 0.1 0.0  12.9  8400 335.6 335.6 0  0 0  0
3      Screen 0.1 0.9 0.0 0.6 0.4 0.0  20.5  2280 239.3  29.4 0  0 0  0
4       Light 0.0 0.0 1.0 0.0 0.0 1.0    NA   Inf   0.0   0.0 0  0 0  0
5       Plant 0.8 0.0 0.2 0.8 0.0 0.2  24.0   Inf 349.3 349.3 0  0 0  0
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0    NA   Inf   0.0   0.0 0  0 0  0
7       Floor 0.6 0.4 0.0 1.0 0.0 0.0  12.5 42000 377.1 226.3 0  0 0  0
```

While the energy fluxes for glass, screen and floor is accumulated in their respective temperature changes, this is not the case for the sky and plant layers. For the sky layer, we calculate the net absorption in every of the $n_s$ minor time steps ($\Delta A_{1j}$; J/m^2^) as
$$
\Delta A_{1j} = \left(A_{1j}'-E_{1j}\right)\Delta t_s\;\;\;\text{for}\;\;\;j=1\ldots n_s
$$
where the net absorption is constituted by radiation absorbed from below $A_{1j'}$ minus the downwards emission $E_{1j}$. The sum $\Delta A_{1*} = \sum_j \Delta A_{1j}$ denotes total absorption by the sky over the whole simulation time step $\Delta t$.

The net absorption by the plant in layer 5 is calculated likewise, except the plant players exchanges radiation with the layer both above and below,
$$
\Delta A_{5j} = \left(A_{5j}+A_{5j}'-E_{5j}-E_{5j}' \right)\Delta t_s\;\;\;\text{for}\;\;\;j=1\ldots n_s
$$
The energy accumulated by the sky and plant layers were $A_{1*}=18\,471 \text{ J/m}^2$ and $A_{5*}=30\,064 \text{ J/m}^2$, respectively. To verify that the algorithm is correct, we can calculate the net absorption for the glass, screen and floor layers based on their temperature change $\Delta T_i$ during the whole simulation step. We get
$$
\begin{align}
\Delta A_{2*} &= \Delta T_2 C_2 = -2.1\text{ K}\,\cdot\,8\,400\text{ J/K/m}^2 &= -17\,724 \text{ J/m}^2\\[6pt]
\Delta A_{3*} &= \Delta T_3 C_3 = 2.5\text{ K}\,\cdot\,2\,280\text{ J/K/m}^2 &= 5\,810 \text{ J/m}^2\\[6pt]
\Delta A_{7*} &= \Delta T_7 C_7 = 0.5\text{ K}\,\cdot\,42\,000\text{ J/K/m}^2 &= 20\,965 \text{ J/m}^2
\end{align}
$$
Noting that the light and heating layers absorb no radiation, i.e. $\Delta A_{4*}=\Delta A_{6*}=0 \text{ J/m}^2$, we can now calculate the total energy absorbed by the system,
$$
\sum_i \Delta A_i*=18\,471-17\,724+5\,810+30\,064+20\,965=57\,591\text{ J/m}^2
$$
This can be compare against the total energy put into the system, namely the $100 \text{ W/m}^2$ short-wave radiation plus the  $20+200 \text{ W/m}^2$. Over the the whole simulation time step this amounts to
$$
(100+20+200)\text{ W/m}^2\,\cdot\,180\text{ s}=57\,600 \text{ J/m}^2
$$
which verifies the sanity of our calculations. The small discrepancy of $57\,600-56\,591=9\text{ J/m}^2$ is due to the numerical inaccuracy when resolving the distribution of radiation among layers.

### Convective and conductive heat transfer

Unlike the exchange of radiation, which occurs directly between more or less distant layers, convective heat transfer occurs between a layer and its surrounding volume of air. The floor layer exchanges heat by convection with the greenhouse air above and by conduction with the soil below We define three volumes, each characterised by their temperature:

1. Outdoors air ($T_{out}$; &deg;C)
2. Indoors air ($T_{in}$; &deg;C)
3. Outdoors soil ($T_{soil}$; &deg;C)

The cover exchanges heat with the outdoors air on the outer surface and the inside air on the inner surface. The floor exchanges convective heat with the indoors air and conductive heat with the outdoors soil. All other layers exchange convective heat  with the indoors air on both upper and lower surfaces.

The heat absorbed by layer $i$ from above is
$$
A_i^{heat}=U_i(T_{Vi}-T_i)
$$
and from below,
$$
{A_i^{heat}}'=U_i'({T_{Vi}}'-T_i)
$$
with specific heat transfer coefficents for the upper and lower surfaces ($U$ and $U'$; W/K/m^2^). The temperature of the volume above and below layer $i$ ($T_{Vi}$ and $T_{Vi}'$, respectively) refer to the appropriate $T_{out}$, $T_{in}$  or $T_{soil}$. We must keep track of the heat absorbed by each volume $\left(A_{out}^{heat}, A_{in}^{heat}, A_{soil}^{heat}\right)$, although we consider $(T_{out})$ and $(T_{soil})$ driving variables; only $T_{in}$ is modelled as a state variable. The area-specific heat capacity of the air is
$$
C_{air} = 1020 \frac{\text{J}}{\text{K}\cdot\text{kg}} 
\cdot 1.19\frac{\text{kg}}{\text{m}^3} 
\cdot 4.4 \frac{\text{m}^3}{\text{m}^2} 
= 5\,341 \frac{\text{J}}{\text{K}\cdot\text{m}^2}
$$
The $U$-values over both inner and outer surface can be expected to increase with the wind; immobile air is a very good insulator. For the outer surface of the glass, we use the empirical relation,
$$
U_2' = 2.8+1.2\cdot v^{0.8}
$$
where $v$ is the outdoors wind speed (m/s).

The $U$-value of all other inner surfaces is set to $1.2$ W/K/m^2^, but we assume a good insulation of the floor against the soil setting that $U_7'=0.1$ 0.1 W/K/m^2^.

Convective and conductive heat transfer needs to be added to the short-wave and long-wave radiation budge, as demonstrated in the following.

#### Computation example

In the `energy-budget-U.R` script we have added columns for $U_i$ and $U_i'$ (`U` and `U_`) and absorbed heat $A_i^{heat}$ and  ${A_i^{heat}}'$ (`H` and `H_`) to the layer budget. In addition, we need a separate budget for the three volumes with columns `T`, `C` and `H` representing $T_j$, $C_j$ and $A_j^{heat}$ ($j=out, in, soil$):

```
        Layer   a   r   t  a_  r_  t_   T     C   U  U_ E E_ F F_ A A_ H H_
1         Sky 0.0 0.0 1.0 1.0 0.0 0.0 -10   Inf 0.0 0.0 0  0 0  0 0  0 0  0
2       Glass 0.9 0.1 0.0 0.9 0.1 0.0  15  8400 6.4 3.3 0  0 0  0 0  0 0  0
3      Screen 0.1 0.9 0.0 0.6 0.4 0.0  18  2280 1.2 1.2 0  0 0  0 0  0 0  0
4       Light 0.0 0.0 1.0 0.0 0.0 1.0  NA   Inf 0.0 0.0 0  0 0  0 0  0 0  0
5       Plant 0.8 0.0 0.2 0.8 0.0 0.2  24   Inf 1.2 1.2 0  0 0  0 0  0 0  0
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0  NA   Inf 0.0 0.0 0  0 0  0 0  0 0  0
7       Floor 0.6 0.4 0.0 0.0 0.0 1.0  12 42000 1.2 0.1 0  0 0  0 0  0 0  0

    Volume  T    C H
1 Outdoors 10  Inf 0
2  Indoors 25 5341 0
3     Soil  6  Inf 0
```

All the functionality of the short-wave (`energy-budget-sw.R`) and long-wave radiation (`energy-budget-lw.R`) is kept while the heat exchange between layers and volumes have been added. We end up with this solution of the energy budget:

```
        Layer   a   r   t  a_  r_  t_     T     C   U  U_     E    E_ F F_ A A_ H H_
1         Sky 0.0 0.0 1.0 1.0 0.0 0.0 -10.0   Inf 0.0 0.0 271.9   0.0 0  0 0  0 0  0
2       Glass 0.9 0.1 0.0 0.9 0.1 0.0  13.0  8400 6.4 3.3 336.0 336.0 0  0 0  0 0  0
3      Screen 0.1 0.9 0.0 0.6 0.4 0.0  19.9  2280 1.2 1.2 237.5  29.2 0  0 0  0 0  0
4       Light 0.0 0.0 1.0 0.0 0.0 1.0    NA   Inf 0.0 0.0   0.0   0.0 0  0 0  0 0  0
5       Plant 0.8 0.0 0.2 0.8 0.0 0.2  24.0   Inf 1.2 1.2 349.3 349.3 0  0 0  0 0  0
6     Heating 0.0 0.0 1.0 0.0 0.0 1.0    NA   Inf 0.0 0.0   0.0   0.0 0  0 0  0 0  0
7       Floor 0.6 0.4 0.0 0.0 0.0 1.0  12.3 42000 1.2 0.1   0.0 225.7 0  0 0  0 0  0

    Volume  T    C H
1 Outdoors 10  Inf 0
2  Indoors 23 5341 0
3     Soil  6  Inf 0
```

The final output of the script checks that all energy to the system (the short-wave $100$ W/m^2^) has been accounted for:

```
glass_dT              -2.0
screen_dT              1.9
floor_dT               0.3
indoors_dT            -2.0

watt_influx          100.0
total_influx       18000.0

sky_absorbed       18500.5
glass_absorbed    -16611.2
screen_absorbed     4236.2
plant_absorbed      5869.7
floor_absorbed     11948.2

indoors_absorbed  -10816.7
outdoors_absorbed   4763.2
soil_absorbed        110.2
total_absorbed     18000.0
```

Explanation:

* The `_dT` variables show changes in temperature (&deg;C).
* The $100$ W/m^2^ amounts to $18\,000$ J/m^2^ `total_influx` during the 3 minutes simulation step.
* The `_absorbed` variables show the final distribution of energy (J/m^2^) absorbed among 5 layers and 3 volumes.
* `total_absorbed` equals the `total_influx` as expected.

Both glass and indoors air lost 2.0 &deg;C in temperature. This energy, together with the energy put into the system at $100$ W/m^2^, was distributed among the other layers, either lost to the surroundings (sky, outdoors, soil) or absorbed (screen, glass, plant, floor).

### Advective heat transfer

Advective heat transfer occurs through outdoors air replacing indoors air. This is due to inadvertent leakage as well as intended ventilation to cool the greenhouse air or reduce air humidity. We formulate the ventilation rate as how many times greenhouse air is replaced per hour $v$ ($\text{h}^{-1}$) . Over a time step $\Delta t$ this leads to the realised rate of air exchange:
$$
v_r = 1-exp\left(-\frac{v}{3\,600\text{ s/h}}\Delta t\right)
$$
i.e. for $v=1 \text{ h}^{-1}$ and $\Delta t = 180\text{ s}$, we get  $v_r=1-exp(-0.05)=0.0488$. For small $v\Delta t$, basic mathematics tells us that $v_r\approx v\Delta t$.

The volume of air exchanged with the outdoors per m^2^ ($V_r$; m^3^/m^2^) is 
$$
V_r=hv_r
$$
where $h$ (m) is the average height of the greenhouse. The change in indoors temperture is simply
$$
\Delta T_{in} =  V_r\left(T_{out}-T_{in} \right)
$$
and the advective heat gained by the greenhouse air becomes
$$
A_{in}^{adv} = \Delta T C_{air}^{vol}
$$
where $C_{air}^{vol} = 1020 \frac{\text{J}}{\text{K}\cdot\text{kg}} 
\cdot 1.19\frac{\text{kg}}{\text{m}^3} = 1\,124\;\frac{\text{J}}{\text{K}\cdot\text{m}^3}$ is the volumetric heat capacity of air. 

As an example with $v=1 \text{ h}^{-1}$, $\Delta t = 30\text{ s}$, $T_{out}=10$ &deg;C and $T_{in}=25$ &deg;C, we get $\Delta T_{in}=-1.1$ &deg;C and $A_{in}^{adv}=-1\,226$ J/m^2^.

#### Computation example

We only need to change the previous script a little to arrive at the final `energy-budget-complete.R` script, which ends up with this outcome:

```
glass_dT              -2.0
screen_dT              1.8
floor_dT               0.3
indoors_dT            -3.1

watt_influx          100.0
total_influx       18000.0

sky_absorbed       18493.6
glass_absorbed    -16892.5
screen_absorbed     4039.3
plant_absorbed      5639.5
floor_absorbed     11837.6

indoors_absorbed  -16652.1
outdoors_absorbed  11424.4
soil_absorbed        110.1
total_absorbed     18000.0
```

Not surpricingly, those components directly affected by ventilation and leakage are affected the most (`indoors_absorbed` and `outdoors_absorbed`). The consequences in terms of temperatures are a large drop in indoors temperature (`indoors_dT` from &minus;2.0 to &minus;3.1  &deg;C) and a slight drop in screen temperature  (`screen_dT` from 1.9 to 1.8  &deg;C) .

#### Scenarios

The examples above were all based on a scenario in which the only controlled input to the system was lamps emiting 100 W/m^2^ short-wave radiation downwards, We can extend this by adding

* lamps emitting 
  * 15 W/m^2^ long-wave radiation up and down 
  * 10 W/m^2^ by convection up and down
* heating emitting 
  * 50 W/m^2^ long-wave radiation up and down 
  * 75 W/m^2^ by convection up and down 

This scenario is implemented in the `energy-budget-scenario.R` script, which gives the following result:

```
glass_dT              -1.9
screen_dT              3.2
floor_dT               0.4
indoors_dT             1.8
watt_influx          400.0
total_influx       72000.0
sky_absorbed       18523.6
glass_absorbed    -15641.3
screen_absorbed     7408.4
plant_absorbed     21183.4
floor_absorbed     18382.7
indoors_absorbed    9490.8
outdoors_absorbed  12541.1
soil_absorbed        111.3
total_absorbed     72000.0
```

The extra $300$ W/m^2^ from long-wave radiation and convective heating on top of the $100$ W/m^2^ short-wave radiation yields a total energy input of $72$ kJ/m^2^ during the $180$ s time step. Most of the extra energy is absorbed by the plant canopy and indoors air.

## Screen control

### Overview

There are up to **three layers** of screens. Counting from the outside in, they are numbered 1, 2, 3. Since the greenhouse has **six faces**, it can be equipped with up to 18 screens in total. The configuration of screens—their physical characteristics, which screen goes where and in which layer—is defined inside the `greenhouse/construction/shelter` box (to be described elsewhere). 

The screens are controlled by **setpoints**, which are used by **controllers** to compute the desired state, which again is communicated to the **actuators**:

![](../media/models/vg/plugins-vg-screens-2.png)

Setpoints may differ between different periods of the year and times of the day, as set by a sequence of `DateTimeSignal` boxes. In this example, the threshold for the `energy1` controller has been set to 5 W/m^2^ (screens will be drawn *below* this sunlight intensity), and the threshold for the `shade1` controller has been set to 500 W/m^2^ (screens will be drawn *above* this sunlight intensity). Let's say the sun is low enough that the `energy1` controller computes a screen state of 0.7 (70% drawn) and the `shade1` controller, a state of 0. The `layer1` *controller* gets information from the `layer1` *setpoint* about which screen controllers it should obey. This information comes as a code (`101000`), which the `layer1` controller decodes as `energy1` and `shade1`. Subsequently, it collects the setting computed by each of these controllers. It chooses the maximum value, which in the end is grabbed by the `layer1` *actuator*, which commands all the screens in layer 1.

### Screen setpoints

This example shows how the `energyThreshold1` setpoint could be defined:

```
Setpoints setpoints {
  Box elementary {
    Box screens {
      PrioritySignal energyThreshold1 {
        .reverseOrder = TRUE
        DateTimeSignal {
          .beginDate = 01/01
          .endDate   = 31/12
          .signalInside = 5
        }
        DateTimeSignal {
          .beginDate = 01/12
          .endDate   = 31/03
          .signalInside = 10
        }
      }
    }
  }
}
```

The `energyThreshold1` box is a `PrioritySignal`, which means that the signal (i.e. the `value` of the setpoint) will be chosen among its children. In this case, it will go through the children from the bottom up (because `reverseOrder` is `TRUE`) . It will pick the first child it meets that matches the current date and time.  Thus the second child will be chosen between 1 December and 31 March (giving a threshold of 10 W/m^2^), otherwise the first child will be chosen (giving a threshold of 5 W/m^2^). In this case, `beginTime` and `endTime` have not been set. They default to midnight (`00:00`), which means any time of the day. 

### Layer setpoints

The setpoints for layers are unusual in that these setpoints are not numbers but rather contain information, such as `"shade1+energy1"`. For example: 

```
Setpoints setpoints {
  Box elementary {
    Box screens {
      Box layer1 {
        PrioritySignal setpoint {
          .reverseOrder = TRUE
          DateTimeSignal {
            .beginDate = 01/01
            .endDate   = 31/12
            .signalInside = ./code[value]
            ScreenCombinationEncode code {
              .formula = "shade1+energy1" // a combination of two screen controllers
            }
          }
          DateTimeSignal {
            .beginDate = 01/06
            .endDate   = 31/08
            .signalInside = ./code[value]
            ScreenCombinationEncode code {
              .formula = "shade1" // a single screen controller
            }
          }
        }
      }
      Box layer2 {
        PrioritySignal setpoint {
	      ...
	    }
      }
      Box layer3 {
        PrioritySignal setpoint {
	      ...
	    }
      }
    }
  }
}
```

The formula for a screen controller combination is simply the names of the involved screen controllers, separated by a plus, e.g. `"shade1+energy1"`. It is a string value, which needs to be encoded as a number to serve as a setpoint. The precise rule for the coding is not important but in this example it would be `101000`. The `ScreenCombinationEncode` box takes a `formula` as an input and produces a `value` with the encoded information (e.g. `101000`), which is grabbed by the `DateTimeSignal` box as `./code[value]`,

Thus the second `DateTimeSignal` box will be chosen between 1 June and 31 August, otherwise the first child will be chosen. In effect, the controller of the first layer (i.e., the outermost layer) will obey the controller of the `shade1` screen during summer and of the controllers for the `shade1` and `energy1` screens for the rest of the year. 

### Screen controllers

The controllers for the two **energy screens** are defined by one setpoint each (`screenEnergyThreshold1` and `screenEnergyThreshold2`) and a common proportional band `screenEnergyThresholdBand`. This logic is set up by the `Controllers` box. Here is how it looks for `energy1`:

```
Controllers controllers {
  Box screens {
  	ProportionalSignal energy1 {
  	  .increasingSignal = FALSE
  	  .maxSignal = setpoints[maxScreen]
  	  .input = outdoors[radiation]
  	  .threshold = setpoints[screenEnergyThreshold1]
  	  .thresholdBand = setpoints[screenEnergyThresholdBand]
  	}
  	...
  }
}
```

This means that the signal (i.e., the position of the screen between 0 and 1) will *decrease* when the sunlight is above the threshold (`increasingSignal` is `FALSE`).

The controllers for the two **shade screens** are defined in a similar fashion by the setpoints  `screenShadeThreshold1` , `screenShadeThreshold2` and `screenShadeThresholdBand`, except that the logic works in the opposite direction:

```
Controllers controllers {
  Box screens {
  	ProportionalSignal shade1 {
  	  .increasingSignal = TRUE
  	  .maxSignal = setpoints[maxScreen]
  	  .input = outdoors[radiation]
  	  .threshold = setpoints[screenEnergyThreshold1]
  	  .thresholdBand = setpoints[screenEnergyThresholdBand]
  	}
  }
}
```

This means that the signal  will *increase* when the sunlight is above the threshold (`increasingSignal` is `TRUE`).

The controllers for the two **fixed screens** just have a fixed value each `screenFixed1` and `screenFixed2`, which demands no additional control logic:

```
Controllers controllers {
  Box screens {
  	Box fixed1 {
  	  &value = setpoints[screenFixed1]
  	}
  }
}
```

### Layer controllers

The controllers for the three screen layers each receives a setpoint as a code:

```
Controllers controllers {
  Box screens {
    ScreenCombination layer1 {
      .code = setpoints/elementary/screens/layer1[code]
    }
    ScreenCombination layer2 {
      .code = setpoints/elementary/screens/layer2[code]
    }
    ScreenCombination layer3 {
      .code = setpoints/elementary/screens/layer3[code]
    }
  }
}
```

The `code` (e.g., `101000`) resolves into one or more screen controllers, e.g. `controllers/screens/shade1` and `controllers/screens/energy1`, which each have a current setpoint value. The `ScreenCombination` box resolves these different setpoints by taking their **maximum value**.  You could say that the screen with the largest desire to be drawn always wins,. 

### Actuators

The setpoint for each screen layer (available as e.g. `controllers/screens/layer1[value]`) is passed on to the actuator for that layer, which governs the physical state (beween 0 and 1) of the screens in that layer (i.e. all the screens in the same layer always have the same desired state).  This behaviour is set up by the `Actuators` box. It looks like this for the first layer:

```
Actuators actuators {
  Box screens {
    Accumulator layer1 {
      .change = ./pid[controlVariable]
      .minValue = 0
      .maxValue = setpoints[maxScreen]
      PidController pid {
        .desiredValue = controllers/screens/layer1[value]
        .sensedValue = ..[value]
        .Kprop = 0.05
      }
    }
    Accumulator layer2 {
      ...
    }
    Accumulator layer3 {
      ...
    }
  }
}

```

The actuator uses [PID regulation](https://en.wikipedia.org/wiki/PID_controller) to mimic the gradual drawing of screens towards the current setpoint. The `pid` box gets the setpoint from the layer controller found in `controllers/screens/layer1[value]`. This `desiredValue` is held up againt the `sensedValue`, which is the current state (between 0 and 1) of `layer1` gotten from the parent box (`layer1`) by `..[value]`.  A `PidController` box offers opportunities for the full register of PID control logic but here only the proportional logic (`Kprop`) is used. The value of 0.05 was found by experimentation to be a robust solution. The `pid` box outputs `controlVariable`, which tells by how much the controlled variable should  change.

The `layer1` box maintains a variable `value`, which holds the current state (between 0 and 1) of the screens in the layer. It starts out at a value of zero and is from then on changed according to the output of the `pid` box (brought in through the `change` port as `./pid[controlVariable]`).

### Screen crack

The possible range of values for the state of a screen layer is between 0 and 1. However, the screens must obey the setpoint for **crack ventilation**, which  is defined to avoid blocking the vents at high humidity or high temperature. This is a complication which we neglected until now. Crack ventilation enters as `setpoints[maxScreen]` in the BoxScript for [screen controllers](#screen-controllers). The logic is set up by the `Setpoints` box:

```
Setpoints setpoints {
  .maxScreen = 1 - max(./cracks/*/[signal])
  Box cracks {
    ProportionalSignal atHighRh {
      .input = indoors/humidity[rh]
      .threshold = ../..[rhMax]
      .thresholdBand = ../..[rhMaxBand]
      .increasingSignal = TRUE
      .minSignal = 0
      .maxSignal = ../..[screenCrackAtHighRh]
    }
    ProportionalSignal atHighTemperature {
      .input = indoors/temperature[value]
      .threshold = ../../ventilationTemperature[value]
      .thresholdBand = ../..[screenCrackAtHighTemperatureBand]
      .increasingSignal = TRUE
      .minSignal = 0
      .maxSignal = ../..[screenCrackAtHighTemperature]
    }
  }
}
```

Here, the maximum allowed screen opening `maxScreen` is found as 1 minus the maximum crack value set in response to the current humidity and temperature (i.e. among its grandchildren `./cracks/*/[signal]`). As you can see from the BoxScript code, quite a lot of additional setpoints are involved in this logic.

# The aphid plugin

## Introduction

The `aphid` plugin contains the model building blocks needed to run the [cereal aphid-fungus model](#cereal-aphid-fungus-model). The functionality of the building blocks ([classes](#classes)) is described below

You can run the boxscripts shown in the sub-sections below yourself. For that purpose you must [install Universal Simulator](#software-installation). For example, to run the  `aphid-juvenile-survival.box` script, you write at the Universal Simulator prompt:

```
run models/aphid/aphid-juvenile-survival.box
```

Paste the clipboard at the R prompt to see the output as described in [Running simulations](#running-simulations).

## AphidImmigration

### Interface

| Inputs                | Type       | Default                    | Description                                       |
| --------------------- | ---------- | -------------------------- | ------------------------------------------------- |
| cropGrowthStage       | double     | 0.0 [Zadoks]               | Current crop growth stage                         |
| fromCropGrowthStage   | double     | 31.0 [Zadoks]              | Immigration begins as this growth stage           |
| fromCropGrowthStage   | double     | 80.0 [Zadoks]              | Immigration ends as this growth stage             |
| immigrationRate       | double     | 0.0 [per tiller d&minus;1] | Immigration rate in the season                    |
| propExposedImmigrants | double     | 0.0 [0;1]                  | Proportion of infected immigrants                 |
| k                     | int        | 0 [positive int]           | Has to match k of the receiving StageAndPhase box |
| **Outputs**           |            |                            |                                                   |
| total                 | double     | [per tiller d&minus;1]     | Total immigration rate                            |
| susceptible           | double     | [per tiller d&minus;1]     | Immigration rate of susceptible aphids            |
| exposed               | vec_double | [per tiller d&minus;1]     | Immigration rate of exposed aphids                |

### Usage

The `AphidImmigration` box computes the daily immigration rate of susceptible and exposed aphids (i.e., aphids uninfected or infected by the fungus).

Immigration will be ongoing when `cropGrowthStage` is between `fromCropGrowthStage` and `toCropGrowthStage` at a rate of `immigrationRate`. The proportion `propExposedImmigrants` will be considered exposed.

Since the exposed aphids have an age structure reflecting the age of the infection, the `exposed` output from `AphidImmigration` needs to be a vector, while the `susceptible` output is a simple number. The length of the `exposed` vector is set by the `k` input.

This  boxscript contains a [CropGrowthStage](#cropgrowthstage) model to drive the `AphidImmigration` model. The default `fromCropGrowthStage`=31 is maintained while `fromCropGrowthStage` is set to 69:

```
// aphid-immigration.box
Simulation sim {

  Calendar calendar {
    .begin = 01/04/2004
    .end   = 31/08/2004
  }

  Records weather {
    .fileName =  "weather/Aarnes_2004.txt"
  } 

  Box wheat{
    CropGrowthStage cropGrowthStage {
      .valueAtStart       = 20.0
      .dayDegreesHalfways = 800.0
      .slope              = 2.8
      .dayDegrees = ./time[total]
      CropIsGrowing isGrowing {
        .temperature = weather[Tavg] 
      }
      DayDegrees time{
        .T = weather[Tavg]
        .T0 = 0
        .isTicking = ../isGrowing[value]
      }
    }
  }
  
  Box aphid {
    AphidImmigration immigration {
      .cropGrowthStage = cropGrowthStage[value]
      .toCropGrowthStage = 69
      .immigrationRate = 0.02
      .k = 15
    }
  }
  
  OutputR {
    PageR {
      .xAxis = calendar[date]
      &exposed = sum(aphid/immigration[exposed])
      PlotR {
        .ports = wheat/cropGrowthStage[value] | aphid/immigration[susceptible] | ..[exposed]
      }
    }
  }
}
```

Run the boxscript with

```
run models/aphid/aphid-immigration.box
```

and the output will show the expected pattern, keeping the immigration of exposed aphids at zero, since `propExposedImmigrants` kept its default value of zero:

<img src="../media/models/aphid/aphid-immigration.png" style="zoom:80%;" />

## AphidIndex

### Interface

| Inputs      | Type   | Default          | Description                       |
| ----------- | ------ | ---------------- | --------------------------------- |
| nymphs      | double | 0.0 [per tiller] | Aphid nymph density $(N_{nymph})$ |
| adults      | double | 0.0 [per tiller] | Aphid adult density $(N_{adult})$ |
| **Outputs** |        |                  |                                   |
| value       | double | []               | Index value $(y)$                 |

### Background

The `AphidIndex` expresses the severity of the aphid attack by the index defined by [Wratten et al. 1979](https://www.cabdirect.org/cabdirect/abstract/19800574072),
$$
y=log_{10}(\,0.5*N_{nymph}+N_{adult}+0.01\,)
$$
It is the logarithm of aphid density with nymphs counting half of the adults. The index is -2 at zero aphid density. It is used as an input to calculate [Yield](#yield).

### Usage

`AphidIndex` is used in the [Crop-aphid-fungus](#crop-aphid-fungus-model) model.

## AphidJuvenileSurvival

### Interface

| Inputs          | Type   | Default         | Description                     |
| --------------- | ------ | --------------- | ------------------------------- |
| cropGrowthStage | double | 0.0 [Zadoks]    | Crop growth stage $(G)$         |
| temperature     | double | 0.0 [oC]        | Daily average temperature $(T)$ |
| **Outputs**     |        |                 |                                 |
| value           | double | [0;1] d&minus;1 | Juvenile survival $(y)$         |

### Background

The `AphidJuvenileSurvival` model computes the daily survival rate of nymphs from the inputs `cropGrowthStage` and `temperature`:
$$
y=\left\{
\begin{matrix}
0.944-3.32\cdot10^{-10}\,exp(0.726\cdot T) &\text{for}&G<73\\
0.45&\text{for}&73\le G<80 \\
0&\text{for}&80\le G
\end{matrix}
\right.
$$
The model parameters were estimated by [Duffy et al. (2017)](https://doi.org/10.1016/j.ecolmodel.2017.03.011).

### Usage

You use `AphidJuvenileSurvival` as a building block in an aphid population dynamics model. This boxscript runs through temperature from 15 to 35℃ and computes the survival at growth stage 60:

```
// aphid-juvenile-survival.box
Simulation sim {
  .steps = temperature[steps]
  SequenceByStep temperature {
    .min = 15
    .max = 35
    .by = 0.5
  }
  AphidJuvenileSurvival survival {
    .cropGrowthStage = 60
    .temperature = temperature[value]
  }
  OutputR {
    PageR {
      .xAxis = temperature[value]
      PlotR {
        .ports = survival[value]
      }
    }
  }
}
```

Run the boxscript with

```
run models/aphid/aphid-juvenile-survival.box
```

and you will get the output:

![](../media/models/aphid/aphid-juvenile-survival.png)

## AphidNetReproduction

### Interface

| Inputs                    | Type   | Default           | Description                                                  |
| ------------------------- | ------ | ----------------- | ------------------------------------------------------------ |
| R0opt                     | double | 51.6 [per capita] | Optimum net reproduction $(R_0^{opt})$                       |
| Tmin                      | double | 3.0 [℃]           | Minimum temperature under which no reproduction occur $(T_{min})$ |
| Tmax                      | double | 30.0 [℃]          | Maximum temperature over which no reproduction occur anymore $(T_{max})$ |
| Topt                      | double | 16.1 [℃]          | Optimum temperature for reproduction  $(T_{opt})$            |
| temperature               | double | 0.0 [℃]           | Daily average temperature  $(T)$                             |
| cropGrowthStage           | double | 0.0 [Zadoks]      | Crop growth stage  $(G)$                                     |
| optimumCropGrowthStageMin | double | 0.0 [Zadoks]      | The crop is optimal for reproduction from this growth stage $(G_{min})$ |
| optimumCropGrowthStageMax | double | 0.0 [Zadoks]      | The crop is optimal for reproduction until this growth stage $(G_{max})$ |
| optimumCropFactor         | double | 0.0 [unitless]    | Fecundity increased by this factor when crop is optimal $(c_{crop})$ |
| alateFactor               | double | 0.0 [unitless]    | Factor to correct alate relative to apterous fecundity $(c_{alate})$ |
| aphidDensity              | double | 0.0 [per tiller]  | Aphid density $(N)$                                          |
| aphidDensityThreshold     | double | 0.0 [per tiller]  | Density threshold when net reproduction is zero $N_{max}$    |
| immunityCost              | double | 0.0 [0;1]         | Relative reduction in reproduction when exposed $(\nu)$      |
| **Outputs**               |        |                   |                                                              |
| apterous                  | double | [per capita]      | Net reproduction for susceptible apterous aphids $(R_0^{aptS})$ |
| alate                     | double | [per capita]      | Net reproduction for susceptible alate aphids $(R_0^{alaS})$ |
| apterousExposed           | double | [per capita]      | Net reproduction for infected apterous aphids  $(R_0^{aptE})$ |
| alateExposed              | double | [per capita]      | Net reproduction for infected alate aphids  $(R_0^{alaE})$   |

### Background

Aphid fecundity depends on temperature, crop growth stage and aphid density. The `AphidNetReproduction` model calculates life time fecundity $(R_0$) for a female according to morph (apterous or alate) and infection status (susceptible or exposed).

The temperature response is modelled as a bi-linear function,
$$
f(T) = \left\{
\begin{matrix}
\frac{R_0^{opt}}{T_{opt}-T_{min}}T - \frac{R_0^{opt}\,T_{min}}{T_{opt}-T_{min}} &\text{for}&T_{min}\le T<T_{opt}\\
\frac{R_0^{opt}}{T_{opt}-T_{max}}T - \frac{R_0^{opt}\,T_{max}}{T_{opt}-T_{max}}&\text{for}&T_{opt}\le T<T_{max} \\
0&\text{for}&T<T_{min} \or T_{max}\le T
\end{matrix}
\right.
$$

Density-dependence is simply linear up until the threshold,
$$
g(N)=\left\{ 
\begin{matrix}
1-N/N_{max}&\text{for}&N<N_{max} \\
0&\text{for}&N\ge N_{max}
\end{matrix}
\right.
$$
The effect of crop growth stage is to increase fecundity with the optimal growth stage range:
$$
h(G)=\left\{ 
\begin{matrix}
1&\text{for}&G<G_{min}\\
c_{crop}&\text{for}&G_{min}\le G<G_{max} \\
1&\text{for}&G_{max}\le G<80 \\
0&\text{for}&G\ge 80
\end{matrix}
\right.
$$
To achieve $R_0$ for the four combinations of mother morph and infection status, the effects of temperature ($f(T)$), density ($g(N)$) and crop ($h(G)$) were multiplied together with the effect of morph ($c_{alate}$) and immunity cost ($\nu$):
$$
\begin{align*} 
R_0^{aptS} &= f(T)\,g(N)\,h(G) \\
R_0^{alaS} &= f(T)\,g(N)\,h(G)\,c_{alate} \\
R_0^{aptE} &= f(T)\,g(N)\,h(G)\,(1-\nu) \\
R_0^{alaE} &= f(T)\,g(N)\,h(G)\,c_{alate}\,(1-\nu)\end{align*}
$$
This boxscript demonstrates the functionality of `AphidNetReproduction` at different temperatures:

```
// aphid-net-reproduction.box
Simulation sim {
  .steps = temperature[steps]
  SequenceByStep temperature {
    .min = 0
    .max = 35
    .by = 1
  } 
  AphidNetReproduction reproduction {
    .temperature = temperature[value]
    .Tmin = 3 
    .Topt = 15
    .Tmax = 30
    .R0opt = 50
    .cropGrowthStage = 60
    .optimumCropGrowthStageFrom = 59
    .optimumCropGrowthStageTo   = 73
    .optimumCropFactor = 1.6
    .aphidDensity = 10
    .aphidDensityThreshold = 40 
    .alateFactor = 0.67
    .immunityCost = 0.2
  }
  OutputR {
    PageR {
      .xAxis = temperature[value]
      PlotR {
        .ports = reproduction[output::*]
        .layout = "merged"
      }
    }
  }
}
```

Run the boxscript with

```
run models/aphid/net-reproduction.box
```

and you will get the output:

<img src="../media/models/aphid/aphid-net-reproduction.png" style="zoom:60%;" />

We can check the calculation at optimum temperature with the parameter values set in the boxscript:
$$
\begin{align*} 
R_0^{apt} &= 50\cdot(1-10/40)\cdot1.6=60\\
R_0^{ala} &= 50\cdot(1-10/40)\cdot1.6\cdot0.67=40.2\\
R_0^{aptE} &= 50\cdot(1-10/40)\cdot1.6\cdot(1-0.2)=48\\
R_0^{alaE} &= 50\cdot(1-10/40)\cdot1.6\cdot0.6\cdot(1-0.2)=32.16
\end{align*}
$$

### Usage

`AphidNetReproduction` is used in the [crop-aphid model](#crop-aphid-model).

## AphidOffspring

### Interface

| Inputs          | Type   | Default          | Description                                                  |
| --------------- | ------ | ---------------- | ------------------------------------------------------------ |
| offspringTotal  | double | 0.0 [per tiller] | Total no. of offspring produced by susceptible aphids $(n_{total})$ |
| aphidDensity    | double | 0.0 [per tiller] | Aphid density $(N)$                                          |
| cropGrowthStage | double | 0.0 [Zadoks]     | Crop growth stage $(G)$                                      |
| **Outputs**     |        |                  |                                                              |
| apterous        | double | [per tiller]     | Total no. of apterous offspring produced $(n_{apt})$         |
| alate           | double | [per tiller]     | Total no. of alate offspring produced $(n_{ala})$            |
| alateProportion | double | [0;1]            | Proportion of alate offspring $(p_{ala})$                    |

### Background

The `AphidOffspring` model splits `offspringTotal` into `apterous` and `alate` (strictly, 'alatiform') offspring in response to `aphidDensity` and `cropGrowthStage` based on the empirical relation [Watt and Dixon, 1981](https://doi.org/10.1111/j.1365-2311.1981.tb00636.x):
$$
\begin{align*} 
p_{ala} &= \left[ (2.603N + 0.847G - 27.189)/100 \right]_0^1\\
n_{apt} &= (1-p_{ala})\,n_{total} \\
n_{ala} &= p_{ala}\,n_{total} 
\end{align*}
$$
where $[\ldots]_0^1$ denotes that the expression is limited to the closed interval $[0;1]$.

### Usage

`AphidOffspring` is used in the [crop-aphid model](#crop-aphid-model).

## Biocontrol

### Interface

| Inputs                  | Type   | Default          | Description                                              |
| ----------------------- | ------ | ---------------- | -------------------------------------------------------- |
| aphidPressureWithoutF   | double | 0.0 [aphid days] | Accumulated aphid pressure without fungus                |
| aphidPressureWithF      | double | 0.0 [aphid days] | Accumulated aphid pressure with fungus                   |
| yieldWithoutF           | double | 0.0 [0;1]        | Relative yield without fungus                            |
| yieldWithF              | double | 0.0 [0;1]        | Relative yield witt fungus                               |
| cropGrowthStage         | double | 0.0 [Zadoks]     | Current crop growth stage                                |
| prevalence              | double | 0.0 [%]          | Prevalence of exposed aphids                             |
| cadaverPrevalence       | double | 0.0 [%]          | Prevalence of cadavers                                   |
| **Outputs**             |        |                  |                                                          |
| aphidPressureDifference | double | [aphid days]     | Difference in aphid pressure caused by fungus            |
| yieldImprovement        | double | [%-points]       | Improvement in yield when controlled                     |
| percentageCadaversGs43  | double | [%]              | Percentage cadavers at GS 43                             |
| percentageCadaversGs61  | double | [%]              | Percentage cadavers at GS 61                             |
| percentageCadaversGs73  | double | [%]              | Percentage cadavers at GS 73                             |
| maxCadaverPrevalence    | double | [%]              | Peak cadaver prevalence before GS80                      |
| maxCadaverPrevalenceGS  | double | [Zadoks]         | Crop growth stage at peak cadaver prevalence before GS80 |
| maxPrevalence           | double | [%]              | Peak prevalence before GS80                              |
| maxPrevalenceGS         | double | [Zadoks]         | Crop growth stage at peak prevalence before GS80         |

### Background

The `Biocontrol` box summarises the effects of the fungus in terms of aphid pressure, yield and the prevalence of cadavers and exposed aphids.

### Usage

`Biocontrol` is used in the [biocontrol](#biocontrol-model-with-uncertainty-analysis) model.

## CadaverConversion

### Interface

| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|succumbedApterousNymphs|double|    0.0 [per tiller]|New apterous nymph cadavers|
|succumbedAlateNymphs   |double|    0.0 [per tiller]|New alate nymph cadavers|
|succumbedApterousAdults|double|    0.0 [per tiller]|New apterous adult cadavers|
|succumbedAlateAdults   |double|    0.0 [per tiller]|New alate adult cadavers|
| **Outputs** | | | |
|cadavers               |double|        [per tiller]|Cadavers in standardised units|
|count                  |double|        [per tiller]|Number of cadavers|

### Background 

The inputs are newly (i.e, during the latest time step) produced cadavers of different life stages and morphs. The outputs are the total number of cadavers $(C_n)$ and the total number, standardised to adult cadavers $(C_{std})$:
$$
\begin{align*} 
C_{n} &= N_{apt} + N_{ala} + A_{ala} + A_{apt} \\
C_{std} &= 0.5\left( N_{apt} + N_{ala} \right) + 0.8 A_{ala} + A_{apt}
\end{align*}
$$
The rationale behind $C_{std}$ is that alates produce less (20%; cf. [Hemmati et al., 2001](https://doi.org/10.1017/S0953756201004014)) spores than apterae and that nymphs produce fewer (50%; guessed) spores than adults.

### Usage

`CadaverConversion` is used in the [crop-aphid-fungus](#crop-aphid-fungus-model) model.

## CadaverTime

### Interface

| Inputs         | Type   | Default         | Description                                     |
| -------------- | ------ | --------------- | ----------------------------------------------- |
| isSporulating  | bool   | FALSE [boolean] | Are cadavers sporulating?                       |
| timeStep       | double | 0.0 [DD]        | Time step $(\tau)$                              |
| rhAccelaration | double | 0.0 [&minus;]   | Acceleration factor above RH threshold $(h)$    |
| **Outputs**    |        |                 |                                                 |
| step           | double | [DD]            | RH-corrected time step  $({\tau}_{corrected})$  |
| total          | double | [DD]            | Accumulated RH-corrected time steps since reset |

### Background 

Cadavers degrade on a day-degree scale. When they are sporulating this degradation is accelerated by a factor `rhAccelaration`. The `CadaverTime` box simple multiplies `timeStep` with `rhAccelaration` if `isSporulating` is true:
$$
{\tau}_{corrected} = 
\left\{ 
\begin{matrix}
h\tau \,\,&\text{if is sporulating} \\
\tau \,\,&\text{otherwise}
\end{matrix}
\right.
$$

### Usage

`CadaverTime` is used in the [crop-aphid-fungus](#crop-aphid-fungus-model) model.

## CropGrowthStage

### Interface

| Inputs             | Type   | Default       | Description                                                |
| ------------------ | ------ | ------------- | ---------------------------------------------------------- |
| valueAtStart       | double | 20.0 [Zadoks] | Growth stage at the beginning of the growth season $(G_0)$ |
| dayDegrees         | double | 0.0 [DD]      | Day-degrees passed since growth season started $(\tau)$    |
| dayDegreesHalfways | double | 720.0 [DD]    | Time when development is half completed $(\tau_{50})$      |
| slope              | double | 2.8 [DD-1]    | Max. development rate $(g)$                                |
| **Outputs**        |        |               |                                                            |
| value              | double | [Zadoks]      | Crop growth stage $(G)$                                    |

### Background

The `CropGrowthStage` is an empirical model driven by day-degrees and fitted to Norwegian data. It follows the sigmoid curve of the log-logistic function 
$$
G = G_0 + \frac{G_{max}-G_0}{1+exp\{1+g[\,ln(\tau)-ln(\tau_{50})]\,\}}
$$
with $G_{max}=99$.

### Usage

The usage of `CropGrowthStage` is demonstrated in [AphidImmigration](#aphidimmigration).

## CropIsGrowing

### Interface

| Inputs      | Type   | Default       | Description                         |
| ----------- | ------ | ------------- | ----------------------------------- |
| temperature | double | 0.0 [&#8451;] | Daily average temperature           |
| T0          | double | 5.0 [&#8451;] | Threshold that triggers crop growth |
| **Outputs** |        |               |                                     |
| value       | bool   | []            | Is the crop currently growing?      |

### Background 

The crop goes out of hibernation to continue growth and development, when experiencing five consecutive days with `temperature` above the threshold `T0`. The output `value` is false at the beginning of the simulation and remains true onwards, once the condition has been fulfilled.

### Usage

`CropIsGrowing` is used in the [crop](#weather-driven) model.

## InfectionRate

### Interface

| Inputs                 | Type   | Default                   | Description                                                  |
| ---------------------- | ------ | ------------------------- | ------------------------------------------------------------ |
| isSporulating          | bool   | FALSE [boolean]           | Are cadavers sporulating?                                    |
| cadavers               | double | 0.0 [per tiller]          | Sporulating cadavers $(C)$                                   |
| transmissionEfficiency | double | 0.0 [per cadaver per day] | Transmission efficiency $(\epsilon)$                         |
| **Outputs**            |        |                           |                                                              |
| value                  | double | [0;1]                     | Proportion of hosts getting exposed in one day $(\epsilon_{finite})$ |

### Background 

Infection spreads at the instantaneous rate $\epsilon$ per day. This is transformed into a finite rate per day applying the functional response of [Nicholson and Bailey 1935](https://zslpublications.onlinelibrary.wiley.com/doi/10.1111/j.1096-3642.1935.tb01680.x):
$$
{\epsilon}_{finite} = 
\left\{ 
\begin{matrix}
1-exp\left(-\epsilon C\right) \,\,&\text{if is sporulating} \\
0 \,\,&\text{otherwise}
\end{matrix}
\right.
$$

### Usage

`InfectionRate` is used in the [crop-aphid-fungus](#crop-aphid-fungus-model) model.

## Prevalence 

### Interface

| Inputs         | Type   | Default          | Description                                      |
| -------------- | ------ | ---------------- | ------------------------------------------------ |
| aphidDensity   | double | 0.0 [per tiller] | Current total density of live aphids             |
| exposedDensity | double | 0.0 [per tiller] | Current density of exposed aphids                |
| cadaverDensity | double | 0.0 [per tiller] | Current density of cadavers                      |
| **Outputs**    |        |                  |                                                  |
| exposed        | double | [%]              | Percentage exposed aphids out of all live aphids |
| cadavers       | double | [%]              | Percentage cadavers out of aphids+cadavers       |

### Background 

The `Prevalence` box calculates the current percentages of `exposed` aphids and of `cadavers`.

### Usage

`Prevalence` is used in the [crop-aphid-fungus](#crop-aphid-fungus-model) model.

## Yield

### Interface

| Inputs          | Type   | Default      | Description                                          |
| --------------- | ------ | ------------ | ---------------------------------------------------- |
| cropGrowthStage | double | 0.0 [Zadoks] | Crop growth stage $(G)$                              |
| aphidIndex      | double | 0.0 []       | Aphid density index of Entwistle-Dixon-Wratten ($I$) |
| **Outputs**     |        |              |                                                      |
| yield           | double | [0;1]        | Yield relative to uninfested crop $(Y)$              |
| loss            | double | [0;1]        | Proportional yield loss due to aphids $(Y_L)$        |
| lossRate        | double | [0;1]        | Daily loss rate $(y)$                                |

### Background 

The `Yield` class implements the yield loss model of [Entwistle and Dixon 1987](https://onlinelibrary.wiley.com/doi/10.1111/j.1744-7348.1987.tb02007.x), which takes `cropGrowthStage` (from [CropGrowthStage](#cropgrowthstage)) and `aphidIndex` (from [AphidIndex](#aphidindex)) as inputs. The crop stage is used in a lookup table to find the coefficient $E$:

|$G$ interval|$E$|
|---|-----|
|[0; 53)|0.075|
|[53; 69)|0.205|
|[69; 71)|0.075|
|[71; 73)|0.056|
|[73; 77)|0.037|
|[77;99]|0.012|

The daily loss rate $r \in [0;1]$ is found by multiplying the aphid index $A$ by $E$:
$$
r=
\left\{ 
\begin{matrix}
AE/100 &\text{for}&A>0 \\
0 &\text{for}&A\le 0
\end{matrix}
\right.
$$
The `Yield` object keeps track of the accumulated loss by updating the relative yield $Y \in [0;1]$ and loss $Y_L \in [0;1]$ in every time step (1 day):
$$
\begin{align*} 
Y &\gets (1-r)Y \\
Y_L &= 1 - Y
\end{align*}
$$

### Usage

`Yield` is used in the [crop-aphid-fungus](#crop-aphid-fungus-model) model.

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

# The saccharina plugin

## PlantReset

### Interface

| Inputs        | Type   | Default               | Description                                        |
| ------------- | ------ | --------------------- | -------------------------------------------------- |
| biomassInit   | double | 0.1 [g dw/m]          | Initial total biomass                              |
| concCInit     | double | 0.3 [g C/g dw]        | Initial total biomass carbon concentration         |
| concNInit     | double | 0.02 [g N/g dw]       | Initial total biomass nitrogen concentration       |
| propCStruct   | double | *computed* [g C/g dw] | *Proportion of carbon in the structural biomass*   |
| propNStruct   | double | *computed* [g N/g dw] | *Proportion of nitrogen in the structural biomass* |
| propCRes      | double | *computed* [g C/g dw] | *Proportion of carbon in the carbon reserve*       |
| propNRes      | double | *computed* []         | *Proportion of nitrogen in the nitrogen reserve*   |
| **Outputs**   |        |                       |                                                    |
| biomassStruct | double | [g dw/m]              | Initial structural biomass                         |
| biomassCRes   | double | [g dw/m]              | Initial carbon reserves biomass                    |
| biomassNRes   | double | [g dw/m]              | Initial nitrogen reserves biomass                  |

### Background

The `PlantReset` box holds the initial values for the state variables of the saccharina model. It converts the primary state variables (its inputs) to biomass terms, which it provides as outputs:

==Under construction, 26 Sep 2022==

### Usage

The `plantReset` box is used by the `plant` and `biomassStruct` boxes, as you can convince yourself by asking for its exports with the [list](#list) command:

```
> run models/saccharina/saccharina.box
...
> list plantReset x
PlantReset plantReset
  >biomassStruct = 4.80276 g dw/m
    >> sim/plant[initBiomassStruct]
    >> sim/biomassStruct[initial]
  >biomassCRes = 1.68387 g dw/m
    >> sim/plant[initBiomassCRes]
  >biomassNRes = 0.713364 g dw/m
    >> sim/plant[initBiomassNRes]
```

## Plant

### Interface


| Inputs | Type | Default | Description |
| ------ | ---- | ------- | ----------- |
|optCConc         |double|    0.35 [g C/g dw]|Optimum total biomass carbon concentration $([C]_{opt})$|
|optNConc         |double|    0.05 [g C/g dw]|Optimum total biomass nitrogen concentration $([N]_{opt})$|
|propCStruct      |double|0.21 [g C/g dw]|*= proportions[propCStruct]* (Proportion of carbon in the structural biomass; $\rho_{struct}^C$)|
|propNStruct      |double|0.02 [g N/g dw]|*= proportions[propNStruct]* (Proportion of nitrogen in the structural biomass; $\rho_{struct}^N$))|
|propCRes         |double|0.47 [g C/g dw]|*= proportions[propCRes]* (Proportion of carbon in the carbon reserve; $\rho_{res}^C$)|
|propNRes         |double|0.37 [g N/g dw]        |*= proportions[propNRes]* (Proportion of nitrogen in the nitrogen reserve; $\rho_{res}^N$)|
|initBiomassStruct|double|*computed* [g dw/m]        |*= plantReset[biomassStruct]*|
|initBiomassCRes  |double|*computed* [g dw/m]        |*= plantReset[biomassCRes]*|
|initBiomassNRes  |double|*computed* [g dw/m]        |*= plantReset[biomassNRes]*|
|biomassStruct    |double|*computed* [g dw/m]  |*= biomassStruct[content]* (Structural biomass; $W_{struct}$)|
|supplyReservesC  |double|*computed* []        |*= allocation[supplyReservesC]*|
|supplyReservesN  |double|*computed* []        |*= allocation[supplyReservesN]*|
| **Outputs**       |        |                       |                                                           |
| biomassCRes       | double | [g dw/m]              | Carbon reserves biomass $(W_{res}^C)$                     |
| biomassNRes       | double | [g dw/m]              | Nitrogen reserves biomass $(W_{res}^N)$                   |
| biomassTotal      | double | [g dw/m]              | Total biomass $(W_{total})$                               |
| massCStruct       | double | [g C/m]               | Mass of carbon in the structural biomass $(C_{struct})$   |
| massNStruct       | double | [g N/m]               | Mass of nitrogen in the structural biomass $(N_{struct})$ |
| massCRes          | double | [g C/m]               | Mass of carbon in the carbon reserve $(C_{res})$          |
| massNRes          | double | [g N/m]               | Mass of nitrogen in the carbon reserve  $(N_{res})$       |
| concC             | double | [g C/g dw]            | Total biomass carbon concentration $()[C])$               |
| concN             | double | [g N/g dw]            | Total biomass nitrogen concentration $()[N])$             |

# To-do topics

Small and not-so-small documentation tasks.

## File folders

### Work folder

### Input folder

### Output folder

## BoxScript Developer

* Downloading Source Code

* Installing Qt Creator

* Building Universal Simulator From Source

* Create Your Own Library of Building Block

## BoxScript framework

* Writing your own Box-based classes

## Computational model

* Explain the simulations steps
* Explain the child-first rule

## Plugins

* Explain plugin design
* Existing plugins
* Own plugins

# To-do coding

* Make "/" a valid path, as in `list /`

L
