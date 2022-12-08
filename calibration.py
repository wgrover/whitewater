import numpy
import matplotlib.pyplot as plt

ledcWrites = numpy.array([0, 32, 64, 96, 128, 160, 192, 224, 255])
currents = numpy.array([0, 5.2, 10.5, 15.9, 20.8, 26.0, 31.1, 36.3, 41.2])
plt.scatter(ledcWrites, currents)
plt.show()