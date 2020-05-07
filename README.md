# lift-simulator
Simulates the operations of lifts servicing a building using multiple threads 
and processes (OS Assignment).

Known bugs:
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
