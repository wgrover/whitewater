import numpy
import matplotlib.pyplot as plt
from scipy import stats
import math

ledcWrites = numpy.array([0, 32, 64, 96, 128, 160, 192, 224, 255])
currents = numpy.array([0, 5.2, 10.5, 15.9, 20.8, 26.0, 31.1, 36.3, 41.2])
fit = stats.linregress(currents, ledcWrites)
print(f"slope = {fit.slope}\tintercept = {fit.intercept}")
plt.scatter(currents, ledcWrites)
plt.plot(currents, fit.slope*currents + fit.intercept)
plt.xlabel("Current [mA]")
plt.ylabel("PWM [0-255]")
# plt.show()

# 1000 ft^3/s highest flow rate

flow = 1001
if flow < 0.1:
    current = 0
    pwm = 0
elif flow > 1000.0:
    current = 41.2
    pwm = 255
else:
    current = (math.log10(flow)+1) * 10
    pwm = round(fit.slope * current + fit.intercept)

print(f"flow = {flow}\tCurrent = {current}\tPWM = {pwm}")
