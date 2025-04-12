import json

json_file = "scripts/raw_data.json"
output_file = "scripts/color_palettes.cpp"

with open(json_file, "r") as f:
    data = json.load(f)

cpp_map = "std::map<std::string, std::map<std::string, std::vector<std::string>>> color_palettes = {\n"

for outer_key, outer_value in data.items():
    if outer_key is not None:
        cpp_map += '    { "' + outer_key + '", {\n'
    for inner_key, inner_value in outer_value.items():
        if inner_key is not None:
            cpp_map += '        { "' + inner_key + '", {\n'
        for color in inner_value:
            if color is not None:
                cpp_map += '            "' + color + '",\n'
        cpp_map = cpp_map.rstrip(",\n") + "\n        } },\n"
    cpp_map = cpp_map.rstrip(",\n") + "\n    } },\n"
cpp_map = cpp_map.rstrip(",\n") + "\n};"

with open(output_file, "w") as f:
    f.write(cpp_map)


# generate_cpp_map("scripts/raw_data.json", "color_palettes.cpp)")
