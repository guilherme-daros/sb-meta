#!/bin/sh

BUILD_DIR="build"

show_help() {
  echo "Usage: ./build.sh [OPTIONS]"
  echo "Build and clean your project."
  echo
  echo "Options:"
  echo "  -h, --help      Show this help message and exit."
  echo "  -c, --clean     Delete build folders."
  echo "  -e, --examples  Build examples"
  echo "  --gtest         build with Google Test framework."
  echo "                  Use this option for running tests."
}

case "$1" in
  -c | --clean)
    echo "Deleting build folders"
    rm -rf $BUILD_DIR output
    ;;
  --gtest)
    cmake -S . -G "Ninja" -B $BUILD_DIR  -DFRAMEWORK=gtest 
    cmake --build $BUILD_DIR
    ;;
   -e | --examples)
    cmake -S . -G "Ninja" -B $BUILD_DIR  -DBUILD_EXAMPLES=yes -DCMAKE_BUILD_TYPE=Debug
    cmake --build $BUILD_DIR
    ;;
  -h | --help)
    show_help
    ;;
  *)
    mkdir -p $BUILD_DIR
    cmake -S . -G "Ninja" -B $BUILD_DIR 
    cmake --build $BUILD_DIR
    ;;
esac
