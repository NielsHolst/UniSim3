rm(list=ls(all=TRUE))
setwd("~/QDev/UniSim3/site")
source("R/common.R")

extract_macros = function(lines) {
  n = length(lines)
  next_token = "insert macro"
  macro = rep(NA, 6)
  macros = data.frame()
  for (i in 1:n) {
    if (!is.na(str_locate(lines[i], next_token)[1])) {
      if (next_token=="insert macro") {
        macro[1] = i
        next_token = "begin macro"
        s = lines[i]
        limits = c(str_locate_all(s, "#")[[1]][,1], str_locate(s, "</")[,1])
        for (j in 1:(length(limits) - 1)) {
          macro[3+j] = substr(s, limits[j]+1, limits[j+1]-1)
        }
        
      } else if (next_token=="begin macro") {
        macro[2] = i
        next_token = "end macro"

      } else {
        macro[3] = i
        next_token = "insert macro"
        macros = rbind(macros, macro)
        macro = rep(NA, 6)
      }
    }
  }
  macros[,1] = as.numeric(macros[,1])
  macros[,2] = as.numeric(macros[,2])
  macros[,3] = as.numeric(macros[,3])
  colnames(macros) = c("Insert", "Begin", "End", "File", "Id", "Slogan")
  if (next_token != "insert macro") {
    print(macros)
    stop(paste("Macro incomplete. Expected token:", next_token))
  }
  macros
}

extract_headers = function(lines) {
  n = length(lines)
  headers = data.frame()
  index_headers = FALSE
  for (i in 1:n) {
    if (str_detect(lines[i], "begin-index-headers")) 
      index_headers = TRUE
    if (str_detect(lines[i], "end-index-headers"))
      index_headers = FALSE
    caption_line = str_locate(lines[i], "<h[1-6][^>]*>")
    if (index_headers & !any(is.na(caption_line))) {
      level = caption_line[1] + 2
      from = caption_line[2] + 1
      to = str_locate(lines[i], "</h[1-6]>")[1] - 1

      level = substr(lines[i],level,level)
      caption = substr(lines[i], from, to) |> str_trim()
      id = str_to_lower(caption) |>             # all characters lowercase
           str_replace_all("&[^;]*;", "") |>    # remove character codes
           str_replace_all("<[^>]*>", "") |>    # remove HTML tags
           str_trim() |>                        # remove leading and trailing whitespace
           str_replace_all("[^A-Za-z0-9]+","-") # replace all non-alphanumeric characters with a "-"
      n = nchar(id)
      if (substr(id,n,n) == "-") 
        id = substr(id, 1, n-1)
      include_header = TRUE
      h = nrow(headers)
      if (h > 0) if (caption == headers[h,3]) include_header = FALSE # Avoid adding superflous, second repeat of caption
      if (include_header)
        headers = rbind(headers, c(i, level, caption, id))
    }
  }
  colnames(headers) = c("Line", "Level", "Caption", "Id")
  headers$Line = as.numeric(headers$Line)
  headers$Level = as.numeric(headers$Level)

  # Add menu lines
  headers$Menu = ""
  n = nrow(headers)
  level = min(headers$Level)
  for (i in 1:n) {
    menu = ""
    h = headers[i,] 
    level_step = h$Level - level
    if (h$Level < 3) {
      if (i==1) {
        # Begin list
        menu = "<ul class=\"border-grey\"><a href=\"#\"><img class=\"squirrel-link\" src=\"media/squirrel.gif\"/ title=\"Go to top of page\"></a>"
      } else if (level_step == 0 ){
        # Remain on level 
        menu = paste0(menu, "</li>")
      } else if (level_step > 0) {
        # Go to a deeper level
        menu = paste0(menu, "<ul>")
      } else {
        # Go to a shallower level {
        menu = paste0(menu, "</li></ul></li>")
      }
      menu = paste0(menu, "<li><a href=\"#", h$Id, "\">", h$Caption, "</a>")
      
      if (i == n) {
        remaining = h$Level - min(headers$Level) + 1
        if (remaining > 0)
          menu = paste0(menu, paste0(rep("</li></ul>", remaining), collapse=""))
      }
      # headers$Menu[i] = if (h$Level < 3) menu else ""
      headers$Menu[i] = menu 
      level = h$Level
    }
  }
  # Done
  headers
}

update_anchors = function(lines, headers) {
  lines_with_anchor = {}
  for (i in 1:nrow(headers)) {
    h = headers[i,]
    # Look for border up to three lines back
    i = 0
    border = NA
    while (is.na(border) & i < 4) {
      i = i + 1
      border = str_extract(lines[h$Line - i], "border-[^\\\"']*")
    }
    if (is.na(border)) {
      # Put anchor in headline
      lines[h$Line] = with(h, paste0("<h", Level, " id='", Id, "'>", Caption, "</h", Level, ">"))
    } else {
      # Remove any anchor from headline itself
      lines[h$Line] = with(h, paste0("<h", Level, ">", Caption, "</h", Level, ">"))
      # Put anchor in line above
      anchor_line_ix = h$Line - i 
      anchor_code = paste0("<p class='hidden-anchor' id='", h$Id, "'>xxx</p>")
      if (anchor_line_ix %in% lines_with_anchor) {
        # Add anchor id
        lines[anchor_line_ix] = paste0(anchor_code, lines[anchor_line_ix])
      } else {
        # Put anchor
        lines[anchor_line_ix] = paste0(anchor_code, "<div class='", border, "'>")
      }
      lines_with_anchor = c(lines_with_anchor, h$Line - i)
    }
  } 
  lines
}

update_file = function(file_name) {
  lines = read_file(file_name)
  macros = extract_macros(lines)

  # Remove inserted macros
  removed = 0
  for (i in 1:nrow(macros)) {
    macros$Insert[i] = macros$Insert[i] - removed
    macros$Begin[i]  = macros$Begin[i]  - removed
    macros$End[i]    = macros$End[i]    - removed
    m = macros[i,]
    lines = lines[-(m$Begin:m$End)]
    removed = removed + m$End - m$Begin + 1
  }

  # Insert macros
  inserted = 0
  for (i in 1:nrow(macros)) {
    macros$Insert[i] = macros$Insert[i] + inserted
    insert = macros$Insert[i]
    slogan = macros$Slogan[i]
    id     = macros$Id[i]
    insertion = read_file(paste0("snippets/",macros$File[i]))
    if (!is.na(slogan)) {
      insertion = str_replace_all(insertion, "#SLOGAN", slogan)
    }
    if (!is.na(id)) {
      id_string = paste0("id=\"", id, "\"") 
      insertion = str_replace_all(insertion, id_string, "id=\"current\"")
    }
    lines = c(lines[1:insert], insertion, lines[(insert+1):length(lines)])
    inserted = inserted + length(insertion)
  }

  # Organise headers and menu
  headers = extract_headers(lines)
  lines = update_anchors(lines, headers)

  menu_begin = which(str_detect(lines, "begin-menu"))
  menu_end = which(str_detect(lines, "end-menu"))
  n = length(lines)
  menu = paste0(headers$Menu, collapse="")

  if (length(menu_begin) > 0)
    lines = c(lines[1:menu_begin], menu, lines[menu_end:n])

  # Write file
  # write_file(file_name, lines)
  write_file("ud.html", lines)
}

update_file(file_name = "boxscript.html")

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
