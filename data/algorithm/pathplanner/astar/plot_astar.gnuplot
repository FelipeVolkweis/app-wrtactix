set terminal pngcairo size 1600,1200 enhanced font 'Verdana,12'
set output 'a_star.png'

set title "A* Navigation Algorithm"

# Set the range to represent the field from (-4.5, -3) to (4.5, 3)
set xrange [-4.5:4.5]
set yrange [-3:3]

set size ratio -1

set grid

# Read the grid dimensions and conversion factor from grid.dat
grid_data = system("head -n 3 grid.dat")
width = real(word(grid_data, 1))  # 900
height = real(word(grid_data, 2)) # 600
conversionFactor = real(word(grid_data, 3)) # 100

# Calculate the grid cell size in real-world coordinates
cell_width = 9.0 / width  # 0.01 m
cell_height = 6.0 / height # 0.01 m

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

# Plot the grid as black squares for obstacles
plot 'grid.dat' every ::3 using (($1-1)*cell_width-4.5):(($2-1)*cell_height-3):(cell_width):(cell_height) with boxes lc rgb "black" notitle, \
     'obstacles.dat' using 1:2 with circles lc rgb "red" notitle, \
     'path.dat' every ::2 using 1:2 with lines lc rgb "blue" title "Path"