# Color Space Progression: RGB → DIN99d
set terminal pngcairo enhanced size 1500,500
set output 'images/color_space_progression.png'

set multiplot layout 1,3 title "Color Space Progression"

# RGB Space
set view 70, 30
set xlabel "R" rotate parallel
set ylabel "G" rotate parallel
set zlabel "B" rotate parallel
set xtics 50
set ytics 50
set ztics 50
set autoscale
set title "RGB Color Space"

splot 'data/rgb_points.dat' using 1:2:3:7 \
      with points pt 7 ps 0.4 lc rgb variable notitle

# XYZ Space (approximation using RGB transformation)
set xlabel "X" rotate parallel
set ylabel "Y" rotate parallel
set zlabel "Z" rotate parallel
set title "XYZ Color Space"

splot 'data/rgb_points.dat' using (0.4124*$1+0.3576*$2+0.1805*$3):(0.2126*$1+0.7152*$2+0.0722*$3):(0.0193*$1+0.1192*$2+0.9505*$3):7 \
      with points pt 7 ps 0.4 lc rgb variable notitle

# DIN99d Space
set xlabel "L99d" rotate parallel
set ylabel "a99d" rotate parallel
set zlabel "b99d" rotate parallel
set xtics 20
set ytics 20
set ztics 20
set title "DIN99d Color Space"

splot 'data/din99d_points.dat' using 1:2:3:4 \
      with points pt 7 ps 0.4 lc rgb variable notitle
