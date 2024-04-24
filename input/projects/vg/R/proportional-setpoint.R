threshold      = 15
threshold_band = 10

x = 10:30

f = function(x, y0, y1) {
  slope = (y1-y0)/(threshold_band)
  intercept = y0 - threshold*slope
  y = slope*x + intercept
  ymin = min(c(y0,y1))
  ymax = max(c(y0,y1))
  y[y<ymin] = ymin
  y[y>ymax] = ymax
  y
}

make_plot = function(y0, y1) {

  M = data.frame(
    Input = x,
    Setpoint = f(x, y0, y1)
  )

  ggplot(M, aes(Input, Setpoint)) +
    geom_line(size=1, colour=blue) +
    scale_x_continuous(breaks=5*(0:6)) +
    annotate("rect", xmin=threshold, xmax=threshold+threshold_band, ymin=-Inf, ymax=Inf, colour=red, fill=red, alpha=0.3) +
    theme_bw() +
    theme(
      axis.title.y = element_text(margin = margin(t = 0, r = 10, b = 0, l = 0))
    )
}

windows(6,2)
ggarrange(
  make_plot(100, 300),
  make_plot(300, 100)
)

