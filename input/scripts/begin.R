BEGIN = TRUE

library("colorspace")
library("ggplot2")
library("ggpubr")
library("gridExtra")
library("lubridate")
library("plyr")
library("reshape2")
library("scales")
library("splines")
library("stringr")

theme_set(theme_bw())

# See https://data-se.netlify.com/2018/12/12/changing-the-default-color-scheme-in-ggplot2/
red = '#e41a1c'
blue = '#377eb8'
green = '#4daf4a'
violet = '#984ea3'
orange = '#ff7f00'
brown = '#a65628'
pink = '#f781bf'
grey = '#999999'
unisim_colours = c(red,blue,green,violet,orange,brown,pink,grey) 

unisim_colour = function(i) { 
  strtoi(c(
    paste0("0X", substr(unisim_colours[i], 2,3)),
    paste0("0X", substr(unisim_colours[i], 4,5)),
    paste0("0X", substr(unisim_colours[i], 6,7))
  ))
}

n_rep_colours = 20
scale_colour_discrete = function(...) {
  scale_colour_manual(..., values = rep(unisim_colours, n_rep_colours))
}

scale_fill_discrete = function(...) {
  scale_fill_manual(..., values = rep(unisim_colours, n_rep_colours))
}

scale_linetype_discrete = function(...) {
  n = length(unisim_colours)
  scale_linetype_manual(..., values = rep(c(rep(1, n), rep(7, n)), 5))
}

update_geom_defaults("line", list(linewidth=1))

ggplot_theme = function(fontSize) {
  if (fontSize==0) {
    NULL
  } else {
    sz = element_text(size = fontSize)
    sz1 = element_text(size = fontSize+1)
    theme(axis.text    = sz,
          axis.title   = sz,
          strip.text.x = sz,
          strip.text.y = sz,
          strip.text   = element_text(margin = margin(t=2, b=3)),
          legend.title = sz1,
          legend.text  = sz,
          )
  }
}

clr = function() {
  graphics.off()
  rm(list=ls(all=TRUE))
}

last_occurence = function(s, needle) {
  tail(str_locate_all(s, needle)[[1]], 1)[1]
}

output_file_folder = function() {
  last = last_occurence(output_file_name, "/")
  substr(output_file_name, 1, last-1)
}

output_file_base_name = function() {
  last = last_occurence(output_file_name, "/")
  file_name = substring(output_file_name, last+1)
  last = last_occurence(file_name, "\\.")
  substr(file_name, 1, last-1)
}

reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

is_windows = function() {
  (.Platform$OS.type == "windows")
}

is_rstudio = function() {
  !is.na(Sys.getenv("RSTUDIO", unset = NA))
}

open_plot_window = function(width=7, height=7) {
  # dev.new(width=width, height=height, noRStudioGD=TRUE)
  if (is_windows()) {
    windows(width=width, height=height) 
  } else {
    X11(width=width, height=height, type="cairo") 
  }
}

# Millimeters from inches; used for output to graphics files
mm = function(x) x/25.4

read_output = function(file_path) {

  column_info = function(file_path) {
    sip = read.table(file_path, header=TRUE, sep="\t", stringsAsFactors=FALSE, nrows=1, na.strings=c("NA","nan","nan.0"))
    Rformat = unlist(sip[1,])
    ix_date = (Rformat == "ymd") | (Rformat == "HMS") | (Rformat == "ymdHMS")
    read_format = Rformat
    read_format[ix_date]  = "character"
    names(read_format) = NULL
    Rformat[!ix_date]  = NA
    list(col_names=colnames(sip), Rformat=Rformat, read_format=read_format)
  }

  if (skip_formats) {
    U = read.table(file_path, header=TRUE, sep="\t")
  } else {
    # Extract column info
    cl = column_info(file_path)

    # Read data frame
    U = read.table(file_path, header=FALSE, sep="\t", colClasses = cl$read_format, skip=2)
    colnames(U) = cl$col_names

    # Convert date/time columns
    ix_date = which(!is.na(cl$Rformat))
    if (length(ix_date) > 0 ) {
      for (i in 1:length(ix_date)) {
        ix = ix_date[i]
        U[,ix] = parse_date_time(U[,ix], c(cl$Rformat[ix]))
      }
    }
  }
  U
}

convert_logical = function(df) {
  ix_bool = which(sapply(df,class)=="logical")
  for (ix in ix_bool) {
    df[,ix] = as.numeric(df[,ix])
  }
  df
}

convert_factors = function(df, cols) {
  ix_factor = which(sapply(df[cols],is.factor))
  for (ix in ix_factor)
    df[cols[ix]][,1] = as.numeric(df[cols[ix]][,1])
  df
}

remove_character_columns = function(df, cols) {
  ix = which(sapply(df[cols],is.character))
  if (length(ix)==0) cols else cols[-ix]
}

sim_columns = function() {
  c(iterationColumn, stepColumn)
}

melt_y = function(df, x, y) {
  # Output columns: iteration step <x1> <x2> <x...> Variable Value
  y = remove_character_columns(df, y)
  df = convert_factors(df, y)
  x  = union(sim_columns(), x)
  xy = union(x, y)
  M = melt(convert_logical(df)[ ,xy], id.vars=x, value.name="Value", variable.name="Variable")
  M[,iterationColumn] = factor(M[,iterationColumn])
  M
}

melt_xy = function(df, x, y) {
  if (length(x)==length(y) && all(x==y)) {
    print("x is equal to y:")
    print(x)
    stop("stopped")
  }
  # Produce columns: iteration step <y1> <y2> <y...> xVariable xValue
  M = melt(convert_logical(df), id.vars=c(sim_columns(),y), measure.vars=x, value.name="xValue", variable.name="xVariable")
  # Produce columns: iteration step xValue xVariable Response ResponseValue
  M = melt(M, id.vars=c(sim_columns(),"xValue","xVariable"), measure.vars=y, value.name="ResponseValue", variable.name="Response")
  M[,iterationColumn] = factor(M[,iterationColumn])
  M
}

Log10 = function(x) {
  y = rep(NA, length(x))
   y[x>0] = log10(x[x>0])
  y
}

# df = data.frame(
  # iteration = c(rep(1,3), rep(2,4)),
  # step = c(1:3, 1:4),
  # x1 = c(1:3,11:14),
  # y1 = c(51:53,61:64)
# )
# df$x2 = 10*df$x1
# df$y2 = 10*df$y1
# x = c("x1","x2")
# y = c("y1","y2")


plot_one_x = function(df, x, y, ncol, nrow, dir, layout) {
  # Produce columns: iteration <x> Variable Value
  # Note: length(x)==1, length(y)>=1
  M = melt_y(df, x, y)

  # Colour by iteration if there are many iterations and only one variable;
  # otherwise, colour by Variable
  many_iterations = length(levels(M[,iterationColumn])) > 1
  one_variable = (length(y) == 1)
  color = if (many_iterations & one_variable) iterationColumn else "Variable"

  # If there is more than one step, it means the plot is a time series
  is_time_series = length(unique(M[,stepColumn])) > 1
  
  # Put the one x column on the x-axis
  # and the Value column on the y-axis
  # P = ggplot(M, aes_string(x=x, y="Value", color=color))  - deprecated use if aes_string
  P = ggplot(M, aes(.data[[x]], Value, color=.data[[color]]))
  
  # Use lines for time series otherwise points
  if (is_time_series) {
    P = P + geom_line()
  } else {
    P = P + geom_point()
  }

  # Plot in facets
  if (layout == "facetted") {
    # Show no legends for colours, as plot will be facetted
    P = P + theme(legend.position="none")
      
    # If there are many iterations then facet by iteration and Variable
    if (many_iterations) {
      form = paste(iterationColumn, "Variable", sep="~")
      P + facet_grid(as.formula(form), scales="free_y") + xlab("") + ylab("")
    # If there is only one iteration and many variables then facet by Variable only
    } else if (!one_variable) { 
      P + facet_wrap(~Variable, ncol=ncol, nrow=nrow, dir=dir, scales="free_y") + xlab(x) + ylab("")
    # If there is only one iteration and only one Variable then produce a simple (x,y) plot
    # Note: x and y both have length 1
    } else {
      P + xlab(x) + ylab(y)
    }
  }
  # Otherwise (keep colour legend)
  else {
    # If there is only one Variable then produce a simple (x,y) plot
    # Note: x and y both have length 1; many iterations will produce many colours
    if (one_variable) {
      P = P + xlab(x) + ylab(y)
    # If there are many variables, the legend will describe them - no y-axis label needed
    } else {
      P = P + xlab(x) + ylab("")
    }
    # If there are many iterations and many variables then facet them by iterations
    # Note: Each facet will be coloured by Variables (hence variables are merged in eacg facet)
    if (many_iterations & !one_variable) { 
      form = paste0("~", iterationColumn)
      P + facet_wrap(as.formula(form), ncol=ncol, nrow=nrow, dir=dir)
    } else {
      P
    }
  }
}

plot_many_x = function(df, x, y, ncol, nrow, dir, layout) {
  # Produce columns: iteration step xValue xVariable Response ResponseValue
  # Note: length(x)>1, length(y)>=1
  M = melt_xy(df, x, y)

  # Colour by iteration if there are many iterations nad we've got time seriues;
  # otherwise, colour by Response
  many_iterations = length(levels(M[,iterationColumn])) > 1
  is_time_series = length(unique(M[,stepColumn])) > 1
  color = if (many_iterations & is_time_series) "iteration" else "Response"
  
  # Put  xValue on the x-axis and ResponseValue on the y-axis
  P = ggplot(M, aes(x=Value, y=ResponseValue, color=.data[[color]])) +
        xlab("") + ylab("")
  
  # Use lines for time series otherwise points
  if (is_time_series) {
    P = P + geom_line(aes(group=.data[[iterationColumn]]))
  } else {
    P = P + geom_point()
  }

  # Plot in facets with one iteration per facet,
  # or if many facets, merge iterations (coloured) into each facet
  if (layout == "facetted" | many_iterations) {
    P = P + facet_grid(Response~xVariable, scales="free") 
  # Otherwise, if only iteration merge Response ((coloured)) into each facet
  } else {
    P = P + facet_wrap(~xVariable, scales="free", dir=dir) 
  }
  P
}

plot_facetted = function(df, x, y, ncol, nrow, dir) {
  if (length(x) == 1) {
    plot_one_x( df, x, y, ncol, nrow, dir, "facetted") 
  } else {
    plot_many_x(df, x, y, ncol, nrow, dir, "facetted")
  }
}

plot_merged = function(df, x, y, ncol, nrow, dir) {
  if (length(x) == 1) {
    plot_one_x( df, x, y, ncol, nrow, dir, "merged") 
  } else {
    plot_many_x(df, x, y, ncol, nrow, dir, "merged")
  }
}

plot_density = function(df, ports, ncol, nrow, dir) {
  M = melt(df[ports], value.name="Value", variable.name="Variable")
  ggplot(M, aes(x=Value, colour=Variable, fill=Variable)) +
    geom_density(alpha=0.3) +
    labs(y="") + 
    facet_wrap(~Variable, scales="free", ncol=ncol, nrow=nrow, dir=dir) +
    theme(
      axis.text.y = element_blank(),
      axis.ticks.y = element_blank(),
      legend.position="none"
    )
}

plot_histogram = function(df, ports, bins, ncol, nrow, dir) {
  M = melt(df[ports], value.name="Value", variable.name="Variable")
  ggplot(M, aes(x=Value, colour=Variable, fill=Variable)) +
    geom_histogram(alpha=0.3, bins=bins) +
    labs(y="") + 
    facet_wrap(~Variable, scales="free", ncol=ncol, nrow=nrow, dir=dir) +
    theme(legend.position="none")    
}
