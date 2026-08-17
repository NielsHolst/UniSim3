// random_binomial.typ

#import "../../../doc/lib/helpers.typ": code-with-link

=== Usage
`RandomBinomial` produces numbers from the binomial distribution.
`value` will be `TRUE` with probability `P`.

#code-with-link(
  "https://ecolmod.org/input/demo/random_binomial/random_binomial1.box",
  raw("// random_binomial1.box"),
  "Simulation sim {
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
}"
)

Run the boxscript:

```
> run demo/random_binomial/random_binomial1.box
```

The final lines of the output show how `x` approaches 60% and `y`
approaches 90%, as expected:

```
> tail
```
