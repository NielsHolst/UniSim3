// output_selector.typ

#import "../../../doc/lib/helpers.typ": code-with-link

- #link(<output-selector-usage>)[Usage]
- #link(<skip-a-number-of-lines>)[Skip a number of lines]
- #link(<skip-lines-until-a-certain-time>)[Skip lines until a certain time]
- #link(<periodic-output>)[Periodic output]
- #link(<final-output>)[Final output]

=== Usage
<output-selector-usage>
The `OutputSelector` box determines when output should be produced by
the `OutputWriter` box. Hence, if you do not include an `OutputSelector`
box in your boxscript then the `OutputWriter` box will create one for
you. The default behaviour of `OutputSelector` is to write one line of
output for every #emph[reset] and #emph[update] step during the
simulation \(see #highlight[Computationel model]). The
`useLocalDecimalChar` and `skipFormats` define different features of the
output file as explained for #link(<boxes_OutputWriter>)[OutputWriter].

If you want to change the default behaviour of `OutputSelector`, you
must include an `OutputSelector` box in your boxscript. The right place
to put it is as a child box of the `OutputText` \(as exemplified below)
or `OutputR` box, since `OutputSelector` affects the output produced by
`OutputText` and `OutputR`.

=== Skip a number of lines
<skip-a-number-of-lines>
You can set the number `steps` and `iterations` a simulation is running
in the #link(<boxes_Simulation>)[Simulation] object. That will by itself
result in `iterations` \* \(`steps` + 1) lines in the output file
\(adding one for each iteration because a line is written also in the
reset step). You can skip a number of initial lines in each iteration by
setting the `beginStep` of the `OutputSelector` box. Maybe the model
needs some warming up before it produces reliable or relevant output.
Here is one simple example:

#code-with-link(
  "https://ecolmod.org/input/demo/output_selector/output_selector1.box",
  raw("// output_selector1.box"),
  "Simulation sim {
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
}"
)

When you run it

```
> run demo/output_selector/output_selector1.box
```

and subsequently inspect the few lines produced with the
#raw(lang:"command", "head") command, you won\'t be surprised:

```
> head
```

Well, unless you had not anticipated that `beginStep` is acting on every
`iteration`.

=== Skip lines until a certain time
<skip-lines-until-a-certain-time>
As an alternative to `beginStep`, you can set `beginDateTime` to
commence output at a certain date-time. You can combine them both, which
will postpone output until both a certain step and a certain date-time
is reached, though I never found the need to do so. Here is a simple
example, working in 5-minute time steps

#code-with-link(
  "https://ecolmod.org/input/demo/output_selector/output_selector2.box",
  raw("// output_selector2.box"),
  "Simulation sim {
  .iterations = 2
  Calendar calendar {
    .begin = 01/08/2022T12:00
    .end = 01/08/2022T12:40
    .timeStep = 5
    .timeUnit = \"m\"
  }
  OutputText {
    .ports = calendar[dateTime]
    OutputSelector {
      .beginDateTime = 01/08/2022T12:30
    }
  }
}"
)

Run the simulation

```
run demo/output_selector/output_selector2.box
```

and again use #raw(lang:"command", "head") to see the expected output:

```
> head
```

=== Periodic output
<periodic-output>
You can write only every other line by setting `period = 2`, every third
line by `period = 3`, and so forth. This can be combined with
`beginStep` and `beginDateTime` as you wish:

#code-with-link(
  "https://ecolmod.org/input/demo/output_selector/output_selector3.box",
  raw("// output_selector3.box"),
  "Simulation sim {
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
}"
)

Notice that the final output written for each iteration is in step 22,
even though an iteration lasts 25 steps. Note also that the period
counts from the first line written in each iteration \(in step 10), not
from the beginning of the simulation in each iteration \(step 0):

```
> head
```

=== Final output
<final-output>
If you want only the final values to appear in the output, i.e. those in
the last step, then you set `final` to `TRUE`. This will overrule any of
the settings on `beginStep`, `beginDateTime` and `period`. Here is a
simple boxscript:

#code-with-link(
  "https://ecolmod.org/input/demo/output_selector/output_selector4.box",
  raw("// output_selector4.box"),
  "Simulation sim {
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
}"
)

When you run the script,

```
> run demo/output_selector/output_selector4.box
```

and look at the output, you will find that only the final step 25 of
each of the three iterations appears in the output:

```
> head 20
```

If you run an uncertainty or sensitivity analysis, you will usually set
`final` to `TRUE`.
