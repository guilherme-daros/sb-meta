#!/bin/sh

BUILD_DIR=".build"

show_help() {
  echo "Usage: ./build.sh [OPTIONS]"
  echo "Build and clean your project."
  echo
  echo "Options:"
  echo "  -h, --help    Show this help message and exit."
  echo "  -c, --clean   Delete .build folders."
  echo "  --gtest       build with Google Test framework."
  echo "                Use this option for running tests."
  echo "  [default]     build with no testing framework."
  echo "                Use this for normal project build."
}

case "$1" in
  -c | --clean)
    echo "Deleting build folders"
    rm -rf .build
    ;;
  --gtest)
    cmake -S . -G "Ninja" -B .build -DFRAMEWORK=gtest -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
    cmake --build .build
    ;;
  -h | --help)
    show_help
    ;;
  *)
    mkdir -p .build
    cmake -S . -G "Ninja" -B .build -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
    cmake --build .build
    ;;
esac
