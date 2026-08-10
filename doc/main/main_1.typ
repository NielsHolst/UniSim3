// main.typ

// ============================================================
// Document configuration
// ============================================================

#set document(
  title: "My Framework Documentation",
  author: "My Project",
)

#set text(
  font: "Garamond",
  size: 10pt,
)

#set heading(numbering: "1.1")

#show link: set text(
  fill: blue,
  weight: "medium",
)

// ============================================================
// Document
// ============================================================

#align(center)[
  #text(size: 24pt, weight: "bold")[
    Universal Simulator
  ]

  #v(1em)

  #text(size: 16pt)[Documentation]

  #v(2em)

  Version 3.0.34
]

#pagebreak()

= Contents

#outline(
  title: "Modules and classes",
  depth: 3,
)

#pagebreak()

// ============================================================
// Modules
// ============================================================

/*
= Module: boxes

#class-api("boxes", "Accumulator") <boxes::Accumulator>
#include "../../src/plugins/boxes/accumulator.typ"
*/

#include "../generated/generated_api.typ"
