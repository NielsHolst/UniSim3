// random_base.typ

#figure([#figure(
  align(center)[#table(
    columns: 4,
    align: (col, row) => (auto,auto,auto,auto,).at(col),
    inset: 6pt,
    [Inputs], [Type], [Default], [Description],
    [P],
    [double],
    [0.95 \[0;1\]],
    [Range of the distribution covered by the \[min;max) range],
    [drawAtInitialize],
    [bool],
    [#emph[computed] \[\]],
    [#emph[ancestors::\*/RandomiserBase::\*\[drawAtInitialize\]];],
    [drawAtReset],
    [bool],
    [#emph[computed] \[\]],
    [#emph[ancestors::\*/RandomiserBase::\*\[drawAtReset\]];],
    [drawAtUpdate],
    [bool],
    [#emph[computed] \[\]],
    [#emph[ancestors::\*/RandomiserBase::\*\[drawAtUpdate\]];],
  )]
  )

  ],
  caption: [
  ]
)

=== Background
You cannot have a `RandomBase` box in your boxscript. It only serves as
a virtual base class for boxes to draw random numbers from various
distributions:

- #link(<randombinomial>)[RandomBinomial]
- #link(<randomlognormal>)[RandomLogNormal]
- #link(<randomloguniform>)[RandomLogUniform]
- #link(<randomnormal>)[RandomNormal]
- #link(<randomuniform>)[RandomUniform]
- #link(<randomuniformint>)[RandomUniformInt]

They all share the interface of `RandomBase`. Of the four inputs, you
should only ever use `P` in your boxscripts. If you want to change when
random numbers are drawn, you should do so in the
#link(<randomiserbase>)[RandomiserBase] box. For this reason the three
inputs \(`drawAtInitialize`, `drawAtReset` and `drawAtUpdate`) will not
be listed among the inputs for the boxes derived from `RandomBase`
below.

=== Usage
You can have as many of these different kinds of #strong[random] boxes
as you want, but you should always keep as siblings inside one common
parent box, together with one #strong[randomiser] box, as described for
#link(<randomiserbase>)[RandomiserBase];.
