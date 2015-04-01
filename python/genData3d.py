import math
fd = open("test.dat", "w")
for x in range(-100, 100, 2):
    for y in range(-100, 100, 2):
        xd = 0.1 * x
        yd = 0.1 * y
        zd = xd * xd + yd * yd
        fd.write("{} {} {}\n".format(xd, yd, zd))
fd.close()
