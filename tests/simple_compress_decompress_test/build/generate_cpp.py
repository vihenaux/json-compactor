import os
import sys

sys.path.insert(1, '../../../src/build/')

import generate_file

def generate():
    os.makedirs("generated_src", exist_ok=True)
    #generate_file.generate_file_from_lib("../../../src/compress-json.cpp", "../../../files/test-schema.json", "generated_src/compress-json.cpp", "../../../src/build/gcg_key")
    #generate_file.generate_file_from_lib("../../../src/compress-json.hpp", "../../../files/test-schema.json", "generated_src/compress-json.hpp", "../../../src/build/gcg_key")
    generate_file.generate_file_from_lib("../../../src/decompress-json.cpp", "../../../files/test-schema.json", "generated_src/decompress-json.cpp", "../../../src/build/gcg_key")
    #generate_file.generate_file_from_lib("../../../src/decompress-json.hpp", "../../../files/test-schema.json", "generated_src/decompress-json.hpp", "../../../src/build/gcg_key")

if __name__ == "__main__":
    generate()
