import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.colors as mcolors

#%% Plot global output (totals) from model
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\test\\oracle\\toroid_distance_kernel\\set15")

totals = np.loadtxt("Totals15run1.txt", skiprows=2)

times = totals[:, 0]
total_males = totals[:, 1:]

plt.figure()
plt.title("Total males across the area - set 15 run 1")
plt.xlabel("Day")
plt.ylabel("Total number of individuals")
plt.plot(times, total_males[:, 0], label="$M_{WW}$")
plt.plot(times, total_males[:, 1], label="$M_{WD}$")
plt.plot(times, total_males[:, 2], label="$M_{DD}$")
plt.plot(times, total_males[:, 3], label="$M_{WR}$")
plt.plot(times, total_males[:, 4], label="$M_{RR}$")
plt.plot(times, total_males[:, 5], label="$M_{DR}$")
plt.legend()

#%% Plot coordinates of patches

os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build\\output_files")

coords = np.loadtxt("CoordinateList101run1.txt", skiprows=2)

x = coords[:, 1]
y = coords[:, 2]

plt.figure()
plt.title("Patch locations")
plt.xlabel("x (km)")
plt.ylabel("y (km)")
plt.scatter(x, y, marker='.')

#%% Spatial plot of total population size on one day

fig, ax = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build\\output_files")
coords = np.loadtxt("CoordinateList1run1.txt", skiprows=2)
x = coords[:, 1]
y = coords[:, 2]

# get populations
local_data = np.loadtxt("LocalData1run1.txt", skiprows=2)

# get populations on one day
t=3 # recorded timestep
sim_day = int(local_data[t*len(x), 0])
local_data_day = local_data[t*len(x):((t+1)*len(x)), 2:8]

# calculate total population for all genotypes in each patch
tot_pops = np.zeros(len(x))
for pat in range(0, len(x)): 
    patch_data = local_data_day[pat, :]
    for i in range(0, len(patch_data)):
        tot_pops[pat] += patch_data[i]

# find maximum and minimum population values in whole simulation for colour map bounds
max_pop = np.amax(local_data)
min_pop = np.amin(local_data)

# make a scatter plot with drive frequency colour map
scat = ax.scatter(x, y, c=tot_pops, cmap='copper', vmin=min_pop, vmax=max_pop, marker='o')
cbar = fig.colorbar(scat, ax=ax, label='Total population size')
#cbar.ax.set_yticklabels(['None', '0.0', '0.1', '0.2', '0.3', '0.4', '0.5', '0.6', '0.7', '0.8', '0.9', '1.0'])

ax.set_title("Population sizes - set 1 run 1")
annotation = fig.text(x=0.1, y=0.9, s='t = {}'.format(sim_day))
ax.set_xlabel("x (km)")
ax.set_ylabel("y (km)")        

#%% Spatial animation of total population size

fig, ax = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build\\output_files")
coords = np.loadtxt("CoordinateList100run1.txt", skiprows=2)
x = coords[:, 1]
y = coords[:, 2]

# get populations
local_data = np.loadtxt("LocalData100run1.txt", skiprows=2)

# get populations on one day
t=0 # recorded timestep
sim_day = int(local_data[t*len(x), 0])
local_data_day0 = local_data[t*len(x):((t+1)*len(x)), 2:8]

# calculate total population for all genotypes in each patch
tot_pops = np.zeros(len(x))
for pat in range(0, len(x)): 
    patch_data = local_data_day0[pat, :]
    for i in range(0, len(patch_data)):
        tot_pops[pat] += patch_data[i]

# find maximum and minimum population values in whole simulation for colour map bounds
max_pop = np.amax(local_data)
min_pop = np.amin(local_data)

# make a scatter plot with population size colour map
scat = ax.scatter(x, y, c=tot_pops, cmap='copper', vmin=min_pop, vmax=max_pop, marker='o')
cbar = fig.colorbar(scat, ax=ax, label='Total population size')

#ax.set_title("Population sizes - set 1 run 1")
annotation = fig.text(x=0.1, y=0.9, s='t = {}'.format(sim_day))
ax.set_xlabel("x (km)")
ax.set_ylabel("y (km)")  

def update_pop_size(t):
    sim_day = int(local_data[t*len(x), 0])
    local_data_day = local_data[t*len(x):((t+1)*len(x)), 2:8]
    
    # calculate total population for all genotypes in each patch
    tot_pops = np.zeros(len(x))
    for pat in range(0, len(x)): 
        patch_data = local_data_day[pat, :]
        for i in range(0, len(patch_data)):
            tot_pops[pat] += patch_data[i]
            
    scat.set_array(tot_pops) # update the scatter point colours according to new tot_pops
    annotation.set_text("t = {}".format(sim_day))
    return scat

#num_frames = int(local_data[-1, 0]) # only valid like this if timestep is 1, otherwise set manually
rec_sites_freq = 1
num_frames = int(len(local_data[:, 0]) / (len(x) / rec_sites_freq))

print(num_frames)
anim = animation.FuncAnimation(fig=fig, func=update_pop_size, frames=num_frames, interval=500)
#anim.save("set1_pop_anim.gif")
plt.show()

#%% Spatial plot of gene drive allele frequency on one day

fig, ax = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build\\output_files")
coords = np.loadtxt("CoordinateList1run1.txt", skiprows=2)
x = coords[:, 1]
y = coords[:, 2]

# get populations per genotype
local_data = np.loadtxt("LocalData1run1.txt", skiprows=2)
t=0 # recorded timestep
sim_day = int(local_data[t*len(x), 0])
local_data = local_data[t*len(x):((t+1)*len(x)), 2:8]

WW = local_data[:, 0]
WD = local_data[:, 1]
DD = local_data[:, 2]
WR = local_data[:, 3]
RR = local_data[:, 4]
DR = local_data[:, 5]

# calculate drive allele frequency for each patch
drive_freq = np.zeros(len(x))
for pat in range(0, len(x)):
    tot = WW[pat] + WD[pat] + DD[pat] + WR[pat] + RR[pat] + DR[pat]
    if (tot == 0):
        drive_freq[pat] = -1 # assign different distinguishable value for no-population patches
    else:
        drive_freq[pat] = (WD[pat] + (2*DD[pat]) + DR[pat]) / (2*tot)

# define discrete colourmap
main_cmap = ['lightgreen', 'aquamarine', 'mediumturquoise', 'darkcyan','steelblue', 'royalblue', 'mediumblue', 'slateblue', 'darkviolet', 'indigo']
all_colours = ['darkgray'] + main_cmap # add colour for no-population patch
cmap = mcolors.ListedColormap(all_colours)
bounds = [-1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0] 
cnorm = mcolors.BoundaryNorm(bounds, cmap.N)

# make a scatter plot with drive frequency colour map
scat = ax.scatter(x, y, c=drive_freq, cmap=cmap, norm=cnorm, marker='o')
cbar = fig.colorbar(scat, ax=ax, ticks=[-1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], label='Drive allele frequency')
cbar.ax.set_yticklabels(['None', '0.0', '0.1', '0.2', '0.3', '0.4', '0.5', '0.6', '0.7', '0.8', '0.9', '1.0'])

#ax.set_title("Patch locations - set 1 run 1")
annotation = fig.text(x=0.1, y=0.9, s='t = {}'.format(sim_day))
ax.set_xlabel("x (km)")
ax.set_ylabel("y (km)")
#plt.grid()

#%% Spatial animation of gene drive allele frequency

fig, ax = plt.subplots()

# get coords of sites
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build\\output_files")
coords = np.loadtxt("CoordinateList101run1.txt", skiprows=2)
x = coords[:, 1]
y = coords[:, 2]

# get populations per genotype
local_data = np.loadtxt("LocalData101run1.txt", skiprows=2)
t=0 # recorded timestep
sim_day = int(local_data[t*len(x), 0])
local_data_day0 = local_data[t*len(x):((t+1)*len(x)), 2:8]

WW_day0 = local_data_day0[:, 0]
WD_day0 = local_data_day0[:, 1]
DD_day0 = local_data_day0[:, 2]
WR_day0 = local_data_day0[:, 3]
RR_day0 = local_data_day0[:, 4]
DR_day0 = local_data_day0[:, 5]

# calculate drive allele frequency for each patch
drive_freq = np.zeros(len(x))
for pat in range(0, len(x)):
    tot = WW_day0[pat] + WD_day0[pat] + DD_day0[pat] + WR_day0[pat] + RR_day0[pat] + DR_day0[pat]
    if (tot == 0):
        drive_freq[pat] = -2 # assign different distinguishable value for no-population patches
    else:
        drive_freq[pat] = (WD_day0[pat] + (2*DD_day0[pat]) + DR_day0[pat]) / (2*tot)

# define discrete colourmap
main_cmap = ['aquamarine', 'mediumturquoise', 'darkcyan','steelblue', 'royalblue', 'mediumblue', 'slateblue', 'darkviolet', 'indigo', 'black']
all_colours = ['darkgray', 'lightgreen'] + main_cmap # add colours for no-population patch and wild-population patch
cmap = mcolors.ListedColormap(all_colours)
bounds = [-2, -1, 0.0001, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
cnorm = mcolors.BoundaryNorm(bounds, cmap.N)

# make a scatter plot with drive frequency colour map
scat = ax.scatter(x, y, c=drive_freq, cmap=cmap, norm=cnorm, marker='o')
cbar = fig.colorbar(scat, ax=ax, label='Drive allele frequency')
cbar.ax.set_yticks([-2, -1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], labels=['no pop', 'wild', '0.0', '0.1', '0.2', '0.3', '0.4', '0.5', '0.6', '0.7', '0.8', '0.9', '1.0'])
labels = cbar.ax.get_yticklabels()
labels[0].set_verticalalignment('bottom') # align first label text above the tick 
labels[1].set_verticalalignment('bottom')

#ax.set_title("Patch locations - set 1 run 1")
annotation = fig.text(x=0.1, y=0.9, s='t = {}'.format(sim_day))
ax.set_xlabel("x (km)")
ax.set_ylabel("y (km)")
#plt.grid()

def update_drive_freq(t):
    sim_day = int(local_data[t*len(x), 0])
    local_data_day = local_data[t*len(x):((t+1)*len(x)), 2:8]
    
    WW = local_data_day[:, 0]
    WD = local_data_day[:, 1]
    DD = local_data_day[:, 2]
    WR = local_data_day[:, 3]
    RR = local_data_day[:, 4]
    DR = local_data_day[:, 5]

    drive_freq = np.zeros(len(x))
    for pat in range(0, len(x)):
        tot = WW[pat] + WD[pat] + DD[pat] + WR[pat] + RR[pat] + DR[pat]
        if (tot == 0):
            drive_freq[pat] = -2
        else:
            drive_freq[pat] = (WD[pat] + (2*DD[pat]) + DR[pat]) / (2*tot)

    scat.set_array(drive_freq) # update the scatter point colours according to new drive_freq
    annotation.set_text("t = {}".format(sim_day))

    return scat

rec_sites_freq = 1
num_frames = int(len(local_data[:, 0]) / (len(x) / rec_sites_freq))
anim = animation.FuncAnimation(fig=fig, func=update_drive_freq, frames=num_frames, interval=20)
anim.save("set101_drive_anim_radial.gif")
plt.show()
