echo "BitStream test"
cd build
./build.sh > /dev/null
./bitstreamtest

echo "Simple compress decompress test"
cd ../simple_compress_decompress_test/build
./build.sh > /dev/null 2> /dev/null
./simple_comp_decomp
if ! diff decompressed_file.json decompressed_file_compare.json > /dev/null 2> /dev/null; then
    echo -e "\033[0;31mWARNING: Differences found between the decompressed file and the reference file!\033[0m"
fi