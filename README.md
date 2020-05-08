# lift-simulator
Simulates and handles the synchronous operations of lifts servicing a building.

## Description
There are two implementations of the software -- lift_sim_A.c and lift_sim_B.c. A was implemented with threads, while B uses System Calls like fork() to create and manage multiple processes. 4 threads/processes are executing simultaneously in both versions. One is adding new requests from an input file to a buffer, the other 3 are lifts, extracting those requests and serving them. 

## Compilation
Each version can be compiled individually with the commands "make a" and "make b". To install Makefile on your system, try entering: 
```bash
sudo apt install make
```

## Execution
The resulting executables can be run by entering:
```bash
./lift_sim_X <buffer_size> <lift_delay> <optional_input.csv>
```
where 'X' can be replaced with A or B. Also note that the programs expect a file inside the same directory called "sim_input.csv" which contains 1 request per line of the form "[start_floor] [destination_floor]", and can accomodate 50-100 lines. But, another input file can be specified at the command line if desired.
Alternatively, the programs can be executed with the make file rules "make runa" and "make runb" (use "make runxval or make runxhel to execute the programs with Valgrind/Helgrind respectfully, where x = a or b").

# Examples

## Input
Some example input files can be found inside the "sample_files" directory, but if a more diverse range of files are required, run the "generate_input.py" (Dependencies: Python 3.6.9).
E.g.
```
7 5
16 8
17 2
1 10
7 1
11 1
...
```

## Output
Each new request and lift operation is logged to an output file named "sim_out.csv". The output will resemble:
```
...

------------------------------------------
New Lift Request From Floor 18 to Floor 3
Request No: 13
------------------------------------------

Lift-2 Operation
Previous position: Floor 8
Request: Floor 1 to 10
Detail operations:
    Go from Floor 8 to Floor 1
    Go from Floor 1 to Floor 10
    #movements for this request: 16
    #request: 2
    Total #movement: 39
Current position: Floor 10

...
```
In the sample_files/ directory, the outputs correspond like such:
sim_out1.csv was run using sim_input1.csv with a buffer size of 10
sim_out2.csv was run using sim_input2.csv with a buffer size of 5
sim_out3.csv was run using sim_input3.csv with a buffer size of 20

## Known Bugs
```
    Bug #1: Occasional random freezing with implementation B
        Fault(s):   Halfway through the sim, the program will randomly freeze.
        Replicate:  Hard to replicate, as it only happens sometimes. Rerun with
                    lift delay 0 a few times and it should occur.
        Status:     FIXED
        Cause:      The problem was a sync issue caused by a logic error in the
                    request() function. I had written: 
                    
                        wait(empty);
                        wait(mutex);
                        ...
                        post(mutex);
                        post(full);
                        
                    when i'd ought to release the mutex after signalling 'full',
                    and not before.
                    Basically I was accessing shared memory (full semaphore)
                    after releasing the lock, breaking the synchronous harmony 
                    between the processes.
                    
    Bug #2: Buffer size is set to size from previous execution (imp. B)
        Fault(s):   More requests are getting into the buffer at once, exceeding
                    the capacity than should be allowed.
        Replicate:  run the program with a buffer size x (say 10). The sim will
                    print the requests added to the buffer in order. After the
                    sim finishes, rerun with a different buffer size (say 1).
                    As indicated by the print statements, there will appear
                    long streaks of "NEW REQUEST" (upto the amount of the 
                    previous execution -- 10), showing that the buffer has
                    exceeded it's capacity.
        Status:     FIXED
        Cause:      In lift_sim_B.c, at line 95, the empty semaphore was being
                    initialized to shm->buffer->capacity. The problem is that 
                    the shared memory buffer wasn't initialised until after
                    this line of code. So the sem was being allocated the value
                    that was previously stored in that address space, before 
                    anything was initialised in it for this particular run. That
                    explains why the buffer size was delayed an execution, and 
                    why the buffer was exceeding it's capacity.
                    My simple fix was to init the sem with the imported 
                    buffer size from the command line.
```