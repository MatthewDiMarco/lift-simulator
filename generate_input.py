import random

# constants
FILE = "sim_input.csv"
MIN_REQS = 50
MAX_REQS = 100
MIN_FLOOR = 1
MAX_FLOOR = 20

# generate the number of requests
nRequests = random.randint(MIN_REQS, MAX_REQS)

# randomly assign requests
f = open(FILE, 'w')
for req in range(nRequests):
    sFlr,dFlr = 1,1
    while (sFlr == dFlr): # requests should not be to same floor
        sFlr = random.randint(MIN_FLOOR, MAX_FLOOR) # start floor
        dFlr = random.randint(MIN_FLOOR, MAX_FLOOR) # destination floor
    f.write('{} {}\n'.format(sFlr, dFlr))
    
f.close()