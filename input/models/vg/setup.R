# Prelude
setwd("~/QDev/UniSim3/input/models/vg")
library(ggplot2)
library(ggpubr)
library(plyr)
library(reshape2)

#
# Set up default ggplot colours and theme
#

theme_set(theme_bw())

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

show_colours = function() {
  n = length(my_colours)
  M = data.frame(
    Colour = factor(1:n),
    Value = rep(1, n)
  )

  windows(8,5)
  ggplot(M, aes(Colour, Value, colour=Colour)) +
    geom_point(size=10)
}
