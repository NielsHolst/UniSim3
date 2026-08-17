// randomiser_sobol_sequence.typ

#import "../../../doc/lib/helpers.typ": code-with-link
#import "../../../doc/lib/helpers.typ": demo-figure

`RandomiserSobolSequence` is derived from
#link(<randomiserbase>)[RandomiserBase];.

=== Usage
You can use `RandomiserMonteCarlo` for stochastic modelling and
uncertainty analysis, but
#link(<randomiserstratified>)[RandomiserStratified] will explore the
parameter space more efficiently. For sensitivity analysis,
#link(<randomisersobolsequence>)[RandomiserSobolSequence] is the
preferred choice because it is the most effective of the three in
covering all of the parameter space.

The #link("https://ecolmod.org/input/demo/randomiser/randomiser7.box")[randomiser7.box⏷]
script shows the differences, subtle but important, between the three
randomisers. The boxscript breaks the general rule not to put more than
one randomiser box into a script. The only good reason would be for a
direct comparison like here. Each of the three randomisers is the source
of random numbers used to generate random numbers for two variables `x`
and `y`, both with a uniform distribution of numbers in the interval
\[20;40) \(see #link(<randomuniform>)[RandomUniform];).

#code-with-link(
  "https://ecolmod.org/input/demo/randomiser/randomiser7.box",
  raw("// randomiser7.box"),
  " 
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
    .scripts = \"randomiser7.R\"
    OutputSelector {
      .final = TRUE
    }
    OutputText {
      .ports = sim/*/*[value]
    }
  }
}"
)

Run the boxscript,

```
>run demo/randomiser/randomiser7.box
```

and study the output:

#demo-figure(
  "randomiser/randomiser7.png",
  caption: [RandomiserSobolSequence example],
)

The three ransomisers worked like this to generate random numbers:

- `RandomiserMonteCarlo` picked 32 numbers at random from the interval
  \[20;40) for both `x` and `y`.
- `RandomiserStratified` first divided the interval \[20;40\] into 32
  slices \(strata) of equal area for both `x` and `y`. Then it shuffled
  the order of these slices at random for both `x` and `y`. Then it
  picked a random number from within each slice \(stratum).
- `RandomiserSobolSequence` picked the first 32 number from the Sobol\'
  sequence for two variables and assigned them to `x` and `y`.

At the bottom you see the distribution of `x`. The Monte Carlo method
yielded the least uniform and the Sobol\' sequence the most uniform
distribution.

At the top you see the joint distribution, where the Sobol\' sequence
stands out with the best coverage of the 2-dimensional variable space;
this is what Sobol\' designed his sequence for. Moreover, the good
coverage of Sobol\' numbers is still obtained with many variables in
n-dimensional space.

The `randomiser` box is ignorant about the statistical distributions
that are generated from. Behind the scenes it just produces random
numbers in the interval \[0;1\], which are then grabbed by its sibling
random boxes \(in this example #link(<randomuniform>)[RandomUniform];)
and transformed into the appropriate distribution. For another
distribution than uniform, you can check out
#link(<randomnormal>)[RandomNormal];, for example.

Find more examples in #link(<randomiserbase>)[RandomiserBase];.
