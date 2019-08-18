#!/bin/bash
# Need to install cmake and SDl2.0
sudo apt-get update
sudo apt-get install -y cmake
sudo apt-get install -y libsdl2-dev
sudo apt-get install -y lslibgtest-dev

mkdir build
cd build
cmake ..
make