#let api = json("../generated/generated_api.json")
#let demo-dir = "../../doc/generated/demo"
#let demo-path(path) = demo-dir + "/" + path

#let demo-image(path, args: (:)) = {
  image(demo-path(path), ..args)
}
#let render-value(value) = {
  if value == "$infinity$" {
    $infinity$
  } 
  else if value == "$-infinity$" {
    $-infinity$
  }  
  else {
    raw(value)
  }
}

#let class-label(module, class) = {
  label(module + "::" + class)
}

#let class-info(module, class) = {
  api.modules.at(module).classes.at(class)
}

#let inline-code(text) = {
  let parts = text.split("`")

  // An even number of parts means an unmatched backtick.
  if calc.even(parts.len()) {
    // Render unchanged rather than silently losing text.
    return text
  }

  let result = []

  for (index, part) in parts.enumerate() {
    if calc.odd(index) {
      // Text between backticks.
      result += raw(part)
    } else {
      // Ordinary description text.
      result += [#part]
    }
  }

  result
}

#let parameter-table(parameters) = {
  if parameters.len() == 0 {
    return
  }

  let long-defaults = parameters.filter(
    p => p.default.len() > 12
  )

  let footnote-label(i) = {
    "abcdefghijklmnopqrstuvwxyz".at(i) + ")"
  }

  let rendered-default(p) = {
    if p.default.len() <= 12 {
      render-value(p.default)
    } else {
      let i = long-defaults.position(
        x => x.id == p.id
      )
      footnote-label(i)
    }
  }

  table(
    columns: (1.2fr, 1.5fr, 1fr, 3fr),
    inset: (x: 6pt, y: 5pt),
    stroke: none,
 
    row-gutter: (2pt, 0pt),
 
    table.hline(y: 0, stroke: 0.6pt),

    table.header[*Parameter*][*Type*][*Default*][*Description*],

    table.hline(y: 0, position: bottom, stroke: 0.6pt),
    table.hline(y: 1, position: top, stroke: 0.6pt),
  
    ..parameters.map(p => (
      raw(p.name),
      raw(p.type),
      rendered-default(p),
      inline-code(p.description)
    )).flatten(),
    table.hline(stroke: 0.6pt),
  )

  if long-defaults.len() > 0 {
    // v(4pt)

    for (i, p) in long-defaults.enumerate() {
      [
        #footnote-label(i)
        #h(3pt)
        #render-value(p.default)
      ]
      linebreak()
    }
  }
}

#let class-api(module, class) = {
  let info = class-info(module, class)

  [
    #heading(level: 2)[Class: #class]
    #label(module + "_" + class)
  ]

  if "inputs" in info and info.inputs.len() > 0 {
    heading(level: 3)[Inputs]
    parameter-table(info.inputs)
  }

  if "outputs" in info and info.outputs.len() > 0 {
    heading(level: 3)[Outputs]
    parameter-table(info.outputs)
  }
}

#let code-with-link(url, label, body) = {
  block(
    fill: luma(245),
    inset: 8pt,
    radius: 4pt,
    width: 100%,
  )[
    #link(url)[
      #text(font: "DejaVu Sans Mono", fill: blue, size: 0.9em)[#label]
    ] \
    #raw(body)
  ]
}

#let demo-figure(path, caption: none, args: (:)) = {
  figure(
    image(demo-path(path), ..args),
    caption: caption,
  )
}