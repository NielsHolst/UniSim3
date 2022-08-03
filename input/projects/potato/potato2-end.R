library(RColorBrewer) 
library(scales)       
my_colours = brewer_pal('qual',2)(3)

sim = read_output(output_file_name)
P = ggplot(sim, aes(x=PestFecundity.end, y=content.end, shape=factor(CropDevRate.end),colour=factor(Parasitoids.end))) +
  scale_colour_manual(values=my_colours) +
  scale_shape_manual(labels=c("Late", "Early"), values = 16:17) +
  geom_abline(colour="orange", size=1) +
  geom_point(size=4) +
  geom_line(size=1) +
  labs(x="Pest fecundity (R0)", y="Hibernating adults", colour="Parasitoids", shape="Crop maturation") +
  ggplot_theme(14) +
  facet_wrap(~Year.end)
print(P)
