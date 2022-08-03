sim = read_output(output_file_name)
names(sim)[names(sim)=="date"] = "Date"

file_name = paste(box_script_folder, "env-AnGiang.txt", sep="/")
obs = read.table(file_name, header=TRUE, stringsAsFactors=FALSE)
obs$Date = as.POSIXct(ymd(obs$Date))
obs$Adult[obs$Adult==0] = NA

show_sim = function() {
  from = min(sim$Date)
  to   = max(sim$Date)
  M = subset(obs, Date>=from & Date<=to)
  M$Stage = (floor(M$Vegetative) + 2*floor(M$Reproductive) + 3*floor(M$Ripening))/(floor(M$Vegetative) + floor(M$Reproductive) + floor(M$Ripening))
  p1 = ggplot(sim) +
      geom_line (aes(x=Date, y=adult), colour="brown") +
      geom_line (aes(x=Date, y=Adult), colour="forestgreen", data=M) +
      geom_point(aes(x=Date, y=Adult), colour="forestgreen",data=M) 
  p2 = ggplot(sim) + geom_line (aes(x=Date, y=Tavg), colour="red")
  p3 = ggplot(sim) + geom_line (aes(x=Date, y=Sunshine), colour="orange")
  p4 = ggplot(sim) + geom_line (aes(x=Date, y=RH), colour="cyan")
  p5 = ggplot(sim) + geom_line (aes(x=Date, y=Rainfall), colour="blue")
  p1
}
print(show_sim())


