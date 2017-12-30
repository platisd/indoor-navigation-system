#!/bin/bash
caller_dir=$PWD
# Move to the current script directory
cd "${0%/*}"

# Build and run the various tests
./gradlew test --info
./gradlew connectedAndroidTest

# Go back to the initial directory when you are done
cd $caller_dir
