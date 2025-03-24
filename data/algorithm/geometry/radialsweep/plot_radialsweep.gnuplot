set terminal pngcairo size 1600,1200 enhanced font 'Verdana,12'
set output 'radialsweep.png'

set title "RadialSweep Algorithm"

# Set the range to represent the field from (-4.5, -3) to (4.5, 3)
set xrange [-4.5:4.5]
set yrange [-3:3]

set size ratio -1

# Read the start and goal points from angles.dat
observer = system("head -n 1 angles.dat")
config = system("head -n 2 angles.dat | tail -n 1")  # Get just the config line

x = real(word(observer, 1))
y = real(word(observer, 2))
radius = real(word(config, 3))

# Set labels for observer
set label "Observer" at x,y point pt 7 ps 2 lc rgb "blue"

R = 0.09  # Obstacle radius

do for [i=3:system("wc -l < angles.dat")] {
    line = system(sprintf("sed -n '%dp' angles.dat", i))
    angle_start = real(word(line, 1))
    angle_end = real(word(line, 2))
    set obj i circle at x, y size radius arc [angle_start:angle_end] fc "web-green" nowedge
}

# Draw semi-circles for each arc
do for [i=3:system("wc -l < angles.dat")] {
    line = system(sprintf("sed -n '%dp' angles.dat", i))
    angle_start = real(word(line, 1))
    angle_end = real(word(line, 2))
    if (angle_end < angle_start) {
        angle_end = angle_end + 360
    }
    mid_angle = (angle_start + angle_end) / 2
    set obj i+200 circle at (x),(y) size radius arc [angle_start:angle_end] fc rgb "yellow" fs solid 0.5 border lc rgb "yellow"
}
# Main plot command
plot 'obstacles.dat' using 1:2:(R) with circles lc rgb "red" notitle, \