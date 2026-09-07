// output_writer.typ

- #link(<output-writer-usage>)[Usage]
- #link(<output-folder>)[Output folder]
- #link(<output-file-format>)[Output file format]

=== Usage
<output-writer-usage>
You\'ll never need to include an `OutputWriter` box in a boxscript
explicitly. It will be created automaticall by boxes that need it, such
as #link(<boxes_OutputR>)[OutputR] and #link(<boxes_OutputText>)[OutputText];. It is
the sole responsibility of `OutputWriter` to write the simulation output
to a file. All of its inputshas been given default expressions, which
look up the relevant data in other boxes. You set #emph[what] to change
what is written in the #link(<boxes_OutputR>)[OutputR] or
#link(<boxes_OutputText>)[OutputText] box and you can change #emph[how] it is
written in the #link(<boxes_OutputSelector>)[OutputSelector] box.

`OutputWriter` depends much on `OutputSelector`, in fact, so much that
if no `OutputSelector` box is present in the boxscript then the
`OutputWriter` box will create an `OutputSelector` as a child box.

The #link("https://ecolmod.org/input/demo/sun/sun1.box")[sun1.box⏷] script, for example,
includes an `OutputR` box explicitly in the script. In a chain reaction,
the `OutputR` box creates an `OutputWriter` box, which again creates an
`OutputSelector` box:

```
> load demo/sun/sun1.box
```

Most often you are not interested in the output file, as Universal
Simulator does its best to handle all output automatically behind the
scenes but, anyway, here are the details.

=== Output folder
<output-folder>
The output file will be written to your output folder, which you can
locate with the #highlight[get folders] command:

```
> get folders
```

The name of the output file will be the same as the boxscript that
produced it, just with a four-digit running number at the end. As a pure
text file, its suffix \(also known as its file type) is \"txt\". The
file consists of tab-separated columns, separated by tab characters
\(no, you cannot change the separator character to something else than a
tab).

The output folder will grow slowly for every run you make. Therefore,
you should sometimes empty it.

=== Output file format
<output-file-format>
The first line contains unique column names constructed from the names
of the `ports` included. The column names have been shortened as much as
possible while keeping them unique. Ports named `value` will have a
column name without the port part if possible; they will be named after
their parent box instead \(\"value\" is not an informative label).

If `skipFormats` is `FALSE`, which it defaults to because that\'s the
default value imported from #link(<boxes_OutputSelector>)[OutputSelector];,
then the second line in the output file will be information about the
format of the values in the column. These format specifications are used
when the file is read into R as a data frame. If you want to do all the
handling of the output file yourself then set `skipFormats` to `TRUE` in
the `OutputSelector` box.

If you want to use the decimal character specific to the language
settings on your computer then set `useLocalDecimalChar` to `TRUE` in
the `OutputSelector` box; otherwise a decimal point will be used.
