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
362.000000, 380.797011, 375.820209, 374.614400,
410.040272, 408.831016, 427.801397, 425.424494,
496.213314, 512.435046, 504.838050, 520.332149,
565.779890, 581.619554, 591.555807, 610.878910,
649.500832, 700.232784, 679.793622, 657.285611,
652.481655, 709.642439, 746.108626, 738.226326,
769.242166, 880.385998, 703.596793, 770.057237,
833.202387, 952.096864, 759.763736, 830.326801,
897.162608, 1023.807730, 815.930680, 890.596365,
961.122829, 1095.518597, 872.097623, 950.865930,
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
