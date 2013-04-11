#!/bin/bash
#script for assembler
clear
g++ handling_macro.cpp -o handling_macro
./handling_macro
echo "after macro processing"
cat temp.txt
echo "macro processing done"
g++ asm.cpp -o asm
./asm
rm temp.txt
rm asm
rm handling_macro
 
