# Load your S data
S_data_file = "~/sites/ecolmod3/download/aphid-biocontrol-S.Rdata"

# Output folder
output_folder = "~/QDev/UniSim3/output"

# Here goes
load(S_data_file)
dim(S)

# Collect data
S_sum = subset(S, Input=="Sum")
S_sum$MeanLowerDiff = S_sum$EffectMean - S_sum$LowerPercentile 
S_sum$MeanHigherDiff = S_sum$HigherPercentile - S_sum$EffectMean 

f = function(s) {
  data.frame(
    Si = s$EffectMean[2], 
    SiLower  = s$MeanLowerDiff[2],
    SiHigher = s$MeanHigherDiff[2],
    STi = s$EffectMean[1], 
    STiLower  = s$MeanLowerDiff[2],
    STiHigher = s$MeanHigherDiff[2]
  )
}
M = ddply(S_sum, .(Output), f)
M

# Write table
if (!dir.exists(output_folder)) dir.create(output_folder, recursive=TRUE)
file_name_path = paste0(output_folder, "/table-2.txt")
print(paste("Writing table to", file_name_path))
write.table(M, file_name_path, quote=FALSE, sep="\t", row.name=FALSE)
