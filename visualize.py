import matplotlib.pyplot as plt

original = [
    362,
    385,
    432,
    341,
    382,
    409,
    498,
    387,
    473,
    513,
    582,
    474,
    544,
    582,
    681,
    557,
    628,
    707,
    773,
    592,
    627,
    725,
    854,
    661,
]

myopt = [
362.000000, 380.797011, 375.656006, 374.708457,
410.362463, 408.691099, 427.805381, 425.281894,
496.582959, 511.338866, 503.630582, 519.399875,
565.340193, 579.379818, 588.932518, 608.541036,
647.650141, 696.244139, 674.678345, 653.164117,
649.787356, 705.639649, 739.852277, 732.063136,
761.194525, 866.954537, 687.737622, 744.320966,
801.213480, 911.942410, 722.968549, 781.968360,
841.232435, 956.930283, 758.199475, 819.615754,
881.251390, 1001.918156,
]

my1 = [
]
my08 = [
]

plt.plot(original)
plt.plot(my08)
plt.plot(my1)
plt.plot(myopt)

plt.show()
