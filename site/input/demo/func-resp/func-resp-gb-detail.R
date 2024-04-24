my_plot = function() {

  f = function(slope, asymp, x) {
    asymp*(1-exp(-slope*x/asymp))
  }

  slope = 0.2
  asymp = 20
  x = 0:500

  M = data.frame(X=x, Curve=f(slope,asymp,x), Line=slope*x)
  M$Line[M$Line>12] = NA

  M = melt(M, measure.vars=c("Curve", "Line"))

  my_theme =  theme(legend.position="none") + 
              theme(panel.grid.major.x = element_blank()) +
              theme(panel.grid.major.y = element_blank()) +
              theme(axis.text.x = element_blank(), axis.ticks.x = element_blank()) +
              theme(axis.text.y = element_blank(), axis.ticks.y = element_blank()) 

  P = ggplot(M, aes(x=X, y=value, colour=variable)) +
    geom_line() +
    geom_hline(yintercept=asymp, colour=unisim_colours[3], size=1) + 
    my_theme +
    scale_x_continuous(expand=c(0,0)) +
    scale_y_continuous(expand=c(0,0), limits=c(0,asymp+1)) 

  Text1 = data.frame(
    X = c(25,15), Y = c(2,19),
    Label = c("s", "Delta*D")
  )

  P1 = 
    P + labs(x=expression("Prey density ("*X*")"), y=expression("Prey killed ("-Delta*X*")")) +
    geom_text(data=Text1, aes(x=X, y=Y, label=Label), colour="black" , parse="TRUE", hjust="left") 
    
  Text2 = data.frame(
    X = c(25,15), Y = c(2,19),
    Label = c("alpha", "100*'%'")
  )
    
  P2 = P + labs(x=expression("Search effort ("*alpha*Y*Delta*t*")"), y=expression("Search efficacy ("*s*")")) +
  geom_text(data=Text2, aes(x=X, y=Y, label=Label), colour="black" , parse="TRUE", hjust="left")
  
  grid.arrange(P2, P1, ncol=2)
}

x11(6,3)
print(my_plot())

figures <- function(df) {
  Pages = list(Page = list(Grob=my_plot(), Width=6, Height=3))
}

  
