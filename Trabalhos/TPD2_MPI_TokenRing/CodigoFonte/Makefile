

mpi_send_receive_cpp:
	mpic++ -g -O0 -o main main.cpp

.PHONY: clean run run2

clean:
	-rm main

run:
	mpirun -np 6 -f hostfile_example.txt ./main

run2:
	mpirun -n 6 ./main
