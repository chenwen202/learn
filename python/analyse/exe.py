import sys
import subprocess


file =  "E:/work/learn/test/bin/main.exe"
child = subprocess.Popen([file],stdin=subprocess.PIPE)
print(child.poll())
child.communicate(input=b"sss")
child.terminate()

