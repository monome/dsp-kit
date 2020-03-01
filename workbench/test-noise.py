import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack


lcg_data = np.genfromtxt('../noise/lcg_values.txt', delimiter=',')[:-1]
print (lcg_data)

pink_data = np.genfromtxt('../noise/pink_values.txt', delimiter=',')[:-1]
print (pink_data)


def plot_spectrum(name, data):
    N = len(data)
    Z = scipy.fftpack.fft(data)
    fig, ax = plt.subplots()
    ax.plot(np.arange(0, N/2), 2.0/N * np.abs(Z[:N//2]))
    plt.title(name)
    plt.show()

plot_spectrum("lcg", lcg_data)
plot_spectrum("pink", pink_data)
#pink_data = np.loadtxt('../noise/pink_values.txt', delimiter=',')
#print (pink_data)

# # Number of samplepoints
# N = 600
# # sample spacing
# T = 1.0 / 800.0
# x = np.linspace(0.0, N*T, N)
# y = np.sin(50.0 * 2.0*np.pi*x) + 0.5*np.sin(80.0 * 2.0*np.pi*x)
# yf = scipy.fftpack.fft(y)
# xf = np.linspace(0.0, 1.0/(2.0*T), N/2)
#
# fig, ax = plt.subplots()
# ax.plot(xf, 2.0/N * np.abs(yf[:N//2]))
# plt.show()
# #
# def plot_spectrum(data):
#     n = data.len
#     spectrum = scipy.fft.fft(data)
#
#     plt.pcolormesh(t, f, np.abs(Zxx), vmin=0, vmax=amp)
#     plt.show()
