# How to update site HTML and other files

## Update HTML pages

The `update-html.R` script updates all HTML files found in the `site` root folder:

```R
setwd("~/QDev/UniSim3/site")
source("R/common.R")
:
files = c(
  "index.html",
  "intro.html",
  "commands.html",
  "boxscript.html",
  "models.html",
  "cereal-aphids.html",
  "vg.html",
  "saccharina.html",
  "download.html",
  "about.html"
)
l_ply(files, update_file)
```

The HTML files are updated *in situ*. Each file corresponds to one HTML file located at the root, e.g., <www.ecolmod.org/vg.html>.

## Upload site

* Upload (update newer) all files and sub-folders in `site` to the server root.
* Upload (update newer) the`UniSim3/input` folder to the server root.

## Source files

### Page header

The page header is defined in the `site/snippets/header.html` file:

```html
  <div class="begin macro"></div>
    <div id="header" class="border-blue">
      <div id="title">
        <img src="media/squirrel.gif"/>
        <div>
          <h1 id="top">Ecological Modelling Laboratory</h1>
          <p class="slogan">#SLOGAN</p>
        </div>
      </div>
      <nav>
        <a href="index.html" id="home">Home</a>
        <a href="intro.html" id="intro">Intro</a>
        <a href="commands.html" id="commands">Commands</a>
        <a href="boxscript.html" id="boxscript">BoxScript</a>
        <a href="models.html" id="models">Models</a>
        <a href="download.html" id="download">Download</a>
        <a href="about.html" id="about">About</a>
      </nav>
    </div>
  <div class="end macro"></div>
```

The header is specified by the first element in the `body` of the HTML page. This is from the `models.html` page:

```html
  <body>
  <div class="insert macro">#header.html#models#Let's sharpen the questions</div>
```

The three tokens are initiated by `#` and indicates

* the name of the header file (`header.html`)
* the name of the top menu item that this page belongs to (`models`)
* the slogan of this page (`Let's sharpen the questions`)

### Page footer

The page footer is defined in the `site/snippets/header.html` file:

```html
<div class="begin macro"></div>
<div id="footer" class="border-brown">
  <p>The Ecological Modelling Laboratory is the lab of</p>
  <p>Senior Scientist Niels Holst, Dept. of Agroecology, Aarhus University, Denmark</p>
</div>
<div class="end macro"></div>
```

### Right column

The call-outs in the right column are defined in the `site/snippets/right.html` file:

```html
<div class="begin macro"></div>
<div class="right">
  <div class="border-red">
    <h2>Try it!</h2>
    <p><a href="https://tildeweb.au.dk/au152367/download.html">Download</a> the latest version with the newly published Cereal Aphid-Fungus model. Also includes the Virtual Greenhouse model. </p>
    <p class="date">5 Oct 2023</p>
  </div>
  <div class="border-violet">
    <h2>Model just published</h2>
    <p>Read <a href="https://link.springer.com/article/10.1007/s10340-023-01674-w">our paper</a> on the Cereal Aphid-Fungus model and study the <a href="https://tildeweb.au.dk/au152367/models.html">detailed documentation</a>. Any questions? <a href="mailto:niels.holst@agro.au.dk">Write us</a>.</p>
    <p class="date">2 Aug 2023</p>
  </div>
  <div class="border-pink">
    <h2>Home page overhaul</h2>
    <p>We remain candy-coloured until further notice.</p>
    <p class="date">1 Aug 2023</p>
  </div>
  <div class="border-orange">
    <h2>Contact</h2>
    <p>Any questions concerning our models and tools? Interested in visiting the lab? Want to chat online? <a href="mailto:niels.holst@agro.au.dk">Write us</a>.</p>
  </div>
</div>
<div class="end macro"></div>
```

### Class interfaces

The interfaces of all classes are generated as tables, which are found as HTML files in the `snippets/plugins` folder. These files are updated with the `help HTML` command a the UniSim prompt. Here is one example:

```html
<div class="begin macro"></div>
<h2>Accumulator</h2>
<h3>Interface</h3><table id="table-interface"><thead><tr><th>Inputs</th><th>Type</th><th>Default</th><th>Purpose / Expression</th></tr></thead><tbody>
<tr><td>initial</td><td>double</td><td>0.0 </td><td>Initial <span class="table-interface-code">value</span> at reset</td></tr>
<tr><td>change</td><td>double</td><td>0.0 </td><td>Change added to <span class="table-interface-code">value</span></td></tr>
<tr><td>minValue</td><td>double</td><td>-1.79769e+308 </td><td>Minimum possible <span class="table-interface-code">value</span></td></tr>
<tr><td>maxValue</td><td>double</td><td>1.79769e+308 </td><td>Maximum possible <span class="table-interface-code">value</span></td></tr>
<tr><th>Outputs</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>value</td><td>double</td><td>0.0 </td><td>Accumulated value</td></tr>
<tr><td>signal</td><td>double</td><td>0.0 </td><td>Synonym for <span class="table-interface-code">value</span></td></tr>
</tbody></table>
<div class="end macro"></div>
```



### Media files

Media files (PNG, JPG, etc.) are found in the `site/media` folder, which is organised in sub-folders following the structure of `UniSim3/input` folder.

### Input files

Input files (BOX, R, TXT, etc.) are found in the `UniSim3/input` folder.

