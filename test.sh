#!/bin/bash
make a; make b;
while [ true ]
do
    python3 generate_input.py
    bufSize=$(( $RANDOM % 20 + 1 )); echo "new size: " $bufSize
    ./lift_sim_A $bufSize 0
    ./lift_sim_B $bufSize 0
done