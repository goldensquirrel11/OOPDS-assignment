#!/usr/bin/bash

cd ~/Documents/OOPDS-assignment || exit

find ./*.cpp ./*.h | entr -c -r ./scripts/compile_test.sh