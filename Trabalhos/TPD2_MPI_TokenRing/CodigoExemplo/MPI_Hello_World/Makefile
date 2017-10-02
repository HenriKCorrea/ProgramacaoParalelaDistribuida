

mpi_hello_world:
	mpicc -g -O0 -o mpi_hello_world mpi_hello_world.c

.PHONY: clean run

clean:
	-rm mpi_hello_world
	#-rm *.o

run:
	mpirun -np 6 -f hostfile_example.txt ./mpi_hello_world