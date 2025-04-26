import sys
import random
import string
import subprocess
import os
import shutil
import json
import git

sys.path.insert(1, '../tools/')

import generate_json_schema
import generate_random_json_from_schema

sys.path.insert(1, '../src/build/')

import generate_file


def generate_random_filename():
    return (''.join(random.choices(string.ascii_letters + string.digits, k=20))) + ".json"


def compilation_test(schema_file_path):
    if os.path.isfile("random_test"):
        os.remove("random_test")
    os.makedirs("build", exist_ok=True)
    os.makedirs("build/cpp-any-type", exist_ok=True)

    generate_file.generate_file_from_lib("../src/compress-json.cpp", schema_file_path, "build/compress-json.cpp", "../src/build/gcg_key")
    generate_file.generate_file_from_lib("../src/compress-json.hpp", schema_file_path, "build/compress-json.hpp", "../src/build/gcg_key")
    generate_file.generate_file_from_lib("../src/decompress-json.cpp", schema_file_path, "build/decompress-json.cpp", "../src/build/gcg_key")
    generate_file.generate_file_from_lib("../src/decompress-json.hpp", schema_file_path, "build/decompress-json.hpp", "../src/build/gcg_key")

    shutil.copyfile("../src/bitstream.cpp", "build/bitstream.cpp")
    shutil.copyfile("../src/bitstream.hpp", "build/bitstream.hpp")
    shutil.copyfile("../src/cpp-any-type/any.cpp", "build/cpp-any-type/any.cpp")
    shutil.copyfile("../src/cpp-any-type/any.hpp", "build/cpp-any-type/any.hpp")

    subprocess.call("./build.sh")

    return os.path.isfile("random_test")


def complete_test(schema_file_path, json_path_array):
    try:
        if not compilation_test(schema_file_path):
            return None

        result = []
        for json_path in json_path_array:
            os.system("./random_test " + json_path)
            json_size = os.path.getsize(json_path)
            compressed_size = os.path.getsize("compressed_file.bin")
            result.append(1 - (compressed_size/json_size))
        return result
    except Exception as e:
        print(e)
        return None



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


def is_schema_needed(schema, compression_history):
    for commit_history in compression_history["stats"]:
        if commit_history["min_compression_schema"] == schema or commit_history["max_compression_schema"] == schema:
            return True
    return False


def is_file_needed(file, compression_history):
    for commit_history in compression_history["stats"]:
        if commit_history["min_compression_file"] == file or commit_history["max_compression_file"] == file:
            return True
    return False


def try_removing_schema(schema, compression_history):
    if not is_schema_needed(schema, compression_history):
        os.remove(schema)


def try_removing_file(file, compression_history):
    if not is_file_needed(file, compression_history):
        os.remove(file)


def main(nb_test_to_run):
    current_git_repo = git.Repo(search_parent_directories=True)
    current_commit = current_git_repo.head.object.hexsha

    with open('compression_stats_history.json') as history_file:
        compression_history = json.load(history_file)

    if compression_history["stats"][-1]["commit_hash"] != current_commit:
        if __name__ == '__main__':
            compression_history["stats"].append({
                "commit_hash": current_commit,
                "min_compression_rate": 1,
                "min_compression_schema": "",
                "min_compression_file": "",
                "max_compression_rate": 0,
                "max_compression_schema": "",
                "max_compression_file": "",
                "avg_compression_rate": 0,
                "nb_compression_test": 0
            })

    if compression_history["stats"][-1]["min_compression_rate"] == 1:
        compression_rate_sum = 0
        nb_tests = 0
        for commit_history in compression_history["stats"][:-1]:
            test_result = complete_test(commit_history["min_compression_schema"], [commit_history["min_compression_file"]])

            if test_result[0] < compression_history["stats"][-1]["min_compression_rate"]:
                original_schema = compression_history["stats"][-1]["min_compression_schema"]
                original_file = compression_history["stats"][-1]["min_compression_file"]

                compression_history["stats"][-1]["min_compression_rate"] = test_result[0]
                compression_history["stats"][-1]["min_compression_schema"] = commit_history["min_compression_schema"]
                compression_history["stats"][-1]["min_compression_file"] = commit_history["min_compression_file"]

                if original_schema != "":
                    try_removing_schema(original_schema, compression_history)
                    try_removing_file(original_file, compression_history)
            if test_result[0] > compression_history["stats"][-1]["max_compression_rate"]:
                original_schema = compression_history["stats"][-1]["max_compression_schema"]
                original_file = compression_history["stats"][-1]["max_compression_file"]

                compression_history["stats"][-1]["max_compression_rate"]= test_result[0]
                compression_history["stats"][-1]["max_compression_schema"]= commit_history["min_compression_schema"]
                compression_history["stats"][-1]["max_compression_file"]= commit_history["min_compression_file"]

                if original_schema != "":
                    try_removing_schema(original_schema, compression_history)
                    try_removing_file(original_file, compression_history)


            test_result = complete_test(commit_history["max_compression_schema"], [commit_history["max_compression_file"]])

            if test_result[0] < compression_history["stats"][-1]["min_compression_rate"]:
                original_schema = compression_history["stats"][-1]["min_compression_schema"]
                original_file = compression_history["stats"][-1]["min_compression_file"]

                compression_history["stats"][-1]["min_compression_rate"] = test_result[0]
                compression_history["stats"][-1]["min_compression_schema"] = commit_history["min_compression_schema"]
                compression_history["stats"][-1]["min_compression_file"] = commit_history["min_compression_file"]

                if original_schema != "":
                    try_removing_schema(original_schema, compression_history)
                    try_removing_file(original_file, compression_history)
            if test_result[0] > compression_history["stats"][-1]["max_compression_rate"]:
                original_schema = compression_history["stats"][-1]["max_compression_schema"]
                original_file = compression_history["stats"][-1]["max_compression_file"]

                compression_history["stats"][-1]["max_compression_rate"]= test_result[0]
                compression_history["stats"][-1]["max_compression_schema"]= commit_history["min_compression_schema"]
                compression_history["stats"][-1]["max_compression_file"]= commit_history["min_compression_file"]

                if original_schema != "":
                    try_removing_schema(original_schema, compression_history)
                    try_removing_file(original_file, compression_history)

            compression_rate_sum += test_result[0]
            nb_tests += 2

        compression_history["stats"][-1]["avg_compression_rate"] = compression_rate_sum / nb_tests
        compression_history["stats"][-1]["nb_compression_test"] = nb_tests


    for _ in range(nb_test_to_run):
        test = create_new_test()

        test_result = complete_test(test["schema"], test["test_files"])
        if test_result is None:
            delete_test(test)
            return

        compression_rate_sum = 0
        for i in range(10):
            if test_result[i] < compression_history["stats"][-1]["min_compression_rate"]:
                original_schema = compression_history["stats"][-1]["min_compression_schema"]
                original_file = compression_history["stats"][-1]["min_compression_file"]

                compression_history["stats"][-1]["min_compression_rate"] = test_result[i]
                compression_history["stats"][-1]["min_compression_schema"] = test["schema"]
                compression_history["stats"][-1]["min_compression_file"] = test["test_files"][i]

                if original_schema != "":
                    try_removing_schema(original_schema, compression_history)
                    try_removing_file(original_file, compression_history)
            if test_result[i] > compression_history["stats"][-1]["max_compression_rate"]:
                original_schema = compression_history["stats"][-1]["max_compression_schema"]
                original_file = compression_history["stats"][-1]["max_compression_file"]

                compression_history["stats"][-1]["max_compression_rate"]= test_result[i]
                compression_history["stats"][-1]["max_compression_schema"]= test["schema"]
                compression_history["stats"][-1]["max_compression_file"]= test["test_files"][i]

                if original_schema != "":
                    try_removing_schema(original_schema, compression_history)
                    try_removing_file(original_file, compression_history)

            compression_rate_sum += test_result[i]

        try_removing_schema(test["schema"], compression_history)
        for test_file in test["test_files"]:
            try:
                try_removing_file(test_file, compression_history)
            except FileNotFoundError:
                pass

        avg = compression_history["stats"][-1]["avg_compression_rate"]
        nb = compression_history["stats"][-1]["nb_compression_test"]
        compression_history["stats"][-1]["avg_compression_rate"] = (avg*nb + compression_rate_sum) / (nb + 10)
        compression_history["stats"][-1]["nb_compression_test"] = nb + 10

    with open('compression_stats_history.json', 'w') as tests_file:
        json.dump(compression_history, tests_file, indent=4)


if __name__ == "__main__":
    nb_test_to_run = 10
    if len(sys.argv) == 2:
        nb_test_to_run = int(sys.argv[1])

    main(nb_test_to_run)