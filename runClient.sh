#!/bin/sh

clear
echo "Compile and Build Files ..."
make 
echo "Files Compiled completly with no Errors !"
echo "running client..."

if [ $# -eq 3 ] 
  then
   echo "connecting to Server On IP provided  and Port number !"
   ./client $1 $2 $3
  else 
    echo "No arguments supplied" 
    echo "Connected to local Host Server 127.0.0.1 on port 7777 "
    ./client 127.0.0.1  hello 7777
fi


