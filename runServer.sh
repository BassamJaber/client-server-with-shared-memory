#!/bin/sh
clear
echo "Compile and Build Files ..."
make 
echo "Files Compiled completly with no Errors !"
echo "running Server ..."

if [ $# -eq 0 ] 
  then
    echo "No arguments supplied" 
    echo "Create Welcome Socket on Port 7777"
    ./server 7777
  else 
   echo "Create a Socket on provided Port Number"
   ./server $1	
fi

