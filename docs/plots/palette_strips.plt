# Generate palette strip visualization
set terminal pngcairo enhanced size 800,150
set output 'images/palette_strip.png'

set boxwidth 1 relative

set xrange [-1:5]
set yrange [0:1]
set xlabel ""
set ylabel ""
unset tics
unset border

# Create color rectangles (this requires the selected colors data)
# Note: This is a simplified version - you might need to create specific data for rectangles
plot 'data/selected_colors.dat' skip 1 using (column(0)):1:4 \
     with boxes lc rgb variable fs solid notitle
