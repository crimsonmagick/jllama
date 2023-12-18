#!/bin/bash
git clone -b mingw-fix https://github.com/crimsonmagick/llama-cpp-java-bindings jllama
cd jllama
git submodule update --init --recursive
./gradlew build jar publish -PRELEASE --info