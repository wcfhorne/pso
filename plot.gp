set xrange [-5.0:5.0]
set yrange [-5.0:5.0]
set zrange [-10.0:70.0]
spheredat = 'sphere.dat'
sphereopt = 'sphere-opt.dat'
n = system(sprintf('cat %s | wc -l', sphereopt))
set term gif animate
set output 'sphere-map.gif'
do for [j=1:n] {
   set title 'time '.j
   #set label 'global ' at swarmdat u 13 every ::j::j w p 
   splot spheredat, sphereopt u 2:3:4 every ::j::j, sphereopt u 5:6:7 every ::j::j, sphereopt u 8:9:10 every ::j::j, sphereopt u 11:12:13 every ::j::j w p pt 7 ps 2
}

set xrange [-5.2:5.2]
set yrange [-5.2:5.2]
set zrange [-10.0:70.0]
rastigandat = 'rastigan.dat'
rastiganopt = 'rastigan-opt.dat'
n = system(sprintf('cat %s | wc -l', rastiganopt))
set term gif animate
set dgrid3d 75,75 qnorm 8
set view 70,45
set output 'rastigan-map.gif'
do for [j=1:n] {
   set title 'time '.j
   splot rastigandat w lines, rastiganopt u 2:3:4 every ::j::j, rastiganopt u 5:6:7 every ::j::j, rastiganopt u 8:9:10 every ::j::j, rastiganopt u 11:12:13 every ::j::j pt 7 ps 2
}

