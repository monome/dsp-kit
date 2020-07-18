function plot_shape(name)
  x = dlmread(sprintf("../cmake-build-debug/%s.data", name));
  clf();
  plot(x);
  hold on;
  %grid on;
 
 % block size in samples
  n = 24000;
% show inflection points
  plot([n*1,n*1], [-140, 140]);
  plot([n*2,n*2], [-140, 140]);
  plot([n*3,n*3], [-140, 140]);
  ylim([-140, 140])

  print(sprintf("shape_plots/%s.png", name))
  
endfunction