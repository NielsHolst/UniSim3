setwd("C:/Users/au152367/Documents/PAPERS/2021/aphids-fungi/sim")
source("setup.R")

S_sum = subset(S, Input=="Sum")
S_sum$MeanLowerDiff = S_sum$EffectMean - S_sum$LowerPercentile 
S_sum$MeanHigherDiff = S_sum$HigherPercentile - S_sum$EffectMean 

f = function(s) {
  data.frame(
    Si = s$EffectMean[2], 
    SiConf = s$MeanLowerDiff[2],
    STi = s$EffectMean[1], 
    STiConf = s$MeanLowerDiff[1]    
  )
}
print(ddply(S_sum, .(Output), f))
