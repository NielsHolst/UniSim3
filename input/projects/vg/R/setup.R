library(ggplot2)
library(plyr)
library(reshape2)

theme_set(
  theme_bw() +
  theme(
    axis.title.x = element_text(margin = unit(c(0.8,0,0,0), "char")),
    axis.title.y = element_text(margin = unit(c(0,0.8,0,0), "char"))
  )
)

# See https://data-se.netlify.com/2018/12/12/changing-the-default-color-scheme-in-ggplot2/
red = '#e41a1c'     # 228  26  28
blue = '#377eb8'    #  37 126 184
green = '#4daf4a'   #  77 175  74
violet = '#984ea3'  # 152  78 163
orange = '#ff7f00'  # 255 127   0
brown = '#a65628'   # 166  86  40
pink = '#f781bf'
grey = '#999999'
black = '#000000'
my_colours = c(red,blue,green,violet,orange,brown,pink,grey,black) 

update_geom_defaults("line", list(linewidth=1))

my_colour = function(i) { 
  strtoi(c(
    paste0("0X", substr(my_colours[i], 2,3)),
    paste0("0X", substr(my_colours[i], 4,5)),
    paste0("0X", substr(my_colours[i], 6,7))
  ))
}

n_rep_colours = 20
scale_colour_discrete = function(...) {
  scale_colour_manual(..., values = rep(my_colours, n_rep_colours))
}

scale_fill_discrete = function(...) {
  scale_fill_manual(..., values = rep(my_colours, n_rep_colours))
}

scale_linetype_discrete = function(...) {
  n = length(my_colours)
  scale_linetype_manual(..., values = rep(c(rep(1, n), rep(7, n)), 5))
}
