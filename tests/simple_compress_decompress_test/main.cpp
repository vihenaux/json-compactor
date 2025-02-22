#include "compress-json.hpp"
#include "decompress-json.hpp"

int main()
{
    compress("test.json", "compressed_file.bin");
    decompress("compressed_file.bin", "decompressed_file.json");

    return 0;
}
