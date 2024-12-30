import generate_file
import os
import re


def generate():
    os.makedirs("generated_src", exist_ok=True)
    #generate_file.generate_file_from_lib("templates/template.test", "project_example.json", "output/out.test", "paste_key_here.txt")
    for current_dir, sub_dirs, files in os.walk("../"):
        if re.search("(.*/)?build(/.*)?", current_dir):
            continue
        if len(current_dir) > 3:
            os.makedirs("generated_src/"+current_dir[3:], exist_ok=True)
        for file in files:
            generate_file.generate_file_from_lib(current_dir+"/"+file, "../../files/test-schema.json", "generated_src/"+current_dir[3:]+file, "gcg_key")


if __name__ == "__main__":
    generate()
