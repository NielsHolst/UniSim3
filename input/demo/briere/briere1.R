# briere1.R
sim = sim[1:3]
colnames(sim) = c("Temperature", "DayDegrees", "Briere")
sim$DayDegrees = sim$DayDegrees/472.1

obs = data.frame(
  Temperature = c(15,17,20,25,27,30,33,35),
  L = c(Inf, 
        121.5, 
        (83.68+78.46)/2,
        (47.97+41.32)/2,
        (38.92+34.42)/2,
        (35.05+32.41)/2,
        (38.33+37)/2,
        Inf)
)
obs$DevRate = 1/obs$L

model = lm(DevRate~Temperature, data=obs[1:5,])
icept = coef(model)[1]
slope = coef(model)[2]
T0 = -icept/slope
duration = 1/slope

open_plot_window(6,3.5)
M = melt(sim, id.vars="Temperature", value.name="DevRate", variable.name="Model")
P = ggplot(M, aes(x=Temperature, y=DevRate, colour=Model)) +
  geom_abline(intercept=icept, slope=slope, colour=brown, size=3, alpha=0.6) +
  geom_line(size=1) +
  geom_point(data=obs, colour=red, size=3) +
  geom_vline(xintercept=T0) +
  geom_hline(yintercept=0) +
  scale_color_manual(values=c(green,blue)) +
  scale_x_continuous(breaks=c(T0,20,25,30,35), labels=c(round(T0,1), 20,25,30,35)) +
  labs(x="Temperature (oC)", y="1/L (per day)")
print(P)

all_figures = list(P)
