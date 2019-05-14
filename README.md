# chip8-emulator
Repository for my Chip8 Emulator written and tested using C++

Directory structure is as follows

|-> bin 	- for output executables

|-> build 	- for .o files

|-> doc 	- for notes

|-> include - project headers

|-> lib 	- libraries that get compiled by project

|-> src 	- application source files

|-> test 	- test code files

# Building
Code was built on Ubuntu18-04

## Required dependencies
cmake
    sudo apt-get update
    sudo apt-get install cmake
SDL2.0
    sudo apt-get update
    sudo apt-get install libsdl2-dev

## How to Build
To build:
Clone repo
cd into the new folder
mkdir build
cd build
cmake ..
make
./main

# Testing
Code was tested on Ubuntu18-04 using gtest

With gtest in /usr/lib

## Required dependencies
cmake
    sudo apt-get update
    sudo apt-get install cmake
gtest
    sudo apt-get update
    sudo apt-get install libgtest-dev

## How to Run Tests
Clone repo
cd into the new folder
cd tests
mkdir build
cd build
cmake ..
make
./tests