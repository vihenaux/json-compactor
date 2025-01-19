git submodule foreach git pull origin master

clang++-11 ../BitStreamTest.cpp ../../src/bitstream.cpp -o bitstreamtest -O3 -Weverything -pedantic -Wno-float-equal -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Werror