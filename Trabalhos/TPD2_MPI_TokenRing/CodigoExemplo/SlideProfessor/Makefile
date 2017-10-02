

mpi_hello_world:
	mpicc -g -O0 -o main main.c

.PHONY: clean run

clean:
	-rm main

run:
	mpirun -np 6 -f hostfile_example.txt ./main