# Selected Colors in DIN99d Space
set terminal pngcairo enhanced size 800,600
set output 'images/selected_colors.png'

set view 60, 30
set xlabel "L99d" rotate parallel
set ylabel "a99d" rotate parallel
set zlabel "b99d" rotate parallel
unset key

# Plot all colors (faded) and selected colors (highlighted)
splot 'data/all_colors.dat' using 1:2:3:($5==0?$4:1/0) \
      with points pt 7 ps 0.2 lc rgb variable title "Candidate Colors", \
      'data/selected_colors.dat' using 1:2:3:4 \
      with points pt 7 ps 1.5 lc rgb variable lw 2 title "Selected Colors"
