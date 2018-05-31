#!/bin/sh
set -ex

mkdir -p $PWD/usr/bin
../../bin/creator ../../bin/proxy $PWD/usr/bin/cmake.exe \
    "/c/Windows/System32/bash.exe -c \"cd #PWD# && exec /usr/bin/cmake #ARGS#\"" -q -i \
    -f "$LOCALAPPDATA/lxss/rootfs/usr/" "/usr/" \
    -t "/usr/" "$LOCALAPPDATA/lxss/rootfs/usr/" \
    -f "\\" "/" \
    -f "/c/" "/mnt/c/" \
    -f "C:" "/mnt/c" \
    -t "/mnt/c" "C:"
../../bin/creator ../../bin/proxy $PWD/usr/bin/make.exe \
    "/c/Windows/System32/bash.exe -c \"cd #PWD# && exec /usr/bin/make #ARGS#\"" -q -i \
    -f "$LOCALAPPDATA/lxss/rootfs/usr/" "/usr/" \
    -t "/usr/" "$LOCALAPPDATA/lxss/rootfs/usr/" \
    -f "\\" "/" \
    -f "/c/" "/mnt/c/" \
    -f "C:" "/mnt/c" \
    -t "/mnt/c" "C:"
../../bin/creator ../../bin/proxy $PWD/usr/bin/gcc.exe \
    "/c/Windows/System32/bash.exe -c \"cd #PWD# && exec /usr/bin/gcc #ARGS#\"" -q -i \
    -f "$LOCALAPPDATA/lxss/rootfs/usr/" "/usr/" \
    -t "/usr/" "$LOCALAPPDATA/lxss/rootfs/usr/" \
    -f "\\" "/" \
    -f "/c/" "/mnt/c/" \
    -f "C:" "/mnt/c" \
    -t "/mnt/c" "C:"
../../bin/creator ../../bin/proxy $PWD/usr/bin/g++.exe \
    "/c/Windows/System32/bash.exe -c \"cd #PWD# && exec /usr/bin/g++ #ARGS#\"" -q -i \
    -f "$LOCALAPPDATA/lxss/rootfs/usr/" "/usr/" \
    -t "/usr/" "$LOCALAPPDATA/lxss/rootfs/usr/" \
    -f "\\" "/" \
    -f "/c/" "/mnt/c/" \
    -f "C:" "/mnt/c" \
    -t "/mnt/c" "C:"
../../bin/creator ../../bin/proxy $PWD/usr/bin/gdb.exe \
    "/c/Windows/System32/bash.exe -c \"cd #PWD# && exec /usr/bin/gdb #ARGS#\"" -q -i \
    -f "$LOCALAPPDATA/lxss/rootfs/usr/" "/usr/" \
    -t "/usr/" "$LOCALAPPDATA/lxss/rootfs/usr/" \
    -f "\\" "/" \
    -f "/c/" "/mnt/c/" \
    -f "C:" "/mnt/c" \
    -t "/mnt/c" "C:" #-l $PWD/log.txt
