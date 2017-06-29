#this python script spits out a text file containing comma-separated values of a log-spaced array
import matplotlib.pyplot as plt
import numpy as np

f = open('output.txt','w')
array = np.logspace(-6, 1, num=1000)
text = ", ".join(str(x) for x in array)
f.write(text)
f.close()
