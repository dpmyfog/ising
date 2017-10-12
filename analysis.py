import numpy as np
import matplotlib.pyplot as plt

def hist_configs():
        histogramArray = np.loadtxt('configurationMap')
        domain = range(0, 512)
        plt.plot(domain, histogramArray)
        plt.show()


hist_configs()
        
	
