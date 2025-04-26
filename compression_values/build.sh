#!/bin/bash

clang++-11 build/*.cpp build/cpp-any-type/*.cpp main.cpp -o random_test -O3 -Weverything -pedantic -Wno-float-equal -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Werror