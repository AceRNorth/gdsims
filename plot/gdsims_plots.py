import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.colors as mcolors

#%% Plot global output (totals) by genotype from model
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set13\\")

totals = np.loadtxt("Totals13run1.txt", skiprows=2, ndmin=2)

times = totals[:, 0]
total_females = totals[:, 1:]

plt.figure()
plt.title("Total females across the area by genotype - set 13 run 1")
plt.xlabel("Day")
plt.ylabel("Total number of individuals")
plt.plot(times, total_females[:, 0], label="$F_{WW}$", linestyle="solid", color="hotpink")
plt.plot(times, total_females[:, 1], label="$F_{WD}$", linestyle="dashed", color="mediumturquoise")
plt.plot(times, total_females[:, 2], label="$F_{DD}$", linestyle="solid", color="royalblue")
plt.plot(times, total_females[:, 3], label="$F_{WR}$", linestyle="dotted", color="slategray")
plt.plot(times, total_females[:, 4], label="$F_{RR}$", linestyle="solid", color="rebeccapurple")
plt.plot(times, total_females[:, 5], label="$F_{DR}$", linestyle="dashed", color="darkviolet")
# # all genotypes
# plt.plot(times, np.sum(total_females, axis=1).tolist(),
#           label="$F_{WW}$+$F_{WD}$+\n$F_{DD}$+$F_{WR}$+\n$F_{RR}$+$F_{DR}$",
#           linestyle="solid", color="black")
# # capable of malaria transmission
# plt.plot(times, np.sum(total_females[:, (0, 1, 3)], axis=1).tolist(),
#           label="$F_{WW}$+$F_{WD}$+\n$F_{WR}$",
#           linestyle="dashed", color="crimson")
plt.legend()

#%% Plot global output (totals) - allele frequency from model

os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set13\\")

totals = np.loadtxt("Totals13run1.txt", skiprows=2, ndmin=2)

times = totals[:, 0]
total_females = totals[0:, 1:]
WW = total_females[:, 0]
WD = total_females[:, 1]
DD = total_females[:, 2]
WR = total_females[:, 3]
RR = total_females[:, 4]
DR = total_females[:, 5]

plt.figure()
plt.title("Total females across the area by allele frequency - set 13 run 1")
plt.xlabel("Day")
plt.ylabel("Total number of individuals")

for line in range(0, 3):  # keep same colours for same type of line
    labels = ["wild",
              "drive",
              "r2 (non-functional) resistance"]
    colours = ["hotpink",
               "royalblue",
               "rebeccapurple"]

    y = []
    for i in range(0, len(WW)):
        top = [(WW[i] + WD[i] + WR[i]),  # wild
               (WD[i] + DD[i] + DR[i]),  # drive
               (WR[i] + RR[i] + DR[i])]  # r2 resistance
        bottom = WW[i] + WD[i] + DD[i] + WR[i] + RR[i] + DR[i]
        if bottom == 0:
            result = 0
        else:
            result = top[line] / bottom
        y.append(result)
    plt.plot(times, y, label=labels[line], color=colours[line])

plt.legend()

#%% Plot coordinates of patches

os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set1")

coords = np.loadtxt("CoordinateList1run1.txt", skiprows=2, ndmin=2)

x = coords[:, 1]
y = coords[:, 2]

fig = plt.figure()
axes = fig.add_subplot(111)
plt.title("Patch locations")
plt.xlabel("x (km)")
plt.ylabel("y (km)")
if len(x) == 1:
    axes.set_xlim(x - x/2, x + x/2)
else:
    axes.set_xlim(np.amin(x), np.amax(x))
if len(y) == 1:
    axes.set_ylim(y - y/2, y + y/2)
else:
    axes.set_ylim(np.amin(y), np.amax(y))
plt.scatter(x, y, marker='.', color="peru")
axes.set_aspect('equal') # set equal aspect ratio for both axes

#%% Spatial plot of total population size on one day

fig, axes = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set1")

ind, x, y = np.loadtxt("CoordinateList1run1.txt", skiprows=2, ndmin=2, unpack=True)
numRecPats = len(x)
localData = np.loadtxt("LocalData1run1.txt", skiprows=2, ndmin=2)  # get populations

if len(localData) > numRecPats:
    recIntervalLocal = int(localData[numRecPats, 0]) - int(localData[0, 0])
else:
    recIntervalLocal = 0

# set these
t = 0 # timestep starting from 0
recStart = 0

# get populations on one day, t+1 because always ignore initialisation day
simDay = int(localData[t*numRecPats, 0])
localDataDay = localData[t*numRecPats:((t+1)*numRecPats), 2:8]

# calculate total population for all genotypes in each patch
totPops = np.zeros(len(x))
for pat in range(0, len(x)): 
    patchData = localDataDay[pat, :]
    for i in range(0, len(patchData)):
        totPops[pat] += patchData[i]

# find maximum and minimum population values in whole simulation for colour map bounds
maxPop = np.amax(localData)
minPop = np.amin(localData)

scat = axes.scatter(x, y, c=totPops, cmap="cividis_r", vmin=minPop, vmax=maxPop, marker='.')
cbar = fig.colorbar(scat, ax=axes, label='Total population size')
annotation = fig.text(x=0.1, y=0.9, s="t = {}".format((t * recIntervalLocal) + recStart))
axes.set_xlabel("x")
axes.set_ylabel("y")
if len(x) == 1:
    axes.set_xlim(x - x/2, x + x/2)
else:
    axes.set_xlim(np.amin(x), np.amax(x))
if len(y) == 1:
    axes.set_ylim(y - y/2, y + y/2)
else:
    axes.set_ylim(np.amin(y), np.amax(y))
axes.set_aspect('equal') # set equal aspect ratio for both axes
axes.minorticks_on()  # need it for animation saving to work
axes.set_title("Population sizes - set 1 run 1")


#%% Spatial animation of total population size

fig, axes = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set1")

ind, x, y = np.loadtxt("CoordinateList1run1.txt", skiprows=2, ndmin=2, unpack=True)
numRecPats = len(x)
localData = np.loadtxt("LocalData1run1.txt", skiprows=2, ndmin=2)  # get populations

if len(localData) > numRecPats:
    recIntervalLocal = int(localData[numRecPats, 0]) - int(localData[0, 0])
else:
    recIntervalLocal = 0

# set these
t = 0 # timestep starting from 0
recStart = 0
recEnd = 1000

# get populations on one day, t+1 because always ignore initialisation day
simDay = int(localData[t*numRecPats, 0])
localDataDay = localData[t*numRecPats:((t+1)*numRecPats), 2:8]

# calculate total population for all genotypes in each patch
totPops = np.zeros(len(x))
for pat in range(0, len(x)): 
    patchData = localDataDay[pat, :]
    for i in range(0, len(patchData)):
        totPops[pat] += patchData[i]

# find maximum and minimum population values in whole simulation for colour map bounds
maxPop = np.amax(localData)
minPop = np.amin(localData)

scat = axes.scatter(x, y, c=totPops, cmap="cividis_r", vmin=minPop, vmax=maxPop, marker='.')
cbar = fig.colorbar(scat, ax=axes, label='Total population size')
annotation = fig.text(x=0.1, y=0.9, s="t = {}".format(simDay))
axes.set_xlabel("x")
axes.set_ylabel("y")
if len(x) == 1:
    axes.set_xlim(x - x/2, x + x/2)
else:
    axes.set_xlim(np.amin(x), np.amax(x))
if len(y) == 1:
    axes.set_ylim(y - y/2, y + y/2)
else:
    axes.set_ylim(np.amin(y), np.amax(y))
axes.set_aspect('equal') # set equal aspect ratio for both axes
axes.minorticks_on()  # need it for animation saving to work
axes.set_title("Population sizes - set 1 run 1")

def updatePopSize(step):
    simDay = int(localData[step*numRecPats, 0])
    localDataDay = localData[step*len(x):((step+1)*len(x)), 2:8]
    
    # calculate total population for all genotypes in each patch
    totPops = np.zeros(len(x))
    for pat in range(0, len(x)): 
        patchData = localDataDay[pat, :]
        for i in range(0, len(patchData)):
            totPops[pat] += patchData[i]

    scat.set_array(totPops) # update the scatter point colours according to new tot_pops
    annotation.set_text("t = {}".format(simDay))
    return scat

numFrames = int((recEnd - recStart) / recIntervalLocal) + 1 # +1 because range(frames) below, assumes recSitesFreq=1

anim = animation.FuncAnimation(fig=fig, func=updatePopSize, frames=numFrames, interval=500)
#anim.save("set100_pop_anim.gif")
plt.show()


#%% Spatial plot of gene drive allele frequency on one day

fig, axes = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set1")

ind, x, y = np.loadtxt("CoordinateList1run1.txt", skiprows=2, ndmin=2, unpack=True)
numRecPats = len(x)
localData = np.loadtxt("LocalData1run1.txt", skiprows=2, ndmin=2)  # get populations

if len(localData) > numRecPats:
    recIntervalLocal = int(localData[numRecPats, 0]) - int(localData[0, 0])
else:
    recIntervalLocal = 0

# set these
t = 0 # timestep starting from 0
recStart = 0

# get populations on one day, t+1 because always ignore initialisation day
simDay = int(localData[t*numRecPats, 0])
localDataDay = localData[t*numRecPats:((t+1)*numRecPats), 2:8]

WW = localDataDay[:, 0]
WD = localDataDay[:, 1]
DD = localDataDay[:, 2]
WR = localDataDay[:, 3]
RR = localDataDay[:, 4]
DR = localDataDay[:, 5]

# calculate drive allele frequency for each patch
driveFreq = np.zeros(numRecPats)
for pat in range(0, numRecPats):
    tot = WW[pat] + WD[pat] + DD[pat] + WR[pat] + RR[pat] + DR[pat]
    if tot == 0:
        driveFreq[pat] = -2  # assign different distinguishable value for no-population patches
    elif tot == WW[pat]:
        driveFreq[pat] = -0.5
    else:
        driveFreq[pat] = (WD[pat] + (2*DD[pat]) + DR[pat]) / (2*tot)

# define discrete colourmap
viridisR = mpl.colormaps['viridis_r'].resampled(14)
# don't want edge colours in viridis so there's enough contrast with two
# additional cmap colours
viridisColours = viridisR(np.linspace(0.2, 0.8, 10))
# add colours for no-population patch and wild-population patch
allColours = np.vstack((mcolors.to_rgba("lightgray"), mcolors.to_rgba("hotpink"), viridisColours))
cmap = mcolors.ListedColormap(allColours)
bounds = [-2, -1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
cnorm = mcolors.BoundaryNorm(bounds, cmap.N)

# make a scatter plot with drive frequency colour map
scat = axes.scatter(x, y, c=driveFreq, cmap=cmap, norm=cnorm, marker='.')
colorbar = fig.colorbar(scat, ax=axes)
colorbar.set_label('Drive allele frequency', labelpad=-10)  # reduce distance to colorbar label
colorbar.ax.set_yticks([-2, -1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0],
                            labels=['no pop', 'wild', '0.0', '0.1', '0.2', '0.3', '0.4',
                                    '0.5', '0.6', '0.7', '0.8', '0.9', '1.0'])
labels = colorbar.ax.get_yticklabels()
labels[0].set_verticalalignment('bottom')  # align first label text above the tick
labels[1].set_verticalalignment('bottom')
annotation = fig.text(x=0.1, y=0.9, s='t = {}'.format(simDay))
axes.set_xlabel("x")
axes.set_ylabel("y")
if len(x) == 1:
    axes.set_xlim(x - x/2, x + x/2)
else:
    axes.set_xlim(np.amin(x), np.amax(x))
if len(y) == 1:
    axes.set_ylim(y - y/2, y + y/2)
else:
    axes.set_ylim(np.amin(y), np.amax(y))
axes.set_aspect('equal') # set equal aspect ratio for both axes
axes.minorticks_on()  # need it for animation saving to work
axes.set_title("Drive allele frequencies - set 1 run 1")


#%% Spatial animation of gene drive allele frequency

fig, axes = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set1")

ind, x, y = np.loadtxt("CoordinateList1run1.txt", skiprows=2, ndmin=2, unpack=True)
numRecPats = len(x)
localData = np.loadtxt("LocalData1run1.txt", skiprows=2, ndmin=2)  # get populations

if len(localData) > numRecPats:
    recIntervalLocal = int(localData[numRecPats, 0]) - int(localData[0, 0])
else:
    recIntervalLocal = 0

# set these
t = 0 # timestep starting from 0
recStart = 0

# get populations on one day, t+1 because always ignore initialisation day
simDay = int(localData[t*numRecPats, 0])
localDataDay = localData[t*numRecPats:((t+1)*numRecPats), 2:8]

WW = localDataDay[:, 0]
WD = localDataDay[:, 1]
DD = localDataDay[:, 2]
WR = localDataDay[:, 3]
RR = localDataDay[:, 4]
DR = localDataDay[:, 5]

# calculate drive allele frequency for each patch
driveFreq = np.zeros(numRecPats)
for pat in range(0, numRecPats):
    tot = WW[pat] + WD[pat] + DD[pat] + WR[pat] + RR[pat] + DR[pat]
    if tot == 0:
        driveFreq[pat] = -2  # assign different distinguishable value for no-population patches
    elif tot == WW[pat]:
        driveFreq[pat] = -0.5
    else:
        driveFreq[pat] = (WD[pat] + (2*DD[pat]) + DR[pat]) / (2*tot)

# define discrete colourmap
viridisR = mpl.colormaps['viridis_r'].resampled(14)
# don't want edge colours in viridis so there's enough contrast with two
# additional cmap colours
viridisColours = viridisR(np.linspace(0.2, 0.8, 10))
# add colours for no-population patch and wild-population patch
allColours = np.vstack((mcolors.to_rgba("lightgray"), mcolors.to_rgba("hotpink"), viridisColours))
cmap = mcolors.ListedColormap(allColours)
bounds = [-2, -1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
cnorm = mcolors.BoundaryNorm(bounds, cmap.N)

# make a scatter plot with drive frequency colour map
scat = axes.scatter(x, y, c=driveFreq, cmap=cmap, norm=cnorm, marker='.')
colorbar = fig.colorbar(scat, ax=axes)
colorbar.set_label('Drive allele frequency', labelpad=-10)  # reduce distance to colorbar label
colorbar.ax.set_yticks([-2, -1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0],
                            labels=['no pop', 'wild', '0.0', '0.1', '0.2', '0.3', '0.4',
                                    '0.5', '0.6', '0.7', '0.8', '0.9', '1.0'])
labels = colorbar.ax.get_yticklabels()
labels[0].set_verticalalignment('bottom')  # align first label text above the tick
labels[1].set_verticalalignment('bottom')
annotation = fig.text(x=0.1, y=0.9, s='t = {}'.format((t * recIntervalLocal) + recStart))
axes.set_xlabel("x")
axes.set_ylabel("y")
if len(x) == 1:
    axes.set_xlim(x - x/2, x + x/2)
else:
    axes.set_xlim(np.amin(x), np.amax(x))
if len(y) == 1:
    axes.set_ylim(y - y/2, y + y/2)
else:
    axes.set_ylim(np.amin(y), np.amax(y))
axes.set_aspect('equal') # set equal aspect ratio for both axes
axes.minorticks_on()  # need it for animation saving to work
axes.set_title("Drive allele frequencies - set 1 run 1")

def updateDriveFreq(step):
    simDay = int(localData[(step*numRecPats), 0])
    localDataDay = localData[(step*numRecPats):((step + 1)*numRecPats), 2:8]
    
    WW = localDataDay[:, 0]
    WD = localDataDay[:, 1]
    DD = localDataDay[:, 2]
    WR = localDataDay[:, 3]
    RR = localDataDay[:, 4]
    DR = localDataDay[:, 5]

    driveFreq = np.zeros(numRecPats)
    for pat in range(0, numRecPats):
        tot = WW[pat] + WD[pat] + DD[pat] + WR[pat] + RR[pat] + DR[pat]
        if tot == 0:
            driveFreq[pat] = -2
        elif tot == WW[pat]:
            driveFreq[pat] = -0.5
        else:
            driveFreq[pat] = (WD[pat] + (2*DD[pat]) + DR[pat]) / (2*tot)

    scat.set_array(driveFreq) # update the scatter point colours according to new driveFreq
    annotation.set_text("t = {}".format(simDay))

    return scat

numFrames = int((recEnd - recStart) / recIntervalLocal) + 1 # +1 because range(frames) below, assumes recSitesFreq=1
anim = animation.FuncAnimation(fig=fig, func=updateDriveFreq, frames=numFrames, interval=500)
anim.save("set1_anim.gif")
plt.show()
