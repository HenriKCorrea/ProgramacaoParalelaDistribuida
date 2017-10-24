NUMBER_OF_PROCESS := 6
TAREFAS := 5
ARRAY_SIZE := 40

application: Main.o SacoDeTrabalho.o 
	mpic++ -o Main Main.o SacoDeTrabalho.o 


Main.o: 
	mpic++ -g -O0 -c Main.cpp

SacoDeTrabalho.o:
	mpic++ -g -O0 -c SacoDeTrabalho.cpp	

.PHONY: clean run run2

clean:
	-rm Main
	-rm *.o


run:
	mpirun -n $(NUMBER_OF_PROCESS) ./Main $(ARRAY_SIZE) $(TAREFAS)
	
run2:
	mpirun -n $(NUMBER_OF_PROCESS) ./Main $(ARRAY_SIZE) $(TAREFAS) qsort