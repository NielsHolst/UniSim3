rm(list=ls(all=TRUE))
graphics.off()
source("~/Rdev/setup.R")
library(ggpubr)
library(reshape2)

# For daytime vs. night time parameters, Is > 3 W/m2 is the threshold
ri_Is = function(Is, LAI) {
  alpha = 0.8
  C1 = 4.30
  C2 = 0.54
  Is_avg = alpha*Is/2/LAI
  (Is_avg + C1)/(Is_avg + C2)
}

ri_To = function(To, daytime) {
  if (daytime) {
    C3 = 2.3e-2
    Tm = 24.5
  } else {
    C3 = 0.5e-2
    Tm = 33.6
  }
  1 + C3*(To - Tm)^2
}

ri_co2 = function(co2, daytime) {
  if (daytime) {
    C4 = 6.1e-7
    pmin(1 + C4*(co2-200)^2, 1.5)
  } else {
    1
  }
}
# ri_co2(1100, TRUE)

ri_vpd = function(vpd, daytime) {
  if (daytime) {
    C5 = 4.3
  } else {
    C5 = 5.2
  }
  pmin(1 + C5*vpd^2, 3.8)
}
# ri_vpd(0.8, TRUE)


plot_ri_Is = function() {
  x = 0:600
  M = data.frame(
    Is = x,
    LAI2 = ri_Is(x, 2),
    LAI4 = ri_Is(x, 4)
  )
  ggplot(melt(M, id.vars="Is", value.name="ri", variable.name="LAI"), aes(Is, ri, colour=LAI)) +
    geom_line() +
    scale_x_continuous(breaks=100*(0:6)) +
    ylim(0, NA)
}
# plot_ri_Is()

plot_ri_To = function() {
  x = 10:30
  M = data.frame(
    To = x,
    Day    = ri_To(x, TRUE),
    Night  = ri_To(x, FALSE)
  )
  ggplot(melt(M, id.vars="To", value.name="ri", variable.name="Period"), aes(To, ri, colour=Period)) +
    geom_line() +
    scale_x_continuous(breaks=5*(0:7), limits=c(0,NA)) +
    scale_y_continuous(breaks=0:10) 
}
# plot_ri_To()

plot_ri_vpd = function() {
  x = (0:100)/100
  M = data.frame(
    vpd = x,
    Day    = ri_vpd(x, TRUE),
    Night  = ri_vpd(x, FALSE)
  )
  ggplot(melt(M, id.vars="vpd", value.name="ri", variable.name="Period"), aes(vpd, ri, colour=Period)) +
    geom_line() +
    scale_x_continuous(breaks=0.2*(0:5), limits=c(0,NA)) +
    scale_y_continuous(breaks=0:10) 
}
# plot_ri_vpd()

plot_ri_co2 = function() {
  x = 10*(20:120)
  M = data.frame(
    co2 = x,
    Day    = ri_co2(x, TRUE),
    Night  = ri_co2(x, FALSE)
  )
  ggplot(melt(M, id.vars="co2", value.name="ri", variable.name="Period"), aes(co2, ri, colour=Period)) +
    geom_line() +
    scale_x_continuous(breaks=200*(0:6), limits=c(0,NA)) 
    # scale_y_continuous(breaks=0:10) 
}
# plot_ri_co2()

ggarrange(
  plotlist = list(plot_ri_Is(), plot_ri_To(), plot_ri_vpd(), plot_ri_co2()),
  ncol=2,
  nrow=2, 
  align="hv" 
)
