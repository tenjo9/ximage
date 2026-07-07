#!/usr/bin/python
import sys
import subprocess
from pathlib import Path


cc = ["gcc"]
warnings = ["-Wall", "-Wextra", "-Wconversion", "-std=c11", "-pedantic"]
src = ["./build/stb_image_write.o"]
libs = ["-lm"]


argc = len(sys.argv)
if argc == 1:
    print("Nothing")

elif argc == 2:
    if sys.argv[1] == "rm":
        subprocess.run(["rm", "./a.out"])
        subprocess.run(["rm", "./image.png"])

    elif sys.argv[1] == "clear":
        subprocess.run(["rm", "-rf", "./build"])
        
    else:
        build_dir = Path("./build/")
        if not build_dir.exists():
            build_dir.mkdir()
            print("Created ./build/")


        if not Path("./build/stb_image_write.o").exists():

            with open("./build/stb_image_write.c", "w") as stb_file:
                stb_file.write("#define STB_IMAGE_WRITE_IMPLEMENTATION\n")
                stb_file.write("#include \"../stb_image_write.h\"\n")

            result = subprocess.run(["gcc", "-c", "-o", "./build/stb_image_write.o", "./build/stb_image_write.c"])
            if result.returncode == 0:
                print("Compiled stb_image_write.o")
            else:
                print("Failed compiling stb_image_write.c")
                sys.exit()


        src.append(sys.argv[1])
        run = cc + warnings + src + libs
        compile_result = subprocess.run(run)
        if compile_result.returncode == 0:
            print("Compilation successful")
            run_result = subprocess.run(["./a.out"])
            if run_result.returncode == 0:
                print("Running successful")
            else:
                print(f"Exited with {run_result.returncode} code")

else:
    print("Too much arguments")
