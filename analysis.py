import numpy as np
import matplotlib.pyplot as plt

def hist_configs():
        theoHistogramArray = np.loadtxt('configurationMap')
        expHistogramArray_5 = np.loadtxt('endStates5')
        expHistogramArray_50 = np.loadtxt('endStates50')
        expHistogramArray_500 = np.loadtxt('endStates500')
        domain = range(0, 512)
        plt.plot(domain, theoHistogramArray)
        plt.figure()
        #plt.plot(domain, expHistogramArray_5)
        #plt.plot(domain, expHistogramArray_50)
        plt.plot(domain, expHistogramArray_500)
        plt.show()
        plt.matshow(np.loadtxt('exampleSnapshot'))
        plt.show()

hist_configs()
        
	
