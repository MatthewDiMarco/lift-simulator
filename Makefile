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
OBJ 	= fileio.o linked_list.o buffer.o
OBJT	= test_linked_list.o test_buffer.o
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

buffer.o : buffer.c buffer.h linked_list.h
	$(CC) buffer.c -c $(FLAGS)

linked_list.o : linked_list.c linked_list.h
	$(CC) linked_list.c -c $(FLAGS)

# test compilation

tests : linked_list.o buffer.o $(OBJT)
	$(CC) linked_list.o test_linked_list.o -o test_linked_list
	$(CC) buffer.o test_buffer.o -o test_buffer

test_linked_list.o : test_linked_list.c linked_list.c linked_list.h
	$(CC) test_linked_list.c -c $(FLAGS)

test_buffer.o : test_buffer.c buffer.c buffer.h linked_list.h
	$(CC) test_buffer.c -c $(FLAGS)

# execution

runa :
	./$(EXECA) $(BUF) $(DELAY) 

runaval :
	valgrind ./$(EXECA) $(BUF) $(DELAY) 

runb :
	./$(EXECB) $(BUF) $(DELAY) 

runbval :
	valgrind ./$(EXECB) $(BUF) $(DELAY) 

runtests :
	valgrind --leak-check=full ./test_linked_list
	valgrind --leak-check=full ./test_buffer

clean :
	rm -f $(EXECA) $(EXECB) test_linked_list test_buffer *.o
	