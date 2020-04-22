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

fp1="/home/moke/ukmodel/infections.tasc"
head=pd.read_table(fp1,nrows=5,header=None,sep=' ')
nrows=head[1][1]
frameNo=2
date=np.loadtxt(fp1,skiprows=5+(int(nrows)+1)*frameNo,max_rows=1,dtype=str)
data=np.loadtxt(fp1,skiprows=6+(int(nrows)+1)*frameNo,max_rows=int(nrows))
p=plt.imshow(data,vmin=0,vmax=1,cmap='bwr')
plt.colorbar(p)
plt.tight_layout()
plt.show()