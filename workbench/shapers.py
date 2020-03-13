import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
import scipy.fftpack

def test_shaper(func, a, gain):
    # test a shaper function by plotting its transfer function,
    # and its spectrum when applied to a sine wave
    x = np.arange(-1*gain, gain, 1/2048)
    n = len(x)
    m = len(a)
    print(n)
    y = np.zeros((n, m))
    z = np.zeros((n, m))

    for i in range(n):
        for j in range(m):
            y[i, j] = func(x[i], a[j])

    plt.rc('axes', prop_cycle=(cycler(color=reversed(plt.cm.coolwarm(np.linspace(0.1,0.9,m))))))

    fig, ax = plt.subplots(3, 1)

    phase = 0
    inc = 1 / (8 * np.pi)
    for i in range(n):
        for j in range(m):
            z[i, j] = func(np.sin(phase) * gain, a[j])
        phase = phase + inc
    for j in reversed(range(m)):
        ax[0].plot(y[:, j])
        ax[1].plot(z[:300, j])
        mag = np.abs(scipy.fft.rfft(z[:, j], None, 0))
        ax[2].plot(np.log10(mag))
        #ax[2].plot(mag)
    plt.savefig(func.__name__)
    plt.title(func.__name__)
    plt.show()

def shaper_tsq(x, t):
    # two-stage quadratic
    # t is softclip threshold in (0, 1) exclusive
    g = 2.0
    ax = abs(x)
    sx = np.sign(x)
    t_1 = t - 1.
    a = g / (2. * t_1)
    b = g * t - (a * t_1 * t_1)
    if ax > t:
        q = ax - 1.
        y = a * q*q + b
        return y * sx / 2
    else:
        return x * g / 2

# not working...
# def shaper_sine(x, a):
#     # sine clipper
#     # param 'a' is reciprocal of threshold in [1, (big?)]
#     z = np.pi * a
#     s = 1/np.sin(z)
#     b = 1/a
#     ax = np.abs(x)
#     sx = np.sign(x)
#     if ax > b:
#         return sx
#     return s * np.sin(z * x)

def shaper_logistic(x, a):
    # `a` in [1, big]
    ax = np.abs(x)
    return x*(ax + a) / (x*x + (a-1)*ax + 1)


def shaper_cubic(x, a):
    g = 2 ** a
    x = x * g
    ax = np.abs(x)
    sx = np.sign(x)
    if ax > (2/3):
       return sx
    else:
        return (9*x/4) - (27*(x*x*x)/16)

def shaper_expo(x, a):
    ax = np.abs(x)
    sx = np.sign(x)
    return sx* (1 - (np.abs(x-sx)**a))


test_shaper(shaper_logistic, [1, 2, 5, 10, 20], 1)
test_shaper(shaper_tsq, [0.25, 0.5, 0.75], 1)
test_shaper(shaper_cubic, [0, 0.5, 1], 1)
test_shaper(shaper_expo, [1, 2, 3, 4, 5, 6], 1)

