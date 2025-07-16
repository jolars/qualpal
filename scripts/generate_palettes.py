# Script to convert JSON color palette data into a C++ map format
# Assumes that the JSON file is structured with outer keys as palette names
# and inner keys as color categories, with values being lists of color strings.

import json
import re


def to_camel_case(s):
    parts = re.split(r"[_\-\s]", s)
    return "".join(word.capitalize() for word in parts if word)


json_file = "scripts/raw_data.json"
output_file = "scripts/color_palettes.cpp"

allowed_outer_keys = ["ochRe"]

with open(json_file, "r") as f:
    data = json.load(f)

cpp_map = "std::map<std::string, std::map<std::string, std::vector<std::string>>> color_palettes = {\n"

for outer_key, outer_value in data.items():
    if outer_key not in allowed_outer_keys:
        continue
    if outer_key is not None:
        cpp_map += '    { "' + to_camel_case(outer_key) + '", {\n'
    for inner_key, inner_value in outer_value.items():
        if inner_key is not None:
            cpp_map += '        { "' + to_camel_case(inner_key) + '", {\n'
        for color in inner_value:
            if color is not None:
                cpp_map += '            "' + color + '",\n'
        cpp_map = cpp_map.rstrip(",\n") + "\n        } },\n"
    cpp_map = cpp_map.rstrip(",\n") + "\n    } },\n"
cpp_map = cpp_map.rstrip(",\n") + "\n};"

with open(output_file, "w") as f:
    f.write(cpp_map)
