rm(list=ls(all=TRUE))
setwd("~/QDev/UniSim3/site")
source("R/common.R")

files = list.files("snippets/plugins/vg")
files

f = function(file_name) {
  lines = read_file(paste0("snippets/plugins/vg/", file_name))
  c("<p class='hidden-anchor' id='virtual-greenhouse'>xxx</p><div class='border-grey'>", 
    lines, 
    "</div> <!-- border-grey -->")
}

x = sapply(files[1:2], f) |> flatten() |> unlist() 
write_file("ud.txt", x)


