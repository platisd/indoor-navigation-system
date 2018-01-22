#!/usr/bin/env bash

# Usage: Run this script once to install git commit template.

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd "$DIR"

git config commit.template commit_msg_template.txt
