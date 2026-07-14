#!/usr/bin/python3
import sys
import subprocess
from pathlib import Path

RED   = "\033[31m"
BLUE  = "\033[34m"
RESET = "\033[0m"


target = "image"
cc = ["gcc"]
warnings = ["-Wall", "-Wextra", "-Wconversion", "-std=c11", "-pedantic"]
flags = ["-I./build/", "-o", f"./{target}"]
src = ["./build/stb_image_write.o"]
libs = ["-lm"]


argc = len(sys.argv)
argv = sys.argv

if argc < 3:
    print(f"{RED}Too few arguments{RESET}")
    sys.exit()

if argc > 3:
    print(f"{RED}Too many arguments{RESET}")
    sys.exit()

match argv[1]:
    case "c":
        src.append(argv[2])
    case "g":
        src.append(argv[2])
        flags.append("-g")
    case "O3":
        src.append(argv[2])
        flags.append("-O3")
    case _:
        print(f"{RED}Uknown option {argv[1]}{RESET}")
        sys.exit()


build = Path("./build/")
if not build.exists():
    build.mkdir()
    print(f"{BLUE}Created {str(build)}{RESET}")

    if subprocess.run(["wget", "https://github.com/nothings/stb/raw/refs/heads/master/stb_image_write.h", "-O", "./build/stb_image_write.h"]).returncode != 0:
        print(f"{RED}Failed downloading stb_image_write.h{RESET}")
        sys.exit()

    with open("./build/stb_image_write.c", "w") as stb_file:
        stb_file.write("#define STB_IMAGE_WRITE_IMPLEMENTATION\n")
        stb_file.write("#include \"stb_image_write.h\"\n")

    if subprocess.run(["gcc", "-I./build", "-c", "-o", "./build/stb_image_write.o", "./build/stb_image_write.c"]).returncode == 0:
        print(f"{BLUE}Compiled ./build/stb_image_write.o{RESET}")
    else:
        print(f"{RED}Failed compiling ./build/stb_image_write.c{RESET}")
        sys.exit()


cmd = cc + warnings + flags + src + libs
print(f"{BLUE}Compiling: {cmd}{RESET}")
if subprocess.run(cmd).returncode == 0:
    print(f"{BLUE}Successfully{RESET}")
else:
    print(f"{RED}Failed{RESET}")
    sys.exit()

print(f"{BLUE}Running: ./{target}{RESET}")
returncode = subprocess.run(f"./{target}").returncode
if returncode == 0:
    print(f"{BLUE}Successfully{RESET}")
else:
    print(f"{RED}Failed with code {returncode}{RESET}")
    sys.exit()
