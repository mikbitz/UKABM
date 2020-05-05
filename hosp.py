#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May  5 13:40:50 2020

@author: moke
"""
#Allocate hospitals approximately proportional to population
#by using 10km binned worldpop data
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib import pyplot as plt
import pandas as pd
import numpy as np

base_dir="/home/moke/ukmodel/worldpop/"
varName="gbr_ppp_2020UKnatgrid10km"

fp1=base_dir+varName+".asc"
head=pd.read_table(fp1,nrows=6,header=None,sep=' ')
nrows=head[1][1]

values=np.loadtxt(fp1,skiprows=6,max_rows=int(nrows))
#exlcude missing value (-9999)
print(np.sum(values,where=values>0))
fig=plt.figure()
fig.set_size_inches(3.5, 4., forward=True)

ax=fig.add_axes()

plt.subplots_adjust(left=0.1, right=0.9, bottom=0., top=1.0)

#scale assuming roughly 200 total hosps
h=200.*values/np.sum(values,where=values>0)
p=plt.imshow(h,vmin=0,vmax=0.1,cmap='bwr')
rg = np.random.default_rng(1)
#threshold to find locations
y,x=np.where(h>0.24516)
#convert to floats and add some randomness
x=x.astype(float)+rg.random(200)-0.5
y=y.astype(float)+rg.random(200)-0.5
plt.scatter(x,y,c='g',s=1)
plt.colorbar(p)
plt.tight_layout()
plt.show()
#scale to cellsize and add lower corner - remember y runs from top of image
xox=x*head[1][4]+head[1][2]
yoy=(124-y)*head[1][4]+head[1][3]
sox=["%.0f" % z for z in xox]
soy=["%.0f" % z for z in yoy]
out=np.array(range(600), dtype='U8').reshape(200,3)
out[:,0]='Hospital'
out[:,1]=sox
out[:,2]=soy
pd.DataFrame(out,columns=['name', 'x', 'y']).to_csv("places.csv",index=False)