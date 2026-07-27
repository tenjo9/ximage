#!/usr/bin/python3
import sys

def parse_obj(filename):

    with open(filename, 'r') as obj:
        lines = obj.readlines()

    v  = []
    vn = []
    vt = []
    f  = []

    for line in lines:
        line = line.split()
        if len(line) == 0:
            continue
        if line[0] == '#':
            continue
        if line[0] == 'v':
            v.append(line[1:])
        if line[0] == 'vn':
            vn.append(line[1:])
        if line[0] == 'vt':
            vt.append(line[1:])
        if line[0] == 'f':
            f.append([vert.split('/') for vert in line[1:]])

    return { 'v': v, 'vn': vn, 'vt': vt, 'f': f }


def write_obj(filename, obj, name):
    with open(filename, "w") as c:
        c.write(f'#include "3d/ximage_3d.h"\n');
        c.write(f"Model {name} = {{\n");


        c.write("    .pos = {0},\n\n")


        c.write(f"    .vcount = {len(obj['v'])},\n    .vertices = (v3d[]) {{\n")
        for v in obj["v"]:
            c.write(f"        {{ {v[0]}, {v[1]}, {v[2]} }},\n")
        c.write(f"    }},\n\n")
        

        c.write(f"    .ncount = {len(obj['vn'])},\n    .normales = (v3d[]) {{\n")
        for vn in obj["vn"]:
            c.write(f"        {{ {vn[0]}, {vn[1]}, {vn[2]} }},\n")
        c.write(f"    }},\n\n")


        c.write(f"    .tcount = {len(obj['vt'])},\n    .uv = (v2d[]) {{\n")
        for vt in obj["vt"]:
            c.write(f"        {{ {vt[0]}, {vt[1]} }},\n")
        c.write(f"    }},\n\n")


        c.write(f"    .fcount = {len(obj['f'])},\n    .faces = (Face[]) {{\n")
        for f in obj["f"]:
            c.write("        {\n")
            c.write(f"            {int(f[0][0])-1}, {int(f[0][1])-1}, {int(f[0][2])-1},\n")
            c.write(f"            {int(f[1][0])-1}, {int(f[1][1])-1}, {int(f[1][2])-1},\n")
            c.write(f"            {int(f[2][0])-1}, {int(f[2][1])-1}, {int(f[2][2])-1},\n")
            c.write("        },\n")
        c.write(f"    }},\n\n")
        c.write(f"}};\n\n")

if len(sys.argv) != 4:
    print("Using:")
    print("    ./obj2c.py <input> <output> <name>")
    sys.exit()

ifile = sys.argv[1]
ofile = sys.argv[2]
name = sys.argv[3]

write_obj(ofile, parse_obj(ifile), name)
