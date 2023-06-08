#!/bin/bash

BUILD_DIR="$(pwd)/build"
DIST_DIR="$BUILD_DIR/dist"
export PATH="$PATH:$DIST_DIR/bin"

export PYTHONPATH="$DIST_DIR/python:$PYTHONPATH"