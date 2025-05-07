import json
import sys

def compare_float(float1, float2):
    return float1 == float2

def compare_array(array1, array2):
    if len(array1) != len(array2):
        return False

    for i in range(len(array1)):
        value1 = array1[i]
        value2 = array2[i]
        if isinstance(value1, float) and isinstance(value2, float):
            if not compare_float(value1, value2):
                print("ARRAY - Float error: ", value1, " ; ", value2)
                return False
        elif isinstance(value1, dict) and isinstance(value2, dict):
            if not compare_dict(value1, value2):
                print("ARRAY - Dict error")
                return False
        elif value1 != value2:
            print("ARRAY - Value error: ", value1, " ; ", value2)
            return False

    return True


def compare_dict(dict1, dict2):
    if dict1.keys() != dict2.keys():
        print("Key error")
        return False

    # Compare key-value pairs
    for key in dict1:
        value1 = dict1[key]
        value2 = dict2[key]

        if isinstance(value1, float) and isinstance(value2, float):
            if not compare_float(value1, value2):
                print("DICT - Float error: ", value1, " ; ", value2)
                return False
        elif isinstance(value1, dict) and isinstance(value2, dict):
            if not compare_dict(value1, value2):
                print("DICT - Dict error")
                return False
        elif isinstance(value1, list) and isinstance(value2, list):
            if not compare_array(value1, value2):
                print("DICT - Array error")
                return False
        elif value1 != value2:
            print("DICT - Value error: ", value1, " ; ", value2)
            return False

    return True


def compare_json(file1, file2):
    try:
        with open(file1, 'r') as f1, open(file2, 'r') as f2:
            data1 = json.load(f1)
            data2 = json.load(f2)
    except (json.JSONDecodeError, FileNotFoundError) as e:
        print(f"Error reading the files: {e}")
        return False

    if not isinstance(data1, dict) and isinstance(data2, dict):
        print("Error : data is not json")
        return False

    return compare_dict(data1, data2)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python compare_json.py <file1.json> <file2.json>")
        sys.exit(1)

    if compare_json(sys.argv[1], sys.argv[2]):
        print("Json files equivalent")
    else:
        print("Json files different")
