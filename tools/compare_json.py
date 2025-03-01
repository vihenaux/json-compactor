import json
import sys

def compare_json(file1, file2):
    try:
        with open(file1, 'r') as f1, open(file2, 'r') as f2:
            data1 = json.load(f1)
            data2 = json.load(f2)
    except (json.JSONDecodeError, FileNotFoundError) as e:
        print(f"Error reading the files: {e}")
        return False
    return data1, data2

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python compare_json.py <file1.json> <file2.json>")
        sys.exit(1)

    if compare_json(sys.argv[1], sys.argv[2]):
        print("Json files equivalent")
    else:
        print("Json files different")
