// main.typ

// ============================================================
// Document configuration
// ============================================================

#set document(
  title: "Universal Simulator Documentation",
  author: "Niels Holst",
)

#set text(
  font: "Garamond",
  size: 10pt,
)

// Outline 
#show outline: it => {
  show heading: set heading(outlined: false)
  show heading: set text(size: 26pt, weight: "bold")
  it
}

// Page with running header and page number
#set page(
  header: context {
    let current-page = counter(page).get().first()

    let headings = query(heading.where(level: 1))

    let on-page = headings.filter(h =>
      h.location().page() == current-page
    )

    let current = if on-page.len() > 0 {
      on-page.first()
    } else {
      let previous = headings.filter(h =>
        h.location().page() < current-page
      )

      if previous.len() > 0 {
        previous.last()
      }
    }

    if current != none {
      set text(
        size: 10pt,
        weight: "regular",
        style: "normal",
      )

      align(center)[
        #current.body
      ]
    }
  },
  footer: context {
    align(right)[
      #counter(page).display()/#counter(page).final().first()
    ]
  },)

#set heading(numbering: "1.1")

// Real level-1 headings: large and start on new page
#show heading.where(level: 1, outlined: true): it => {
  pagebreak(weak: true)

  set text(
    size: 26pt,
    weight: "bold",
  )

  block(
    above: 18pt,
    below: 18pt,
    it,
  )
}

// Level 2
#show heading.where(level: 2): it => {
  set text(size: 18pt, weight: "bold")

  block(
    above: 18pt,
    below: 10pt,
    it,
  )
}

// Level 3
#show heading.where(level: 3): it => {
  set text(size: 13pt, weight: "bold")

  block(
    above: 12pt,
    below: 6pt,
    it,
  )
}

// Links
#show link: set text(
  fill: blue,
  weight: "medium",
)

// ============================================================
// Document
// ============================================================

#align(center)[
  #text(size: 26pt, weight: "bold")[Universal Simulator]

  #v(1em)

  #text(size: 24pt, weight: "bold")[Documentation]

  #v(2em)

  #text(size: 22pt, weight: "bold")[Version 3.0.34]
]

#pagebreak(weak: true)

#outline(
  title: [Modules and classes],
  depth: 3,
)

#pagebreak(weak: true)

// ============================================================
// Modules
// ============================================================

#include "../generated/generated_api.typ"
