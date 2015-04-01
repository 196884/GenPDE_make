import matplotlib.pyplot as plt
import numpy as np

def loadData(file_name):
    lines = [ line.strip() for line in open(file_name, "r")]
    return lines

def plotVector(v):
    n  = len(v)
    xs = np.arange(n)
    plt.plot(xs, v)
    plt.show()

def extractSlice(l, size, stride, offset, skip):
    return [float(l[offset + k + skip]) for k in [stride * i for i in np.arange(size)]]

def plotSlice(l, size, stride, offset, skip):
    v = extractSlice(l, size, stride, offset, skip)
    xs = np.arange(size)
    plt.plot(xs, v)
    plt.show()

data1 = "pricer1BeforeAVRm.dat"
data2 = "pricer1AfterAVRm.dat"


