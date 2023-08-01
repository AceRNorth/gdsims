
import numpy as np
import matplotlib.pyplot as plt

totals = np.ones((1, 7))
totals1 = np.zeros((1, 7))

totals = np.loadtxt("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Totals1run1.txt", skiprows=2)

times = totals[:, 0]
total_males = totals[:, 1:]

#print(times)
#print(total_males)

plt.plot(times, total_males[:, 0], label={"M_{WW}"})
plt.plot(times, total_males[:, 1], label={"M_{WD}"})
plt.plot(times, total_males[:, 2], label={"M_{DD}"})
plt.plot(times, total_males[:, 3], label={"M_{WR}"})
plt.plot(times, total_males[:, 4], label={"M_{RR}"})
plt.plot(times, total_males[:, 5], label={"M_{DR}"})
plt.legend()