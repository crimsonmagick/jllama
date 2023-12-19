#!/bin/bash
git clone -b master https://github.com/crimsonmagick/llama-cpp-java-bindings jllama
cd jllama
git submodule update --init --recursive
./gradlew build jar publish -PRELEASE --info