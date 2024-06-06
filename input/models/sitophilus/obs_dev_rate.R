source("~/Rdev/setup.R")
x = 
"75 15 0.005144695
75 17.5 0.010803859
75 20 0.015819936
75 25 0.030482315
75 25 0.025016077
75 30 0.038135048
75 30 0.031446945
75 32.5 0.033504823
58 17.5 0.00977492
58 17.5 0.00977492
58 20 0.01318328
58 25 0.020192926
58 25 0.025401929
58 30 0.025659164
58 30 0.03022508
58 32.5 0.024630225
44 20 0.008810289
44 25 0.015691318
"
obs = strsplit(x, r"{\s}") |> unlist() |> as.numeric() |> matrix(ncol=3, byrow=TRUE) |> as.data.frame()
colnames(obs) = c("RH", "T", "DevRate")
obs

model = lm(DevRate ~ T, subset(obs, RH==75 & T <=30))
coef(model)


model = nls(DevRate ~ a*T*(T - T0)*sqrt(Tmax - T)/RH, data=obs, start=list(a=0.0015, T0=12, Tmax=35))
model

M = expand.grid(
  RH = c(44, 58, 75),
  T   = 5:45
)
M$DevRate = predict(model, M)
M$PDevRate[is.na(M$DevRate)] = 0

ggplot(obs, aes(T, DevRate, colour=factor(RH))) +
  geom_line(data=M) +
  geom_point(size=3)
  
model = function(rh) {
  M = nls(DevRate ~ a*T*(T - T0)*sqrt(Tmax - T), data=subset(obs, RH==rh), start=list(a=0.0015, T0=12, Tmax=35))
  data.frame(coef(M))
}
model(58)
