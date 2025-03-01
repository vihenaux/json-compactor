import json
import random
import string
import sys

def generate_random_string(length=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def generate_random_data(schema):
    data = {}

    for key, value in schema.get('properties', {}).items():
        if 'type' in value:
            if value['type'] == 'string':
                data[key] = generate_random_string(random.randint(0, 20))
            elif value['type'] == 'integer':
                data[key] = random.randint(0, 100)
            elif value['type'] == 'float':
                data[key] = round(random.uniform(0, 100), 3)
            elif value['type'] == 'double':
                data[key] = round(random.uniform(0, 100), 6)
            elif value['type'] == 'boolean':
                data[key] = random.choice([True, False])
            elif value['type'] == 'array':
                if 'items' in value:
                    item_type = value['items'].get('type', None)
                    if item_type == 'string':
                        data[key] = [generate_random_string(random.randint(0, 20)) for _ in range(random.randint(0, 5))]
                    elif item_type == 'integer':
                        data[key] = [random.randint(0, 100) for _ in range(random.randint(0, 5))]
                    elif item_type == 'float':
                        data[key] = [round(random.uniform(0, 100), 3) for _ in range(random.randint(0, 5))]
                    elif item_type == 'double':
                        data[key] = [round(random.uniform(0, 100), 6) for _ in range(random.randint(0, 5))]
                    elif item_type == 'boolean':
                        data[key] = [random.choice([True, False]) for _ in range(random.randint(0, 5))]
                    elif item_type == 'object':
                        data[key] = [generate_random_data(value['items']) for _ in range(random.randint(0, 5))]
            elif value['type'] == 'object':
                data[key] = generate_random_data(value)

    return data

def load_json_schema(schema_path):
    with open(schema_path, 'r') as schema_file:
        return json.load(schema_file)

def write_json_to_file(data, output_path):
    with open(output_path, 'w') as output_file:
        json.dump(data, output_file, indent=4)

def generate_random_json_from_schema(schema_path, output_path):
    schema = load_json_schema(schema_path)
    random_data = generate_random_data(schema)
    write_json_to_file(random_data, output_path)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        schema_path = input("Enter the path to the JSON schema: ")
        output_path = input("Enter the path to save the generated JSON file: ")
        generate_random_json_from_schema(schema_path, output_path)
    else:
        generate_random_json_from_schema(sys.argv[1],sys.argv[2])
