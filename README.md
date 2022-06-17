# Project for parallel programming
Room assignment problem is solved using simulated annealing. The same problem is solved in two technologies MPI and XMP.
## Run xmp project:
xmpcc rooms.c -o rooms
<br />
mpiexec -n 4 -f nodes ./rooms  | egrep -v '(context|handle)'
