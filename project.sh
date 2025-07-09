#!/bin/bash

BUILD_ROOT_DIR="build"
TOOLCHAIN_FILE="toolchain-arm.cmake" # Default toolchain

if [ $# -eq 0 ]; then
  echo "No options given. Exiting."
  exit 1
fi

if [ "$1" = "clean" ]; then
    rm -rf $BUILD_ROOT_DIR
    exit 0
fi

case "$1" in
    release)
        BUILD_SUB_DIR="release"
        BUILD_TYPE="Debug"
        TOOLCHAIN_FILE="toolchain-arm.cmake"
        ;;
    simulation)
        echo "building for simulation..."
        BUILD_SUB_DIR="simulation"
        BUILD_TYPE="Simulation"
        TOOLCHAIN_FILE="toolchain-host.cmake" # Use the host toolchain for simulation
        ;;
    debug|*)
        BUILD_SUB_DIR="debug"
        BUILD_TYPE="Debug"
        TOOLCHAIN_FILE="toolchain-arm.cmake"
        ;;
    mccabe)
        cmake -B build/release
        cmake --build build/release  --target run_pmccabe
        exit 0
        ;;
    *)
        echo "please add an option"
        exit 0
        ;;

esac

BUILD_DIR="$BUILD_ROOT_DIR/$BUILD_SUB_DIR"

if [ "$2" = "run" ] && [ "$BUILD_TYPE" = "Simulation" ]; then
    echo "Running the simulation executable..."
    $BUILD_DIR/bin/exploracao_project.sim
    exit 0
fi

case "$2" in
    clean)
        ninja -C $BUILD_DIR clean
        ;;
    erase)
        rm -rf $BUILD_DIR
        ;;
    build|*)
        # if [ ! -d "$BUILD_DIR" ]; then
        #     cmake -G Ninja -B $BUILD_DIR -S . -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE

        # fi
        # Always run CMake to ensure configuration is up to date.
        echo "building with toolchain $TOOLCHAIN_FILE"
        cmake -G Ninja -B $BUILD_DIR -S . -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE
        ninja -C $BUILD_DIR
        ;;
    verbose)
        # For the verbose flag, make sure cmake configuration is also considered.
        cmake -G Ninja -B $BUILD_DIR -S . -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE
        ninja -C $BUILD_DIR -v
        ;;

esac