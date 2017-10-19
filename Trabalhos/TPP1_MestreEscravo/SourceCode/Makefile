

application: Main.o MasterProcess.o SlaveProcess.o
	mpic++ -o Main Main.o MasterProcess.o SlaveProcess.o


Main.o: MasterProcess.o SlaveProcess.o
	mpic++ -g -O0 -c Main.cpp

MasterProcess.o:
	mpic++ -g -O0 -c MasterProcess.cpp

SlaveProcess.o:
	mpic++ -g -O0 -c SlaveProcess.cpp

.PHONY: clean run

clean:
	-rm Main
	-rm *.o
	-rm *.txt
	-rm LogOutput/*.txt

run:
	mpirun -n 6 ./Main
	
