import random, math, os, shutil

random.seed()

filename = "inputfile.txt"

f = open(filename, 'w')

for i in range(0,100):
    num = (random.randrange(0,10**12))
    f.write(str(num) + "\n")
f.close()
