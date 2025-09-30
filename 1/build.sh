#!/bin/bash

mkdir -p build
cd build

cmake ..
make

echo "构建完成！使用 ./build/bin/dpll_solver 运行"