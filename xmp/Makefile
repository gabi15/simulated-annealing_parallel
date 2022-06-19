define INFO_MSG

You can call make with targets:

all
  Creates from  rooms.c executable file called rooms.o

nodes
  Creates file named nodes with the list of available compute nodes

run
  This target runs the program.
  You can specify args:

    nodes_number (default value: 2, minimum value: 2)
      Number of nodes to be run.
	  example: make run nodes_number=3

    nodes (default value: nodes)
      Name of the file with node list. You can create one by running script: make nodes

	dislike_array
		dislike array of people, space separated floats

	assignment_array
		room assignment array, index is person, number is a room, space separated

	example:
	make run nodes_number=9 dislike_array=d_example.dat assignment_array=a_example.dat
info
  Prints info about makefile usage.

endef

XMPCC=/opt/nfs/mpich-3.2/bin/mpicc
MPIEXEC=/opt/nfs/mpich-4.0.1/bin/mpiexec
LD_LIBRARY_PATH=/opt/nfs/mpich-4.0.1/lib

LIBS=-l m

# Defaults
nodes_number=2
nodes=nodes
dislike_array=""
assignment_array=""

NAME = rooms.o
SRC=rooms.c

SHELL = /bin/bash

all: $(NAME)

$(NAME):
	source /opt/nfs/config/source_omni134_xmp.sh && xmpcc $(SRC) $? -o $@ $(LIBS)


.PHONY: clean run nodes info

clean:
	rm -f $(NAME)

clean_nodes:
	rm -f ./nodes

run: $(NAME)
	source /opt/nfs/config/source_omni134_xmp.sh && mpiexec -f $(nodes) -n $(nodes_number) ./$(NAME) $(dislike_array) $(assignment_array) | egrep -v '(context|handle)'

nodes:
	@:$(shell /opt/nfs/config/station206_name_list.sh 1 16 > nodes)

info:
	@:$(info $(INFO_MSG))
