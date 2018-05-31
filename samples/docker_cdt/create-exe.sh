#!/bin/sh
set -ex

CONTAINER="docker_test"

mkdir -p $PWD/usr/bin
../../bin/creator ../../bin/proxy $PWD/usr/bin/cmake.exe \
    "docker exec -i $CONTAINER sh -c \"cd #PWD# && exec /usr/bin/cmake #ARGS#\"" -q -i \
    -f "$PWD/usr/include" "/usr/include" \
    -t "/usr/include" "$PWD/usr/include" \
    -f "$PWD/usr/lib" "/usr/lib" \
    -t "/usr/lib" "$PWD/usr/lib" \
    -f "\\" "/" \
    -f "C:" "/c" #-l $PWD/log.txt
    
chmod +x "$PWD/usr/bin/cmake.exe"

../../bin/creator ../../bin/proxy $PWD/usr/bin/make.exe \
    "docker exec -i $CONTAINER sh -c \"cd #PWD# && exec /usr/bin/make #ARGS#\"" -q -i \
    -f "$PWD/usr/include" "/usr/include" \
    -t "/usr/include" "$PWD/usr/include" \
    -f "$PWD/usr/lib" "/usr/lib" \
    -t "/usr/lib" "$PWD/usr/lib" \
    -f "\\" "/" -f "C:" "/c"
chmod +x "$PWD/usr/bin/make.exe"

../../bin/creator ../../bin/proxy $PWD/usr/bin/gcc.exe \
    "docker exec -i $CONTAINER sh -c \"cd #PWD# && exec /usr/bin/gcc #ARGS#\"" -q -i \
    -f "$PWD/usr/include" "/usr/include" \
    -t "/usr/include" "$PWD/usr/include" \
    -f "$PWD/usr/lib" "/usr/lib" \
    -t "/usr/lib" "$PWD/usr/lib" \
    -f "\\" "/" \
    -f "C:" "/c"
chmod +x "$PWD/usr/bin/gcc.exe"

../../bin/creator ../../bin/proxy $PWD/usr/bin/g++.exe \
    "docker exec -i $CONTAINER sh -c \"cd #PWD# && exec /usr/bin/g++ #ARGS#\"" -q -i \
    -f "$PWD/usr/include" "/usr/include" \
    -t "/usr/include" "$PWD/usr/include" \
    -f "$PWD/usr/lib" "/usr/lib" \
    -t "/usr/lib" "$PWD/usr/lib" \
    -f "\\" "/" \
    -f "C:" "/c"
chmod +x "$PWD/usr/bin/g++.exe"

../../bin/creator ../../bin/proxy $PWD/usr/bin/gdb.exe \
    "docker exec -i $CONTAINER sh -c \"cd #PWD# && exec /usr/bin/gdb #ARGS#\"" -q -i \
    -f "$PWD/usr/include" "/usr/include" \
    -t "/usr/include" "$PWD/usr/include" \
    -f "$PWD/usr/lib" "/usr/lib" \
    -t "/usr/lib" "$PWD/usr/lib" \
    -f "\\" "/" \
    -f "C:" "/c" \
    -t "/c" "C:" #-l $PWD/log.txt
chmod +x "$PWD/usr/bin/gdb.exe"
