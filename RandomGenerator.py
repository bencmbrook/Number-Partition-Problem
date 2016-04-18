import random, math, os, shutil

inputSize = 50

random.seed()
shutil.rmtree("input")
os.mkdir("input")
os.chdir("input")

for x in range(1,inputSize+1):
    filename = "in" + str(x) + ".txt"
    f = open(filename, 'w')
    for i in range(0,100):
        num = (random.randrange(0,10**12))
        f.write(str(num) + "\n")
    f.close()
