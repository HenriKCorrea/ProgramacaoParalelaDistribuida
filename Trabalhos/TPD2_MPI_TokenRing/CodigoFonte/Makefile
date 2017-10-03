

mpi_send_receive_cpp: Main.o MasterProcess.o SlaveProcess.o
	mpic++ -o Main Main.o MasterProcess.o SlaveProcess.o


Main.o: MasterProcess.o SlaveProcess.o
	mpic++ -g -O0 -c Main.cpp

MasterProcess.o:
	mpic++ -g -O0 -c MasterProcess.cpp

SlaveProcess.o:
	mpic++ -g -O0 -c SlaveProcess.cpp

.PHONY: clean run run2

clean:
	-rm Main
	-rm *.o

run:
	mpirun -np 6 -f hostfile_example.txt ./Main

run2:
	mpirun -n 6 ./Main
