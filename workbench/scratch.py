import numpy as np
import matplotlib.pyplot as plt
import scipy.fft

n = 1024
x = np.zeros((n, 1))
phase = 0
inc =  1000 / (2 * np.pi * n)
for i in range(n):
    x[i] = np.sin(phase)
    phase = phase + inc

fig, ax = plt.subplots(2, 1)
ax[0].plot(x)

z = scipy.fft.rfft(x, None, 0)
#ax[1].plot(np.abs(z))
ax[1].plot(np.abs(z))

plt.show()