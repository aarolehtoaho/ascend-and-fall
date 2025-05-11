#!/bin/bash
set -e

mkdir -p ..build/windows
cd ..build/windows

cmake -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ ../..

make -j$(nproc) || {
    echo "Build failed."
    exit 1
}
echo "Build completed."
