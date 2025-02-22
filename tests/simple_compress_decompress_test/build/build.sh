python3 generate_cpp.py
git submodule foreach git pull origin master
cp ../../../src/bitstream.cpp generated_src/
cp ../../../src/bitstream.hpp generated_src/
mkdir "generated_src/cpp-any-type"
cp ../../../src/cpp-any-type/any.cpp generated_src/cpp-any-type/
cp ../../../src/cpp-any-type/any.hpp generated_src/cpp-any-type/
cp ../main.cpp generated_src/

clang++-11 -ggdb generated_src/*.cpp generated_src/cpp-any-type/*.cpp -o simple_comp_decomp -O3 -Weverything -pedantic -Wno-float-equal -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Werror