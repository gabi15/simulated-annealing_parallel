define INFO_MSG

You can call make with targets:

all
  Creates from  room_assignment.c executable file called room_assignment

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

MPICC=/opt/nfs/mpich-3.2/bin/mpicc
MPIEXEC=/opt/nfs/mpich-3.2/bin/mpiexec

LIBS=-lm

# Defaults
nodes_number=2
nodes=nodes
dislike_array=""
assignment_array=""

NAME = room_assignment
SRC=room_assignment.c

all: $(NAME)

$(NAME):
	$(MPICC) $(SRC) $? -o $@ $(LIBS)


.PHONY: clean run nodes info

clean:
	rm -f $(NAME)

clean_nodes:
	rm -f ./nodes

run: $(NAME)
	$(MPIEXEC) -f $(nodes) -n $(nodes_number) ./$(NAME) $(dislike_array) $(assignment_array)

nodes:
	@:$(shell /opt/nfs/config/station206_name_list.sh 1 16 > nodes)

info:
	@:$(info $(INFO_MSG))
