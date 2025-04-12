set terminal pngcairo size 1600,1200 enhanced font 'Verdana,12'
set output 'uvf.png'

set title "UVF Navigation Algorithm"

# Set the range to represent the field from (-4.5, -3) to (4.5, 3)
set xrange [-4.5:4.5]
set yrange [-3:3]

set size ratio -1

# Read the start and goal points from path.dat
first_point = system("head -n 1 path.dat")
second_point = system("head -n 2 path.dat | tail -n 1")

x_start = real(word(first_point, 1))
y_start = real(word(first_point, 2))
x_goal = real(word(second_point, 1))
y_goal = real(word(second_point, 2))

# Set labels for start and goal points
set label "Start" at x_start,y_start point pt 7 ps 2 lc rgb "blue"
set label "Goal" at x_goal,y_goal point pt 7 ps 2 lc rgb "green"

R = 0.09  # Obstacle radius

# Function to plot rectangles
plot 'obstacles.dat' using 1:2:(R) with circles lc rgb "red" notitle, \
     'path.dat' every ::2 using 1:2 with lines lc rgb "blue" title "Path"