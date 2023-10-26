library(plyr)
library(stringr)

read_file = function(file_name) {
  print(paste("Reading", file_name))
  con = file(file_name,open="r")
  lines = readLines(con, warn=FALSE) 
  close(con)
  lines
}

write_file = function(file_name, lines) {
  print(paste("Writing", file_name))
  f = file(file_name, "wt", encoding="UTF-8")
  # writeLines(lines, f, sep="\n", useBytes=TRUE)
  writeLines(lines, f, sep="\n")
  close(f)
}
