#!/bin/bash
caller_dir=$PWD
# Move to the current script directory
cd "${0%/*}"

ins_node_test_dir="ins-node/test"
ins_server_dir="server"

# Specify which directories are to be excluded from coverage report
coverage_ignore_dirs="\
    '/usr/*' \
    '${ins_node_test_dir}/ins_mock/*' \
    '${ins_node_test_dir}/build/arduino_mock/*' \
    '${ins_node_test_dir}/stubs/*' \
    '${ins_node_test_dir}/ut/*' \
    '${ins_server_dir}/include/external/spdlog/*' \
    '${ins_server_dir}/test/*'
"

eval lcov --directory . --capture --output-file coverage.info
eval lcov --remove coverage.info $coverage_ignore_dirs --output-file coverage.info
eval lcov --list coverage.info

# Go back to the initial directory when you are done
cd $caller_dir
