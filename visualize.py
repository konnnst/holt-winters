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
    362.000000, 384.933750, 431.813419, 341.502817,
    381.846264, 408.912981, 497.603315, 387.601767,
    472.621759, 512.846859, 581.702984, 474.587265,
    543.701686, 581.857258, 680.553036, 557.666515,
    627.697083, 706.652235, 772.717011, 592.952335,
    626.878512, 724.558143, 853.401541, 662.010758,
    676.001581, 789.916838, 632.644535, 644.520371,
    715.247788, 835.120463, 668.337487, 680.377588,
    754.493994, 880.324088, 704.030439, 716.234804,
    793.740201, 925.527713, 739.723391, 752.092021,
]

statsmodels = [
]

my100 = [
    362.000000, 382.584744, 372.713472, 372.594570,
    415.481219, 406.434183, 429.655808, 422.856594,
    514.457111, 509.781750, 502.127872, 517.917378,
    591.680060, 578.348886, 587.541376, 608.607552,
    683.042423, 702.564712, 666.915542, 646.850397,
    681.954775, 720.451791, 736.799318, 722.243433,
    736.614504, 859.730562, 687.769714, 699.899245,
    775.860711, 904.934187, 723.462666, 735.756461,
    815.106918, 950.137812, 759.155617, 771.613678,
    854.353125, 995.341437, 794.848569, 807.470894,
]

my2 = [

]


plt.plot(original)
plt.plot(my100)
plt.plot(my2)
plt.plot(myopt)

plt.show()
