rm(list=ls(all=TRUE))
setwd("C:/MyDocuments/QDev/UniSim3/site")
source("R/common.R")
library("purrr")

plugin_name = "cmbp"

files = list.files(paste0("snippets/plugins/", plugin_name))
files

f = function(file_name) {
  id = str_extract(file_name, "^[^\\.]*")
  line1 = paste0("<p class='hidden-anchor' id='", id, "'>xxx</p><div class='border-grey'>") 
  line2 = paste0("<div class='insert macro'>#plugins/", plugin_name,"/", file_name, "</div>")
  lines = read_file(paste0("snippets/plugins/", plugin_name, "/", file_name))
  c(line1, line2, 
    lines, 
    "</div> <!-- border-grey -->\n")
}

x = sapply(files, f) |> flatten() |> unlist() 
write_file(paste0("extracted_", plugin_name, ".txt"), x)


