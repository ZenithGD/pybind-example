#!/bin/bash
# Build and install
mkdir -p build
cd build
if cmake -GNinja ..
then
    if ninja
    then
        ninja install
        foo_path="$(pwd)/build/dist/bin"
        echo "Executable at $foo_path, use foo_setpath.sh to make it available in PATH."
    else
        echo "Couldn't generate project!"
    fi
else
    echo "Check the build errors!"
fi