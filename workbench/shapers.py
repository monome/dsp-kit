import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
import scipy.fftpack

def test_shaper(func, a, gain):
    # test a shaper function by plotting its transfer function,
    # and its spectrum when applied to a sine wave
    x = np.arange(-1*gain, gain, 1/4096)
    n = len(x)
    m = len(a)
    print(n)
    y = np.zeros((n, m))
    z = np.zeros((n, m))

    for i in range(n):
        for j in range(m):
            y[i, j] = func(x[i], a[j])

    plt.rc('axes', prop_cycle=(cycler(color=reversed(plt.cm.copper(np.linspace(0.1,0.9,m))))))

    fig, ax = plt.subplots(3, 1)

    phase = 0
    inc = 1 / (2**2*np.pi)
    for i in range(n):
        for j in range(m):
            z[i, j] = func(np.sin(phase) * gain, a[j])
        phase = phase + inc
    print(func.__name__)
    for j in reversed(range(m)):
        ax[0].plot(y[:, j], linewidth=0.5)
        ax[1].plot(z[:300, j], linewidth=0.5)
        #ax[1].plot(z[:, j])
        #zj = np.concatenate(z[:,j], np.zeros(n,1))
        #mag = np.abs(scipy.fft.rfft(zj, None, 0))
        mag = np.abs(scipy.fft.rfft(z[:, j], n*4, 0, "ortho"))
        db =np.log10(mag) * 20
        print([np.amin(mag), np.amax(mag)])
        print([np.amin(db), np.amax(db)])
        ax[2].plot(db, linewidth=0.5)
        #ax[2].plot(mag)
    print("")
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
    ax = np.abs(x)
    return x*(ax + a) / (x*x + (a-1)*ax + 1)

def shaper_cubic(x, a):
    g = 2 ** a
    x = x * g
    y = ((3*x/2) - ((x*x*x)/2))
    return y / g

def shaper_expo(x, a):
    ax = np.abs(x)
    sx = np.sign(x)
    return sx* (1 - (np.abs(x-sx)**a))


def shaper_sine(x, a):
    x = x * (2 ** a)
    return np.sin(np.pi*x/2)


def shaper_reciprocal(x, a):
    return np.sign(x)*(1-(1/(np.abs(a*x)+1)))


def shaper_tanh(x, a):
    x = x * (2 ** a)
    return np.tanh(x)

test_shaper(shaper_logistic, [1, 2, 5, 10, 20], 1)
test_shaper(shaper_tsq, [0.25, 0.5, 0.75], 1)
test_shaper(shaper_cubic, [-1, -0.5, 0, 0.25, 0.5], 1)
test_shaper(shaper_expo, [1, 2, 3, 4, 5, 6], 1)
test_shaper(shaper_sine, [-1, -0.5, -0.25, 0, 0.25, 0.5], 1)
test_shaper(shaper_reciprocal, [1, 2, 4, 8, 16, 32], 1)

test_shaper(shaper_tanh, [0, 0.5, 1, 2, 3, 4, 5, 6], 1)

