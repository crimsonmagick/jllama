#!/bin/bash
DEFAULT_BRANCH="master"
BRANCH_NAME=${BRANCH_NAME:-$DEFAULT_BRANCH}
git clone -b $BRANCH_NAME https://github.com/crimsonmagick/llama-cpp-java-bindings jllama
cd jllama
git submodule update --init --recursive
args=("$@")
./gradlew build jar publish -PRELEASE "${args[@]}" --info
#./gradlew build jar publish -PRELEASE --info
