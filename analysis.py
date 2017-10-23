import numpy as np
import matplotlib.pyplot as plt
import os
import stats
import sys

def histogram_list(filename):
        hist = np.loadtxt(filename)
        dict = {}
        for idx in range(len(hist)):
                dict[hist[idx]] = 0
        for idx in range(len(hist)):
                dict[hist[idx]] += 1

        return dict.keys(), dict.values()

def plot_hist(filename):
        keys, vals = histogram_list(filename)
        plt.title(filename)
        plt.plot(keys, vals, 'r.')


def get_stats(filename):
        list = np.loadtxt(filename)
        return stats.Stats(list)

#print os.listdir("magSqFiles")

actualMagStDev = get_stats("coarsemagnetizations/_single_mag_high")[2]

difference = 100000000
retVal = ""

for f in os.listdir("magSqFiles/"):
        currDifference = abs(get_stats("magSqFiles/" + f)[2] - actualMagStDev)
        if currDifference < difference:
                difference = currDifference
                retVal = f
print("coarseGrained J: " + str(0.35) + " native J: " + retVal)

domain = [.1, .15, .2, .25, .3, .35, .4,  .55, .6, .65, .7, .75, .8]
rng = [0.06, 0.2, 0.18, 0.25, 0.27, 0.27, 0.31, 0.43, 0.4, 0.22, 0.34, 0.44, 0.34]

plt.plot(domain, rng, 'r.')
plt.show()
       
#plot_hist("magSqFiles")
#print("===========================")
#print get_stats("coarsemagnetizations/_single_mag_high")[2],

#plot_hist("coarsemagnetizations/_single_mag_low")





#plt.legend()

#plt.show()

