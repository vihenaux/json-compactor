#include "build/compress-json.hpp"
#include "build/decompress-json.hpp"

int main(int, char **argv)
{
    std::string to_compress_file_path(argv[1]);
    compress(to_compress_file_path, "compressed_file.bin");
    decompress("compressed_file.bin", "decompressed_file.json");

    return 0;
}
