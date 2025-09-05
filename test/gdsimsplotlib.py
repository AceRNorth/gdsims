# -*- coding: utf-8 -*-
"""
Created on Tue Sep  2 13:56:14 2025
Colourblind-friendly plotting library companion for GDSiMS.
Includes plot and animation functions for the simulation output files.

@author: Sara Perez Vizan
"""

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.colors as mcolors

def plot_totals_genotype(totals_file, sum_lines=False, title="Total females across the area by genotype"):
    """
    Plots the total adult mated females across the simulation area, classed by genotype.

    Parameters
    ----------
    totals_file : str
        Filepath for the totals file.
    sum_lines : bool, optional
        Whether summation lines for all genotypes and mosquito genotypes capable of malaria transmission
        will be plotted. The default is False.
    title : str, optional
        Plot title. The default is "Total females across the area by genotype".

    Returns
    -------
    fig : matplotlib.figure.Figure
        Plot figure.

    """
    totals = np.loadtxt(totals_file, skiprows=2, ndmin=2)

    times = totals[:, 0]
    total_females = totals[:, 1:]

    fig = plt.figure()
    plt.title(title)
    plt.xlabel("Day")
    plt.ylabel("Total number of individuals")
    plt.plot(times, total_females[:, 0], label="$F_{WW}$", linestyle="solid", color="hotpink")
    plt.plot(times, total_females[:, 1], label="$F_{WD}$", linestyle="dashed", color="mediumturquoise")
    plt.plot(times, total_females[:, 2], label="$F_{DD}$", linestyle="solid", color="royalblue")
    plt.plot(times, total_females[:, 3], label="$F_{WR}$", linestyle="dotted", color="slategray")
    plt.plot(times, total_females[:, 4], label="$F_{RR}$", linestyle="solid", color="rebeccapurple")
    plt.plot(times, total_females[:, 5], label="$F_{DR}$", linestyle="dashed", color="darkviolet")
    
    if sum_lines == True:
        # all genotypes
        plt.plot(times, np.sum(total_females, axis=1).tolist(),
                  label="$F_{WW}$+$F_{WD}$+\n$F_{DD}$+$F_{WR}$+\n$F_{RR}$+$F_{DR}$",
                  linestyle="solid", color="black")
        # capable of malaria transmission
        plt.plot(times, np.sum(total_females[:, (0, 1, 3)], axis=1).tolist(),
                  label="$F_{WW}$+$F_{WD}$+\n$F_{WR}$",
                  linestyle="dashed", color="crimson")
    plt.legend()
    
    return fig
    
def plot_totals_allele_freq(totals_file, title="Total females across the area by allele frequency"):
    """
    Plots the total females across the simulation area, classed by genotype.

    Parameters
    ----------
    totals_file : str
        Filepath for the totals file.
    title : str, optional
        Plot title. The default is "Total females across the area by allele frequency".

    Returns
    -------
    fig : matplotlib.figure.Figure
        Plot figure.

    """
    totals = np.loadtxt(totals_file, skiprows=2, ndmin=2)

    times = totals[:, 0]
    total_females = totals[0:, 1:]
    WW = total_females[:, 0]
    WD = total_females[:, 1]
    DD = total_females[:, 2]
    WR = total_females[:, 3]
    RR = total_females[:, 4]
    DR = total_females[:, 5]

    fig = plt.figure()
    plt.title(title)
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
    
    return fig
    
def plot_coords(coords_file, title="Patch locations"):
    """
    Plots the patch coordinate points from the output coordinates file.

    Parameters
    ----------
    coords_file : str
        Filepath for the output coordinates file.
    title : str, optional
        Plot title. The default is "Patch locations".

    Returns
    -------
    fig : matplotlib.figure.Figure
        Plot figure.

    """
    coords = np.loadtxt(coords_file, skiprows=2, ndmin=2)

    x = coords[:, 1]
    y = coords[:, 2]

    fig = plt.figure()
    axes = fig.add_subplot(111)
    plt.title(title)
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
    
    return fig

def plot_local_pop_size(local_file, coords_file, t=0, title="Population sizes"):
    """
    Plots a snapshot of total population size per patch at the timestep t.
    Note timestep, not simulation day, as the interval depends on the recording interval
    rec_interval_local used in the simulation.

    Parameters
    ----------
    local_file : str
        Filepath for the local file.
    coords_file : str
        Filepath for the output coordinates file.
    t : int, optional
        Recorded timestep, starting from 0. The default is 0.
    title : str, optional
        Plot title. The default is "Population sizes".

    Returns
    -------
    fig : matplotlib.figure.Figure
        Plot figure.
    scat : matplotlib.collection.PathCollection
        Scatter points from the plot.
    annotation : matplotlib.text
        Simulation day annotation from the plot.

    """
    fig, axes = plt.subplots()

    ind, x, y = np.loadtxt(coords_file, skiprows=2, ndmin=2, unpack=True)
    num_rec_pats = len(x)
    local_data = np.loadtxt(local_file, skiprows=2, ndmin=2)  # get populations

    sim_day = int(local_data[t*num_rec_pats, 0])
    local_data_day = local_data[t*num_rec_pats:((t+1)*num_rec_pats), 2:8]

    # calculate total population for all genotypes in each patch
    tot_pops = np.zeros(len(x))
    for pat in range(0, len(x)): 
        patch_data = local_data_day[pat, :]
        for i in range(0, len(patch_data)):
            tot_pops[pat] += patch_data[i]

    # find maximum and minimum population values in whole simulation for colour map bounds
    max_pop = np.amax(local_data)
    min_pop = np.amin(local_data)

    scat = axes.scatter(x, y, c=tot_pops, cmap="cividis_r", vmin=min_pop, vmax=max_pop, marker='.')
    cbar = fig.colorbar(scat, ax=axes, label='Total population size')
    annotation = fig.text(x=0.1, y=0.9, s="t = {}".format(sim_day))
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
    axes.set_title(title)
    
    return (fig, scat, annotation)

def animate_local_pop_size(local_file, coords_file, interval=500, title="Population sizes"):
    """
    Creates an animation of total population size per patch.
    Timesteps will depend on the rec_interval_local used for the simulation
    and thus the days recorded on the local output file.
    Important: the return object must be assigned to a variable for the animation to be visible, e.g. 'anim'.
    The variable must exist until you output the Animation.
    You can then save the animation by using anim.save("anim_filepath.gif").
    Visit matplotlib's FuncAnimation docs for more details.

    Parameters
    ----------
    local_file : str
        Filepath for the local file.
    coords_file : str
        Filepath for the output coordinates file.
    interval : int, optional
        Frame interval for the animation in milliseconds. The default is 500.
    title : str, optional
        Plot title. The default is "Population sizes".

    Returns
    -------
    anim : matplotlib.animation.FuncAnimation
        Animation object.

    """
    fig, scat, annotation = plot_local_pop_size(local_file, coords_file)

    ind, x, y = np.loadtxt(coords_file, skiprows=2, ndmin=2, unpack=True)
    num_rec_pats = len(x)
    local_data = np.loadtxt(local_file, skiprows=2, ndmin=2)
    def update(step):
        sim_day = int(local_data[(step*num_rec_pats), 0])
        local_data_day = local_data[(step*len(x)):((step+1)*len(x)), 2:8]
        
        # calculate total population for all genotypes in each patch
        tot_pops = np.zeros(len(x))
        for pat in range(0, len(x)): 
            patch_data = local_data_day[pat, :]
            for i in range(0, len(patch_data)):
                tot_pops[pat] += patch_data[i]

        scat.set_array(tot_pops) # update the scatter point colours according to new tot_pops
        annotation.set_text("t = {}".format(sim_day))
        return scat

    rec_start = local_data[0, 0]
    rec_end = local_data[-1, 0]
    if len(local_data) > num_rec_pats:
        rec_interval_local = int(local_data[num_rec_pats, 0]) - int(local_data[0, 0])
    else:
        rec_interval_local = 0

    num_frames = int((rec_end - rec_start) / rec_interval_local) + 1 # +1 because range(frames) below, assumes recSitesFreq=1
    anim = animation.FuncAnimation(fig=fig, func=update, frames=num_frames, interval=interval)
    plt.show()
    return anim

def plot_local_drive_allele_freq(local_file, coords_file, t=0, title="Drive allele frequencies"):
    """
    Plots a snapshot of drive allele frequency per patch at the timestep t.
    Note timestep, not simulation day, as the interval depends on the recording interval
    rec_interval_local used in the simulation.

    Parameters
    ----------
    local_file : str
        Filepath for the local file.
    coords_file : str
        Filepath for the output coordinates file.
    t : int, optional
        Recorded timestep, starting from 0. The default is 0.
    title : str, optional
        Plot title. The default is "Drive allele frequencies".

    Returns
    -------
    fig : matplotlib.figure.Figure
        Plot figure.
    scat : matplotlib.collection.PathCollection
        Scatter points from the plot.
    annotation : matplotlib.text
        Simulation day annotation from the plot.

    """
    fig, axes = plt.subplots()

    ind, x, y = np.loadtxt(coords_file, skiprows=2, ndmin=2, unpack=True)
    num_rec_pats = len(x)
    local_data = np.loadtxt(local_file, skiprows=2, ndmin=2)  # get populations

    sim_day = int(local_data[t*num_rec_pats, 0])
    local_data_day = local_data[t*num_rec_pats:((t+1)*num_rec_pats), 2:8]

    WW = local_data_day[:, 0]
    WD = local_data_day[:, 1]
    DD = local_data_day[:, 2]
    WR = local_data_day[:, 3]
    RR = local_data_day[:, 4]
    DR = local_data_day[:, 5]

    # calculate drive allele frequency for each patch
    drive_freq = np.zeros(num_rec_pats)
    for pat in range(0, num_rec_pats):
        tot = WW[pat] + WD[pat] + DD[pat] + WR[pat] + RR[pat] + DR[pat]
        if tot == 0:
            drive_freq[pat] = -2  # assign different distinguishable value for no-population patches
        elif tot == WW[pat]:
            drive_freq[pat] = -0.5
        else:
            drive_freq[pat] = (WD[pat] + (2*DD[pat]) + DR[pat]) / (2*tot)

    # define discrete colourmap
    viridis_r = mpl.colormaps['viridis_r'].resampled(14)
    # don't want edge colours in viridis so there's enough contrast with two
    # additional cmap colours
    viridis_colours = viridis_r(np.linspace(0.2, 0.8, 10))
    # add colours for no-population patch and wild-population patch
    all_colours = np.vstack((mcolors.to_rgba("lightgray"), mcolors.to_rgba("hotpink"), viridis_colours))
    cmap = mcolors.ListedColormap(all_colours)
    bounds = [-2, -1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
    cnorm = mcolors.BoundaryNorm(bounds, cmap.N)

    # make a scatter plot with drive frequency colour map
    scat = axes.scatter(x, y, c=drive_freq, cmap=cmap, norm=cnorm, marker='.')
    colorbar = fig.colorbar(scat, ax=axes)
    colorbar.set_label('Drive allele frequency', labelpad=-10)  # reduce distance to colorbar label
    colorbar.ax.set_yticks([-2, -1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0],
                                labels=['no pop', 'wild', '0.0', '0.1', '0.2', '0.3', '0.4',
                                        '0.5', '0.6', '0.7', '0.8', '0.9', '1.0'])
    labels = colorbar.ax.get_yticklabels()
    labels[0].set_verticalalignment('bottom')  # align first label text above the tick
    labels[1].set_verticalalignment('bottom')
    annotation = fig.text(x=0.1, y=0.9, s='t = {}'.format(sim_day))
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
    axes.set_title(title)

    return (fig, scat, annotation)

def animate_local_drive_allele_freq(local_file, coords_file, interval=500, title="Drive allele frequencies"):
    """
    Creates an animation of drive allele frequency per patch.
    Timesteps will depend on the rec_interval_local used for the simulation
    and thus the days recorded on the local output file.
    Important: the return object must be assigned to a variable for the animation to be visible, e.g. 'anim'.
    The variable must exist until you output the Animation.
    You can then save the animation by using anim.save("anim_filepath.gif").
    Visit matplotlib's FuncAnimation docs for more details.

    Parameters
    ----------
    local_file : str
        Filepath for the local file.
    coords_file : str
        Filepath for the output coordinates file.
    interval : int, optional
        Frame interval for the animation in milliseconds. The default is 500.
    title : str, optional
        Plot title. The default is "Drive allele frequencies".

    Returns
    -------
    anim : matplotlib.animation.FuncAnimation
        Animation object.

    """
    fig, scat, annotation = plot_local_drive_allele_freq(local_file, coords_file)
    
    ind, x, y = np.loadtxt(coords_file, skiprows=2, ndmin=2, unpack=True)
    num_rec_pats = len(x)
    local_data = np.loadtxt(local_file, skiprows=2, ndmin=2)
    def update(step):
        sim_day = int(local_data[(step*num_rec_pats), 0])
        local_data_day = local_data[(step*num_rec_pats):((step + 1)*num_rec_pats), 2:8]
        
        WW = local_data_day[:, 0]
        WD = local_data_day[:, 1]
        DD = local_data_day[:, 2]
        WR = local_data_day[:, 3]
        RR = local_data_day[:, 4]
        DR = local_data_day[:, 5]

        drive_freq = np.zeros(num_rec_pats)
        for pat in range(0, num_rec_pats):
            tot = WW[pat] + WD[pat] + DD[pat] + WR[pat] + RR[pat] + DR[pat]
            if tot == 0:
                drive_freq[pat] = -2
            elif tot == WW[pat]:
                drive_freq[pat] = -0.5
            else:
                drive_freq[pat] = (WD[pat] + (2*DD[pat]) + DR[pat]) / (2*tot)

        scat.set_array(drive_freq) # update the scatter point colours according to new drive_freq
        annotation.set_text("t = {}".format(sim_day))

        return scat

    rec_start = local_data[0, 0]
    rec_end = local_data[-1, 0]
    if len(local_data) > num_rec_pats:
        rec_interval_local = int(local_data[num_rec_pats, 0]) - int(local_data[0, 0])
    else:
        rec_interval_local = 0

    num_frames = int((rec_end - rec_start) / rec_interval_local) + 1 # +1 because range(frames) below, assumes recSitesFreq=1
    anim = animation.FuncAnimation(fig=fig, func=update, frames=num_frames, interval=500)
    plt.show()
    
    return anim