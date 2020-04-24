#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 22 16:33:18 2020

@author: moke
"""

import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib import pyplot as plt
import pandas as pd
import numpy as np

fp1="/home/moke/ukmodel/population.tasc"
head=pd.read_table(fp1,nrows=6,header=None,sep=' ')
nrows=head[1][1]
#frame numbers start at 0
frameNo=0
date=np.loadtxt(fp1,skiprows=6+(int(nrows)+1)*frameNo,max_rows=1,dtype=str)
data=np.loadtxt(fp1,skiprows=7+(int(nrows)+1)*frameNo,max_rows=int(nrows))
print(np.sum(data))
p=plt.imshow(data,vmin=0,vmax=5,cmap='bwr')
plt.colorbar(p)
plt.tight_layout()
plt.show()