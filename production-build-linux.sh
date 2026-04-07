#!/bin/sh

echo "BUILDING PTYPER LINUX"

g++ -O3 -shared -fPIC -o "builds/release/PTyper-Linux.dll" "src/ptyper-linux.cpp"

echo "FINISHED BUILDING PTYPER LINUX"