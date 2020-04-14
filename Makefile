##
 # FILE: 		Makefile
 # AUTHOR:		Matthew Di Marco
 # CREATED:		11/04/20
 # -----
 # MODIFIED:	14/04/20
 # BY			Matthew Di Marco
 # -----
 # PURPOSE:		Makefile for implementation for Lift Simulator.
##

# variables

CC 		= gcc
FLAGS 	= -std=c99 -Wall -Werror
OBJ 	= fileio.o linked_list.o
OBJA 	= lift_sim_A.o
OBJB 	= lift_sim_B.o
EXECA 	= lift_sim_A
EXECB 	= lift_sim_B

BUF 	= 10
DELAY	= 1

# debug conditional compilation

ifdef DEBUG
FLAGS += -g
DEBUG : clean $(EXEC)
endif

# production code compilation

a : $(OBJA) $(OBJ)
	$(CC) -pthread $(OBJA) $(OBJ) -o $(EXECA)

b : $(OBJB) $(OBJ)
	$(CC) -pthread $(OBJB) $(OBJ) -o $(EXECB)

lift_sim_A.o : lift_sim_A.c lift_sim.h fileio.h linked_list.h
	$(CC) lift_sim_A.c -c $(FLAGS)

lift_sim_B.o : lift_sim_B.c lift_sim.h fileio.h linked_list.h
	$(CC) lift_sim_B.c -c $(FLAGS)

fileio.o : fileio.c fileio.h linked_list.h
	$(CC) fileio.c -c $(FLAGS)

linked_list.o : linked_list.c linked_list.h
	$(CC) linked_list.c -c $(FLAGS)

# test compilation

# clean and execution

runa :
	./$(EXECA) $(BUF) $(DELAY) 

runb :
	./$(EXECB) $(BUF) $(DELAY) 

clean :
	rm -f $(EXECA) $(EXECB) *.o