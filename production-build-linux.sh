#!/bin/sh

echo "BUILDING PTYPER LINUX"

g++ -O3 -shared -fPIC -o "builds/release/PTyper.dll" "src/ptyper.cpp"
g++ -O3 -shared -fPIC -o "builds/release/PTyper-Threaded.dll" "src/ptyper-threaded.cpp"
g++ -g "src/driver.cpp" -o "builds/release/driver" "builds/release/PTyper.dll" "builds/release/PTyper-Threaded.dll"

echo "FINISHED BUILDING PTYPER LINUX"