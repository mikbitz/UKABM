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

base_dir="/home/moke/ukmodel/output/"
varName="population"
runNumber="0006"
experiment="experiment.testing"

fp1=base_dir+experiment+"/run_"+runNumber+"/"+varName+".tasc"
head=pd.read_table(fp1,nrows=6,header=None,sep=' ')
nrows=head[1][1]
#frame numbers start at 0
frameNo=0
date=np.loadtxt(fp1,skiprows=6+(int(nrows)+1)*frameNo,max_rows=1,dtype=str)
values=np.loadtxt(fp1,skiprows=7+(int(nrows)+1)*frameNo,max_rows=int(nrows))
print(np.sum(values))
fig=plt.figure()
fig.set_size_inches(3.5, 4., forward=True)
#fig.suptitle("A title")
ax=fig.add_axes()

plt.subplots_adjust(left=0.1, right=0.9, bottom=0., top=1.0)

p=plt.imshow(values,vmin=0,vmax=1400,cmap='bwr')
plt.colorbar(p)
plt.tight_layout()
plt.show()
