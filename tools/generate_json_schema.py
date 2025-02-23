import random
import string
import json

ALL_SCHEMA_TYPES = ['string', 'integer', 'float', 'double', 'boolean', 'array', 'object']
NON_ARRAY_SCHEMA_TYPES = ['string', 'integer', 'float', 'double', 'boolean', 'object']

def generate_random_string(length=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def generate_random_schema_with_arrays(num_properties=5):
    schema = {
        "type": "object",
        "properties": {}
    }

    for _ in range(num_properties):
        key = generate_random_string(random.randint(5, 8))
        property_type = random.choice(ALL_SCHEMA_TYPES)

        if property_type == 'array':
            schema["properties"][key] = {
                "type": "array",
                "items": generate_random_array_item_schema()
            }
        elif property_type == 'object':
            schema["properties"][key] = {
                "type": "object",
                "properties": generate_random_schema_with_arrays(random.randint(2, 5))["properties"]
            }
        else:
            schema["properties"][key] = {"type": property_type}


    return schema

def generate_random_array_item_schema():
    item_type = random.choice(NON_ARRAY_SCHEMA_TYPES)
    if item_type == 'object':
        return {
            "type": "object",
            "properties": generate_random_schema_with_arrays(random.randint(2, 5))["properties"]
        }
    else:
        return {"type": item_type}

def generate_schema(output_path):
    with open(output_path, 'w') as output_file:
        json.dump(generate_random_schema_with_arrays(random.randint(2, 5)), output_file, indent=4)

if __name__ == "__main__":
    random_schema = generate_random_schema_with_arrays(random.randint(2, 5))
    print(json.dumps(
        random_schema,
        indent=4,
        separators=(',', ': ')
    ))
