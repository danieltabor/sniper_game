#!/usr/bin/python

import os
import sys
import subprocess
import shutil

def main():
    exe_path = sys.argv[1]
    dst_path = sys.argv[2]
    if not os.path.exists(dst_path):
        os.makedirs(dst_path)

    print "[dist added] %s" % exe_path
    shutil.copy(exe_path,dst_path)

    proc = subprocess.Popen(["ldd",exe_path],stdout=subprocess.PIPE)
    proc.wait()
    while True:
        line = proc.stdout.readline()
        if not len(line):
            break
        if "=>" in line:
            line = line.strip().split(" ")
            src_path = line[2]
            print "[dist added] %s" % src_path
            shutil.copy(src_path,dst_path)

if __name__ == "__main__":
    main()
