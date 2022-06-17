Run xmp project:
xmpcc rooms.c -o rooms
mpiexec -n 4 -f nodes ./rooms  | egrep -v '(context|handle)'
