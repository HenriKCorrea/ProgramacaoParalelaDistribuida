NUMBER_OF_PROCESS := 6
TAREFAS := 5
ARRAY_SIZE := 40

application: Main.o SacoDeTrabalho.o #MasterProcess.o SlaveProcess.o
	mpic++ -o Main Main.o SacoDeTrabalho.o #MasterProcess.o SlaveProcess.o


Main.o: #MasterProcess.o SlaveProcess.o
	mpic++ -g -O0 -c Main.cpp

SacoDeTrabalho.o:
	mpic++ -g -O0 -c SacoDeTrabalho.cpp	

# MasterProcess.o:
# 	mpic++ -g -O0 -c MasterProcess.cpp

# SlaveProcess.o:
# 	mpic++ -g -O0 -c SlaveProcess.cpp

.PHONY: clean run

clean:
	-rm Main
	-rm *.o
	#-rm *.txt
	#-rm LogOutput/*.txt

run:
	mpirun -n $(NUMBER_OF_PROCESS) ./Main $(ARRAY_SIZE) $(TAREFAS)
	
