from sys import argv

offsets = {
    "game" : 
    {
        "text" : 0x8063f094,
        "data" : 0x807b1600,
        "bss" : 0x808610e0,
        "rodata" : 0x807ad440,
    }
}

def get_curr_offset(name:str):
    parts = name.split("_")
    rel_code = int(parts[1])
    if len(parts) == 3:
        assert(parts[0] == "fn"), parts[0]
        return offsets["game"]["text"] + int(parts[2], 16)
    elif len(parts) == 4:
        section = parts[2]
        return offsets["game"][section] + int(parts[3], 16)

def main():
    symbol_name = argv[1]
    print(f"{get_curr_offset(symbol_name):x}")

if __name__ == "__main__":
    main()