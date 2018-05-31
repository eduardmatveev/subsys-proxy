mkdir -p bin
cd bin

../../usr/bin/cmake ../ -DCMAKE_BUILD_TYPE=Debug
../../usr/bin/make -j
