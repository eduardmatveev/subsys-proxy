mkdir -p bin
cd bin
cmake ../src -DCMAKE_BUILD_TYPE=Release
cmake --build . --target all -- -j
