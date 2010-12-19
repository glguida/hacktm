set key off
set ticslevel 0
set size square
set multiplot layout 2,2 title "Column [10,10]"
set xrange [0:1000]
set yrange [0:1000]
set xlabel "Input Space"
set zlabel "Perm" rotate left
set title "(x,y) view"
plot "showDendrites.data" index 210
set yrange [*:*]
set title "(x,perm) view"
plot "showDendrites.data" index 210 using 1:3 
set title "(y,perm) view"
plot "showDendrites.data" index 210 using 2:3 
set title "(x,y,perm) view"
set yrange [0:1000]
splot "showDendrites.data" index 210
unset multiplot
pause -1