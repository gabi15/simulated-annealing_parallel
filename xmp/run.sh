source /opt/nfs/config/source_omni134_xmp.sh
# source /opt/nfs/config/source_omni2020.sh
# source /opt/nfs/config/source_omni132.sh

xmpcc rooms.c -o rooms.o


mpiexec -n 8 -f nodes ./rooms.o | egrep -v '(context|handle)'
# mpiexec -n 8 -f nodes ./rooms.o
# mpiexec -n 8 -f nodes ./rooms.o d_example.dat a_example.dat