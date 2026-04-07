#!/bin/sh

echo "BUILDING PTYPER LINUX"

g++ -shared -fPIC -o "builds/PTyper-Linux.dll" "src/ptyper-linux.cpp"
g++ -g "src/driver.cpp" -o "builds/driver" "builds/PTyper-Linux.dll"

echo "FINISHED BUILDING PTYPER LINUX"