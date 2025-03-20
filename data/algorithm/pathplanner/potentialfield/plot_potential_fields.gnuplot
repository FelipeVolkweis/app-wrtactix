set terminal pngcairo size 1600,1200 enhanced font 'Verdana,12'
set output 'potential_fields.png'

set title "Potential Fields Navigation Algorithm"

set xrange [0:9]
set yrange [0:6]

set size ratio -1

set grid

first_point = system("head -n 1 path.dat")
last_point = system("tail -n 1 path.dat") 

y_start = real(word(first_point, 2))  
x_start = real(word(first_point, 1))  
x_goal = real(word(last_point, 1))    
y_goal = real(word(last_point, 2))    

set label "Start" at x_start,y_start point pt 7 ps 2 lc rgb "blue"
set label "Goal" at x_goal,y_goal point pt 7 ps 2 lc rgb "green"

R = 0.09  # Replace with your obstacle radius
set style circle radius R
plot 'obstacles.dat' using 1:2 with circles lc rgb "red" notitle, \
     'path.dat' using 1:2 every ::1::(system("wc -l < path.dat")-2) with lines lc rgb "blue" title "Path"