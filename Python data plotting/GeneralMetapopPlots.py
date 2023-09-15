import pandas as pd
import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt


# set_label = 0 
# run_label = 0 
# num_pat = 0 
# rec_sites_freq = 0 
# rec_interval_global = 0 
# max_t = 0 
# rec_interval_local = 0 
# rec_start = 0 
# rec_end = 0 
# num_reps = 0 
# num_driver_M = 0 
# num_driver_sites = 0 
# driver_start = 0 
# mu_j = 0 
# mu_a = 0 
# disp_rate = 0 
# gamma = 0 
# beta = 0
# theta = 0 
# xi = 0 
# e = 0 
# max_disp = 0 
# psi = 0 
# mu_aes = 0 
# t_hide1 = 0
# t_hide2 = 0  
# t_wake1 = 0 
# t_wake2 = 0 
# alpha0 = 0 
# mean_dev = 0
# min_dev = 0 
# side = 0 
# central_radius = 0 

#%% Read input parameter set from .csv file
params = pd.read_table("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Parameters.csv", delimiter=",")
labels = params[["Parameter"]]
labels = labels.drop(index=0)
input_data = params[["set 1"]]
input_data = input_data.drop(index=0)
input_data = input_data.rename(dict(zip(input_data.index, labels["Parameter"])), axis=0)
display(input_data)
input_list = input_data["set 1"].tolist()
input_string = " ".join(str(i) for i in input_list)

# Run C++ model with input data
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Output files")
proc = subprocess.Popen(["C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\GeneralMetapop.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
outs, errs = proc.communicate(input=input_string)

#%% Plot global output (totals) from model
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Output files")

totals = np.loadtxt("Totals1run2.txt", skiprows=2)

times = totals[:, 0]
total_males = totals[:, 1:]

plt.plot(times, total_males[:, 0], label="$M_{WW}$")
plt.plot(times, total_males[:, 1], label="$M_{WD}$")
plt.plot(times, total_males[:, 2], label="$M_{DD}$")
plt.plot(times, total_males[:, 3], label="$M_{WR}$")
plt.plot(times, total_males[:, 4], label="$M_{RR}$")
plt.plot(times, total_males[:, 5], label="$M_{DR}$")
plt.legend()
