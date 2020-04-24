#import matplotlib
#matplotlib.use('Qt5Agg')
from matplotlib import pyplot as plt
from matplotlib import animation
import pandas as pd
import numpy as np
#from scipy import ndimage
fig=plt.figure()
fig.set_size_inches(3.5, 4., forward=True)
plt.subplots_adjust(left=0.1, right=0.9, bottom=0., top=1.0)
plt.show()
#base_dir="/home/moke/working/"
#varName="totalInfected"
#runNumber="119"
#experiment="experiment.testCovidUK"

fp1="/home/moke/ukmodel/infections.tasc"
head=pd.read_table(fp1,nrows=6,header=None,sep=' ')
nrows=head[1][1]
t =int(head[1][5])
#frame numbers start at 0
frameNo=0
date=np.loadtxt(fp1,skiprows=6+(int(nrows)+1)*frameNo,max_rows=1,dtype=str)
values=np.loadtxt(fp1,skiprows=7+(int(nrows)+1)*frameNo,max_rows=int(nrows))

img=plt.imshow(values,vmin=0,vmax=20,cmap='viridis')
def animate(i):
    print(i)
    values=np.loadtxt(fp1,skiprows=7+(int(nrows)+1)*i,max_rows=int(nrows))

    img.set_data(values)
    fig.canvas.draw()
    fig.canvas.flush_events()
    return img
anim = animation.FuncAnimation(fig, animate, frames=t, interval=50)
# Set up formatting for the movie files
#Writer = animation.writers['ffmpeg']
#writer = Writer(fps=15, metadata=dict(artist='Total Stock Biomass'), bitrate=1800)
#anim.save('UKStockBiomass.mp4', writer=writer)

