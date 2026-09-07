// maker.typ

#import "../../../doc/lib/helpers.typ": code-with-link

- #link(<make_numbered_clones>)[Make numbered clones]
- #link(<make_named_clones>)[Make named clones]
- #link(<make_clones_from_file>)[Make clones from file]

=== Usage
You use a `Maker` box if you want to make several copies \(clones) of a
box in the boxscript. The clones will all be exact copies of the
original box \(albeit with different names), including its port values
and any child boxes, recursively \(i.e. the clones are deep copies).

==== Make numbered clones <make_numbered_clones>
In the first example, we create want to create four box objets
representing islands with populations of foxes and rabbits. The
geolocation of the islands and animal densities should be random. Here
is our first sketch of a model without the use of a `Maker`:
#code-with-link(
  "https://ecolmod.org/input/demo/maker/islands1.box",
  raw("// islands1.box"),
  "Simulation sim {
  .iterations = 4
  .steps = 5
  Box random {
    RandomiserMonteCarlo randomiser {
    }
    RandomUniform latitude {
      .min = 10
      .max = 20
    }
    RandomUniform longitude {
      .min = -40
      .max = -30
    }
    RandomUniformInt initFox {
      .min = 10
      .max = 20
    }
    RandomUniformInt initRabbit {
      .min = 200
      .max = 1000
    }
  }
  Box island {
    &latitude = random/latitude[value]
    &longitude = random/longitude[value]
    Box fox {
      &value = random/initFox[value]
    }
    Box rabbit {
      &value = random/initRabbit[value]
    }
  } 
  OutputText {
    .ports = sim[iteration] | island[*] | island/*[*]
    OutputSelector {
      .final = TRUE
    }
  }
}"
)

With the `OutputSelector`\'s `final` port set to `TRUE`, we get only one
line in the output per iteration, as we can quickly see following
#raw(lang:"command", "run") by #raw(lang:"command", "head");:
```
> run demo/maker/islands1.box
```

This boxscript will allow us to model on each island \(given further
elaborations of the model) #emph[in sequence];. But if we wanted to
model them #emph[simultaneously];, maybe to include migration between
the islands? For that we can use a `Maker` box:
#code-with-link(
  "https://ecolmod.org/input/demo/maker/islands2.box",
  raw("// islands2.box"),
  "Simulation sim {
  .iterations = 4
  .steps = 5
  Box random {
    RandomiserMonteCarlo randomiser {
    }
    RandomUniform latitude {
      .min = 10
      .max = 20
    }
    RandomUniform longitude {
      .min = -40
      .max = -30
    }
    RandomUniformInt initFox {
      .min = 10
      .max = 20
    }
    RandomUniformInt initRabbit {
      .min = 200
      .max = 1000
    }
  }
  Maker archipelago {
    .replicates = 3
    Box isle {
      &latitude = random/latitude[value]
      &longitude = random/longitude[value]
      Box fox {
        &value = random/initFox[value]
      }
      Box rabbit {
        &value = random/initRabbit[value]
      }
    } 
  }
  OutputText {
    .ports = sim[iteration] | archipelago/*[*] | archipelago/*/*[*]
    OutputSelector {
      .final = TRUE
    }
  }
}"
)

Here, the `isle` is used as a template by the `Maker` \(called
`archipelago`) to make three clones \(since `replicates = 3`, which are
simply numbered. The #raw(lang:"command", "list") shows the model
structure:
```
> list
```

If we run the model and look at the output, we are not quite there
though. Take a look at the output \(lines have been broken for easier
reading):
```
> run demo/maker/islands2.box
```

In each of the four iterations, the three isles share their position
\(like being stacked), and they have the same densities of foxes and
rabbits. We don\'t know the exact research questions addresses by this
model but let\'s say that in each iteration, isles should be placed
randomly and with different, random population densities. Then we need
for each isle to draw its own random numbers:
#code-with-link(
  "https://ecolmod.org/input/demo/maker/islands3.box",
  raw("// islands3.box"),
  "Simulation sim {
  .iterations = 4
  .steps = 5
  RandomiserMonteCarlo randomiser {
  }
  Maker archipelago {
    .replicates = 3
    Box isle {
      &latitude = ./latitude[value]
      &longitude = ./longitude[value]
      RandomUniform latitude {
        .min = 10
        .max = 20
      }
      RandomUniform longitude {
        .min = -40
        .max = -30
      }
      Box fox {
        &value = ./init[value]
        RandomUniformInt init {
          .min = 10
          .max = 20
        }
      }
      Box rabbit {
        &value = ./init[value]
        RandomUniformInt init {
          .min = 200
          .max = 1000
        }
      }
    } 
  }
  OutputText {
    .ports = sim[iteration] | archipelago/*[*] | fox[value] | rabbit[value]
    OutputSelector {
      .final = TRUE
    }
  }
}"
)

If we inspect `isle0`, we can see that is equipped with its own random
numbers:
```
> list isle0
```

This is confirmed further by the output of the model. Now the isles are
different in all aspects in every iteration:
```
> head
```

Note the there can only be one randomiser box on a boxscript. All the
random variables in a boxscript draw numbers from this one source.

==== Make named clones <make_named_clones>
To continue the example above, let\'s say that we want to model some
concrete islands with known positions but still with random population
densities. We can achieve this by supplying the `Maker` box with a
vector of `names` in stead of just the number of replicates. In this
example, we include only to islands:
#code-with-link(
  "https://ecolmod.org/input/demo/maker/islands4.box",
  raw("// islands4.box"),
  "Simulation sim {
  .iterations = 4
  .steps = 5
  RandomiserMonteCarlo randomiser {
  }
  Maker azores {
    .names = c(\"Faial\", \"Flores\")
    Box isle {
      &latitude  = if (name(.[]) == \"Faial\") then 38.58 else 39.46
      &longitude = if (name(.[]) == \"Faial\") then 28.70 else 31.13
      Box fox {
        &value = ./init[value]
        RandomUniformInt init {
          .min = 10
          .max = 20
        }
      }
      Box rabbit {
        &value = ./init[value]
        RandomUniformInt init {
          .min = 200
          .max = 1000
        }
      }
    } 
  }
  OutputText {
    .ports = sim[iteration] | archipelago/*[*] | fox[value] | rabbit[value]
    OutputSelector {
      .final = TRUE
    }
  }
}"
)

As you can see, we got the wished for structure of the model:
```
> list
```
==== Make clones from file <make_clones_from_file>
#highlight[To do...]
