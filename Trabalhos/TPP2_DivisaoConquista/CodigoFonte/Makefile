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
ifeq ($(HOSTNAME), atlantica*)
CEXE = ladrun -np
TESTE = FARINHA
else
CEXE = mpirun -n
TESTE = Nao_Tem_Farinha
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
RELCFLAGS = -O3 -DNDEBUG

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

.PHONY: clean debug prep_dbg prep_rel release remake runrel runrel2 rundbg rundbg2

# Default build
all: debug release
	@echo $(TESTE)

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