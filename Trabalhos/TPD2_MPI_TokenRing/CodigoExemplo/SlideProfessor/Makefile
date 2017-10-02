

mpi_send_receive:
	mpicc -g -O0 -o main main.c

.PHONY: clean run run2

clean:
	-rm main

run:
	mpirun -np 6 -f hostfile_example.txt ./main

run2:
	mpirun -n 6 ./main
