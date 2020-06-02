function plot_shape(name)
  path = sprintf("../cmake-build-debug/%s.data", name)
  x = dlmread(path);
  clf();
  plot(x);
  hold on;
  %grid on;
 
 % block size in samples
  n = 24000;
  ymin = -40;
  ymax = 140;
% show inflection points
  plot([n*1,n*1], [ymin, ymax]);
  plot([n*2,n*2], [ymin, ymax]);
  plot([n*3,n*3], [ymin, ymax]);
  ylim([ymin, ymax])

  print(sprintf("shape_plots/%s.png", name))
  
endfunction