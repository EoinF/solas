#!/bin/bash
set -ex

git clone https://github.com/Microsoft/vcpkg ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
sudo apt-get install libsfml-dev
~/vcpkg/vcpkg install glm
git submodule update --init --recursive
