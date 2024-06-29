#!/usr/bin/bash

g++ test.cpp -o "test.out" 2>&1 | tee ./output/test_output.txt || exit
./test.out
