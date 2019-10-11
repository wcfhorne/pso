set xrange [-5.0:5.0]
set yrange [-5.0:5.0]
set zrange [-10.0:70.0]
filedat = 'sphere.dat'
swarmdat = 'swarm.dat'
n = system(sprintf('cat %s | wc -l', swarmdat))
set term gif animate
set output 'output.gif'
do for [j=1:n] {
   set title 'time '.j
   #set label 'global ' at swarmdat u 13 every ::j::j w p 
   splot filedat, swarmdat u 2:3:4 every ::j::j, swarmdat u 5:6:7 every ::j::j, swarmdat u 8:9:10 every ::j::j, swarmdat u 11:12:13 every ::j::j w p pt 7 ps 2
}

