suffix = ".png"
# scale_output = 0.5

output_path = function() {
  slashes = str_locate_all(output_file_name, "/")[[1]]
  last_slash = slashes[nrow(slashes)]
  substr(output_file_name, 1, last_slash-1)
}

insert_file_number = function(filePath, number) {
  paste0(
    filePath,
    "_", number, suffix)
}
 
save_figure_page = function(page, filePath) {
  print(paste("Saving figure:", filePath, "..."))
  png(filePath, page$Width, page$Height, "in", res=72)
  print(page$Grob)
  dev.off()
}

check_figure_inputs = function() {
  for (i in 1:length(all_figures)) {
    if (!("FilePath" %in% names(all_figures[[i]])))
      print(paste("Incomplete figure", i))
  }
}

save_figure = function(figure) {
  if (!("FilePath" %in% names(figure)))
    stop("Incomplete figure")
  filePath = figure$FilePath
  baseName = figure$FileBaseName
  fileType = figure$FileType
  pages = figure$Pages[[1]]
  numPages = length(pages)
  filePathBaseName = paste0(filePath, "/", baseName)
  
  if (!dir.exists(filePath))
    dir.create(filePath, recursive=TRUE)
  if (numPages == 0) {
    print(paste0("No figure produced by ", paste0(filePathBaseName, fileType), "; skipped"))
  } else if (numPages == 1) {
    silent = save_figure_page(pages[[1]], paste0(filePathBaseName, suffix))
  } else {
    for (i in 1:numPages) {
      silent = save_figure_page(pages[[i]], insert_file_number(filePathBaseName, i))
    }
  } 
}

figure_to_markdown = function(figure) {
  if (!("FilePath" %in% names(figure)))
    stop("Incomplete figure")
  with(figure, paste0("![", FileBaseName, "](", FilePath, "/", FileBaseName, ".png)\n"))
}


check_figure_output = function(figure) {
  filePath = figure$FilePath
  baseName = figure$FileBaseName
  fileType = figure$FileType
  pages = figure$Pages[[1]]
  numPages = length(pages)
  expected = figure$PagesExpected
  filePathName = paste0(filePath, "/", baseName, fileType)
  if (expected & numPages == 0) {
    print(paste("Expected figure was not produced for script:", filePathName))
  }
}

source_script = function(script) {
  print(paste0("Sourcing ", script, "..."))
  source(script)
}

Sys.setenv(LANG = "en_US.UTF-8") # use English spelling for e.g. months

work_dir = paste0(output_path(), "/batch_output")
if (!dir.exists(work_dir)) dir.create(work_dir)

print("Checking figure inputs..")
check_figure_inputs()
print("Done")

setwd(work_dir)
l_ply(all_figures, save_figure)
paste("Figures written as PNG files to", work_dir) |> print()

md = laply(all_figures, figure_to_markdown)
file_name = "all_figures.md"
f = file(file_name, "wt", encoding="UTF-8")
writeLines(md, f, sep="\n")
close(f)
paste0("Markdown file written to ", work_dir, "/", file_name) |> print()


print("Checking figure outputs..")
l_ply(all_figures, check_figure_output)
print("Done")

