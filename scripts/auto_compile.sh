#!/usr/bin/bash

cd ~/Documents/OOPDS-assignment || exit

find ./*.cpp ./*.h config.txt | entr -c -r ./scripts/compile.sh
