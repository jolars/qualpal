# HSL Color Space Visualization
set terminal pngcairo enhanced size 700,600
set output 'images/hsl_sampling.png'

# 3D HSL cylindrical coordinates
set view 50, 30
set xlabel "Saturation × cos(Hue)" rotate parallel
set ylabel "Saturation × sin(Hue)" rotate parallel
set zlabel "Lightness" rotate parallel
set title "HSL Cylindrical Space"

# Convert HSL to cylindrical coordinates and plot
splot 'data/hsl_points.dat' using ($2*cos($1*pi/180)):($2*sin($1*pi/180)):3:4 \
      with points pt 7 ps 1 lc rgb variable notitle
