#!/usr/bin/bash

g++ main.cpp -o "main.out" 2>&1 | tee ./output/main_output.txt || exit
echo "config.txt" | ./main.out
