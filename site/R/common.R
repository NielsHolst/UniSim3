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
  # Change to site_new folder
  wd = getwd()
  setwd("..")
  if (!dir.exists("site_new")) {
    dir.create("site_new")
  }
  setwd("site_new")
  # Write file
  print(paste("Writing", file_name))
  f = file(file_name, "wt", encoding="UTF-8")
  writeLines(lines, f, sep="\n")
  close(f)
  # Change back to previous folder
  setwd(wd)
}

write_file_debug = function(lines) {
  print(paste("Writing", "ud.html"))
  f = file("ud.html", "wt", encoding="UTF-8")
  for (i in 1:length(lines)) {
    print(lines[[i]])
    writeLines(lines[[i]], f, sep="\n")
  }
  close(f)
}
