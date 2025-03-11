import sys
import random
import string
import subprocess
import os
import shutil
import json

sys.path.insert(1, '../../tools/')

import compare_json
import generate_json_schema
import generate_random_json_from_schema

sys.path.insert(1, '../../src/build/')

import generate_file


def generate_random_filename():
    return (''.join(random.choices(string.ascii_letters + string.digits, k=20))) + ".json"


def compilation_test(schema_file_path):
    if os.path.isfile("random_test"):
        os.remove("random_test")
    os.makedirs("build", exist_ok=True)
    os.makedirs("build/cpp-any-type", exist_ok=True)

    generate_file.generate_file_from_lib("../../src/compress-json.cpp", schema_file_path, "build/compress-json.cpp", "../../src/build/gcg_key")
    generate_file.generate_file_from_lib("../../src/compress-json.hpp", schema_file_path, "build/compress-json.hpp", "../../src/build/gcg_key")
    generate_file.generate_file_from_lib("../../src/decompress-json.cpp", schema_file_path, "build/decompress-json.cpp", "../../src/build/gcg_key")
    generate_file.generate_file_from_lib("../../src/decompress-json.hpp", schema_file_path, "build/decompress-json.hpp", "../../src/build/gcg_key")

    shutil.copyfile("../../src/bitstream.cpp", "build/bitstream.cpp")
    shutil.copyfile("../../src/bitstream.hpp", "build/bitstream.hpp")
    shutil.copyfile("../../src/cpp-any-type/any.cpp", "build/cpp-any-type/any.cpp")
    shutil.copyfile("../../src/cpp-any-type/any.hpp", "build/cpp-any-type/any.hpp")

    subprocess.call("./build.sh")

    return os.path.isfile("random_test")


def complete_test(schema_file_path, json_path_array):
    try:
        if not compilation_test(schema_file_path):
            return [False for _ in range(len(json_path_array))]

        result = []
        for json_path in json_path_array:
            os.system("./random_test " + json_path)
            result.append(compare_json.compare_json(json_path, "decompressed_file.json"))
        return result
    except Exception:
        return [False for _ in range(len(json_path_array))]



def create_new_test():
    new_test = {}
    new_test["schema"] = "random_files/schema_" + generate_random_filename()
    generate_json_schema.generate_schema(new_test["schema"])
    new_test["test_files"] = []
    for _ in range(10):
        test_file_tmp_path = "random_files/" + generate_random_filename()
        new_test["test_files"].append(test_file_tmp_path)
        generate_random_json_from_schema.generate_random_json_from_schema(new_test["schema"], test_file_tmp_path)

    return new_test


def delete_test(test_to_delete):
    os.remove(test_to_delete["schema"])
    for json_path in test_to_delete["test_files"]:
        os.remove(json_path)


def main():
    with open('once_failed_tests.json') as tests_file:
        tests = json.load(tests_file)

    tests_succeeded = True
    for test in tests["once_failed_tests_array"]:
        if False in complete_test(test["schema"], test["test_files"]):
            tests_succeeded = False

    if not tests_succeeded:
        return

    for _ in range(100):
        test = create_new_test()

        test_result = complete_test(test["schema"], test["test_files"])
        if False not in test_result:
            delete_test(test)
        else:
            test_files_to_keep = []
            for i in range(len(test_result)):
                if not test_result[i]:
                    test_files_to_keep.append(test["test_files"][i])
                else:
                    os.remove(test["test_files"][i])
            test["test_files"] = test_files_to_keep

            tests["once_failed_tests_array"].append(test)
            break

    with open('once_failed_tests.json', 'w') as tests_file:
        json.dump(tests, tests_file, indent=4)





if __name__ == "__main__":
    main()