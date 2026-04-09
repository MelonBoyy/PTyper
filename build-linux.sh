#!/bin/sh

echo "BUILDING PTYPER LINUX"

g++ -shared -fPIC -o "builds/PTyper.dll" "src/ptyper.cpp"
g++ -shared -fPIC -o "builds/PTyper-Threaded.dll" "src/ptyper-threaded.cpp"
g++ -g "src/driver.cpp" -o "builds/driver" "builds/PTyper.dll" "builds/PTyper-Threaded.dll"

echo "FINISHED BUILDING PTYPER LINUX"