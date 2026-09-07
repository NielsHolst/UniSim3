# Extract path to QRC file
args = commandArgs(trailingOnly = FALSE)
script_arg = grep("^--file=", args, value = TRUE)
script_file = sub("^--file=", "", script_arg)
project_dir = dirname(normalizePath(script_file, winslash = "/"))
qrc_file_name_path <- file.path(project_dir, "resources/application.qrc")

# Explicit entries
explicit_files = c(
    "boxes_head.cson",
    "notepad_grammar_template.xml",
    "version.txt"
)

# Add everything below data/input recursively
input_path = file.path(project_dir, "resources", "data", "input")

input_files = list.files(
    input_path,
    recursive = TRUE,
    full.names = TRUE,
    all.files = TRUE,
    no.. = TRUE
)

# Remove directories
input_files = input_files[!dir.exists(input_files)]

# Normalize absolute paths
input_files = normalizePath(
    input_files,
    winslash = "/",
    mustWork = TRUE
)

# Convert absolute paths to resource-relative paths
resources_dir = normalizePath(
    file.path(project_dir, "resources"),
    winslash = "/"
)

prefix = paste0(resources_dir, "/")

input_files = substring(
    input_files,
    nchar(prefix) + 1
)

# Collate file lists
all_files = c(explicit_files, sort(input_files))

# XML code wrappers
xml_escape = function(x) {
    x = gsub("&", "&amp;", x, fixed = TRUE)
    x = gsub("<", "&lt;", x, fixed = TRUE)
    x = gsub(">", "&gt;", x, fixed = TRUE)
    x = gsub("\"", "&quot;", x, fixed = TRUE)
    x
}

new_lines = c(
    '<!DOCTYPE RCC>',
    '<RCC version="1.0">',
    '  <qresource>',
    sprintf("    <file>%s</file>", xml_escape(all_files)),
    '  </qresource>',
    '</RCC>'
)

# Read the old QRC file
old_lines = if (file.exists(qrc_file_name_path)) {
    readLines(qrc_file_name_path, warn = FALSE)
} else {
    character()
}

# Write new QRC file if changed

# Note: We don't need to rewrite the QRC file, if the content of an input file changed.
#       We only need a rewrite, if new input files appeared or old ones disappeared

if (!identical(old_lines, new_lines)) {
    message(paste("Writing resource file to:", qrc_file_name_path))
    writeLines(new_lines, qrc_file_name_path, useBytes = TRUE)
} else {
    message("Resource file unchanged")
}

