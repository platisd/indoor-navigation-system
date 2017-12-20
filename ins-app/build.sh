#!/bin/bash

caller_dir=$PWD
# Move to the current script directory
cd "${0%/*}"

# Build the component
./gradlew connectedCheck -PdisablePreDex --stacktrace

# Go back to the initial directory when you are done
cd $caller_dir
