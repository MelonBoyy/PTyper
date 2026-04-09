#!/bin/sh

echo "BUILDING PTYPER LINUX"

g++ -O3 -shared -fPIC -o "builds/release/PTyper.dll" "src/ptyper.cpp"
g++ -O3 -shared -fPIC -o "builds/release/PTyper-Threaded.dll" "src/ptyper-threaded.cpp"

echo "FINISHED BUILDING PTYPER LINUX"