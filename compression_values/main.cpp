#include "build/compress-json.hpp"
#include "build/decompress-json.hpp"
#include <chrono>

int main(int, char **argv)
{
    std::string to_compress_file_path(argv[1]);

    auto start_compression = std::chrono::high_resolution_clock::now();
    for(unsigned int i(0); i < 100; ++i)
        compress(to_compress_file_path, "compressed_file.bin");
    auto end_compression = std::chrono::high_resolution_clock::now();
    auto duration_compression = std::chrono::duration_cast<std::chrono::microseconds>(end_compression - start_compression);

    auto start_decompression = std::chrono::high_resolution_clock::now();
    for(unsigned int i(0); i < 100; ++i)
        decompress("compressed_file.bin", "decompressed_file.json");
    auto end_decompression = std::chrono::high_resolution_clock::now();
    auto duration_decompression = std::chrono::duration_cast<std::chrono::microseconds>(end_decompression - start_decompression);

    std::cout << duration_compression.count()/100 << " " << duration_decompression.count()/100 << std::endl;

    return 0;
}
