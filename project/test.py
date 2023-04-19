import subprocess
import sys

py_args = ["C:\\Users\\Jia\\Desktop\\cs2214\\project\\tests\\{}\\{}".format(sys.argv[1], sys.argv[2])]

py_proc = subprocess.Popen(["python", "C:\\Users\\Jia\\Desktop\\cs2214\\project\\project1\\asm.py"] + py_args) 

py_exit_code = py_proc.wait()

if py_exit_code != 0:
    print("asm.py failed", py_exit_code)
    exit()

cpp_args = ["C:\\Users\\Jia\\Desktop\\cs2214\\project\\test.bin"]

cpp_proc = subprocess.Popen(["C:\\Users\\Jia\\Desktop\\cs2214\\project\\project3\\simcache.exe"] + cpp_args)

cpp_exit_code = cpp_proc.wait()

if cpp_exit_code != 0:
    print("sim.py failed", cpp_exit_code)
    exit()