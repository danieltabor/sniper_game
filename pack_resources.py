#!/usr/bin/python
import sys

def main():
    c_fp = open("resources.c","wb")
    h_fp = open("resources.h","wb")
    h_fp.write('#include "res.h"\n')
    h_fp.write('#define RESOURCE_COUNT %d\n' % len(sys.argv[1:]))
    c_fp.write("#define __RESOURCES_C__\n")
    c_fp.write('#include "res.h"\n')
    count = 0
    lengths = []
    for path in sys.argv[1:]:
        f = open(path,"rb")
        data = f.read()
        defname = path.upper().replace(".","_")
        h_fp.write("#define %s %d\n" % (defname,count))
        c_fp.write("static char resource%d[] = {" % count)
        for i in xrange(len(data)):
            c_fp.write("0x%02X" % ord(data[i]))
            if i != len(data)-1:
                c_fp.write(",")
        c_fp.write("};\n")
        lengths.append(len(data))
        count += 1
    h_fp.write("#ifndef __RESOURCES_C__\n")
    h_fp.write("extern resource_t resources[];\n")
    h_fp.write("#endif\n")
    c_fp.write("resource_t resources[] = {\n")
    for i in xrange(len(lengths)):
        c_fp.write("(resource_t){.len=%d,.data=resource%d}" % (lengths[i],i))
        if i != len(lengths)-1:
            c_fp.write(",\n")
        else:
            c_fp.write("\n")
    c_fp.write("};\n")

if __name__ == "__main__":
    main()
