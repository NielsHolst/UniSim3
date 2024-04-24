source("~/QDev/unisim3/input/scripts/begin.R")
library(treemapify)

M = data.frame (
  Component  = factor(c("Structural", "Structural", "Structural", "CReserves", "CReserves", "NReserves", "NReserves")),
  Material   = factor(c("C",          "N",          "Not CN",     "C",         "Not C",     "N",         "Not N")),
  Percentage =        c(21,           2,            100-21-2,     47,          100-47,      37,          100-37)
)
M$Component = reorder_levels(M$Component, c(3:1))
M

M2 = data.frame (
  Component  = factor(c("Structural", "Structural", "Structural", "CReserves", "CReserves", "CReserves", "NReserves", "NReserves", "NReserves")),
  Material   = factor(c("C",          "N",          "Not CN",     "C",         "N",         "Not CN",    "C",         "N",         "Not CN")),
  Percentage =        c(21,           2,            100-21-2,     47,          2,           100-47-2,    21,          37,          100-21-37)
)
M2$Component = reorder_levels(M2$Component, c(3:1))
M2


make_plot = function(M) {
  ggplot(M, aes(area=Percentage, fill=Component, subgroup=Material)) +
    geom_treemap() +
    geom_treemap_subgroup_border(colour = "white", size = 5) +
    geom_treemap_subgroup_text(place = "centre", grow = TRUE, alpha = 0.25, colour = "white")
}

  ggplot(M2, aes(area=Percentage, fill=Material, subgroup=Component)) +
    geom_treemap() +
    geom_treemap_subgroup_border(colour = "white", size = 5) +
    geom_treemap_subgroup_text(place = "centre", grow = TRUE, alpha = 0.25, colour = "white")



ggarrange(
  make_plot(M),
  make_plot(M2),
  ncol=1
)


