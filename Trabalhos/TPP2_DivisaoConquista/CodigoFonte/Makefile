#
# Compiler flags
#
#CC     = mpic++
#CFLAGS = -Wall -Werror -Wextra
HOSTNAME = $(shell hostname)
ifeq ($(HOSTNAME), marfim)
CC = ladcomp -env mpiCC
else
CC = mpic++
endif

#
# Project files
#
SRCS = Main.cpp
OBJS = $(SRCS:.cpp=.o)
EXE  = Main
ifeq ($(shell echo $(HOSTNAME) | grep -o atlantica), atlantica)
CEXE = ladrun -np
else
CEXE = mpirun -n
endif


#
# Project flags - TPP2 Mestre Escravo
#
NUMBER_OF_PROCESS := 7
TAMANHO_VETOR := 100000
DELTA := 25000				#DELTA = TAMANHO_VETOR / NUMERO DE FOLHAS

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
#RELCFLAGS = -O3 -DNDEBUG

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

.PHONY: clean debug prep_dbg prep_rel release remake runrel runrel2 rundbg rundbg2 runbs1 runbs2 runbs3 runbs4 runbs5 runbs6 runbs7 runbs8 runbs9 runbs10 runqs1 runqs2 runqs3 runqs4 runqs5 runqs6 runqs7 runqs8 runqs9 runqs10

# Default build
all: debug release


#
# Debug rules
#
debug: prep_dbg $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: prep_rel $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

$(RELDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<


#
# Other rules
#
prep_dbg:
	@if test ! -d $(DBGDIR); then mkdir $(DBGDIR); fi	#If DBGDIR do not exists, create dir
	
prep_rel:
	@if test ! -d $(RELDIR); then mkdir $(RELDIR); fi	#If RELDIR do not exists, create dir

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)

runrel:
	$(CEXE) $(NUMBER_OF_PROCESS) $(RELEXE) $(TAMANHO_VETOR) $(DELTA)
	
runrel2:
	$(CEXE) $(NUMBER_OF_PROCESS) $(RELEXE) $(TAMANHO_VETOR) $(DELTA) -qsort

rundbg:
	$(CEXE) $(NUMBER_OF_PROCESS) $(DBGEXE) $(TAMANHO_VETOR) $(DELTA)
	
rundbg2:
	$(CEXE) $(NUMBER_OF_PROCESS) $(DBGEXE) $(TAMANHO_VETOR) $(DELTA) -qsort


runbs1:
	$(CEXE) 1 $(RELEXE) 100000 100000

runbs2:
	$(CEXE) 3 $(RELEXE) 100000 50000

runbs3:
	$(CEXE) 7 $(RELEXE) 100000 25000

runbs4:
	$(CEXE) 15 $(RELEXE) 100000 12500

runbs5:
	$(CEXE) 31 $(RELEXE) 100000 6250

runbs6:
	$(CEXE) 1 $(RELEXE) 1000000 1000000

runbs7:
	$(CEXE) 3 $(RELEXE) 1000000 500000

runbs8:
	$(CEXE) 7 $(RELEXE) 1000000 250000

runbs9:
	$(CEXE) 15 $(RELEXE) 1000000 125000

runbs10:
	$(CEXE) 31 $(RELEXE) 1000000 62500


runqs1:
	$(CEXE) 1 $(RELEXE) 100000 100000 -qsort

runqs2:
	$(CEXE) 3 $(RELEXE) 100000 50000 -qsort

runqs3:
	$(CEXE) 7 $(RELEXE) 100000 25000 -qsort

runqs4:
	$(CEXE) 15 $(RELEXE) 100000 12500 -qsort

runqs5:
	$(CEXE) 31 $(RELEXE) 100000 6250 -qsort

runqs6:
	$(CEXE) 1 $(RELEXE) 1000000 1000000 -qsort

runqs7:
	$(CEXE) 3 $(RELEXE) 1000000 500000 -qsort

runqs8:
	$(CEXE) 7 $(RELEXE) 1000000 250000 -qsort

runqs9:
	$(CEXE) 15 $(RELEXE) 1000000 125000 -qsort

runqs10:
	$(CEXE) 31 $(RELEXE) 1000000 62500 -qsort


#
#Acessar Sparta
#
#ssh portoalegre\\14104112@sparta.pucrs.br

#
#Acessar Marfim (LAD)
#
#ssh ppd59010@marfim.lad.pucrs.br

#
#Compilar código
#
#ladcomp -env mpiCC <codigo fonte>
