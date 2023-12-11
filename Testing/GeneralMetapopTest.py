import pandas as pd
import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt

#%% Run C++ program for test case parameters from file

num_sets = 10

for j in range(0, num_sets):
    # Read input parameter set from .csv file
    params = pd.read_table("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Parameters - new layout v3.csv", delimiter=",")
    #params = pd.read_table("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Parameters.csv", delimiter=",")
    labels = params[["Parameter"]]
    labels = labels.drop(index=0)
    input_data = params[["set " + str(j+1)]]
    input_data = input_data.drop(index=0)
    input_data = input_data.rename(dict(zip(input_data.index, labels["Parameter"])), axis=0)
    display(input_data)
    input_list = input_data["set " + str(j+1)].tolist()
    input_string = " ".join(str(i) for i in input_list)
    
    # Run C++ model with input data
    os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Output files")
    proc = subprocess.Popen(["C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\GeneralMetapop.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    #proc = subprocess.Popen(["C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Prepared for testing version\\General.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    outs, errs = proc.communicate(input=input_string)


#%% Compare test case data for all runs

#directory = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Test oracle data"
#num_files = len([f for f in os.listdir(directory) if (os.path.isfile(os.path.join(directory, f)))])

num_runs = 2 # later take from pd read table
num_sets = 10 # later take from number of folders in oracle folder

make_plot = True

for j in range(0, num_sets):
    print("Set " + str(j+1))
    for i in range(0, num_runs):
        # import oracle data
        os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Testing\\Test oracle data\\Set" + str(j+1))
        #os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Prepared for testing version\\Set" + str(j+1))
        totals_oracle = np.loadtxt("Totals" + str(j+1) + "run" + str(i+1) + ".txt", skiprows=2)
        times_oracle = totals_oracle[:, 0]
        tot_males_oracle = totals_oracle[:, 1:]
        coords_oracle = np.loadtxt("CoordinateList" + str(j+1) + "run" + str(i+1) + ".txt", skiprows=2)
        local_oracle = np.loadtxt("LocalData" + str(j+1) + "run" + str(i+1) + ".txt", skiprows=2)
        
        # plot oracle data
        if make_plot:
            plt.figure()
            plt.title("Totals oracle data for " + "set " + str(j+1) + " run " + str(i+1))
            plt.xlabel("Day")
            plt.ylabel("Total number of individuals")
            plt.plot(times_oracle, tot_males_oracle[:, 0], label="$M_{WW}$")
            plt.plot(times_oracle, tot_males_oracle[:, 1], label="$M_{WD}$")
            plt.plot(times_oracle, tot_males_oracle[:, 2], label="$M_{DD}$")
            plt.plot(times_oracle, tot_males_oracle[:, 3], label="$M_{WR}$")
            plt.plot(times_oracle, tot_males_oracle[:, 4], label="$M_{RR}$")
            plt.plot(times_oracle, tot_males_oracle[:, 5], label="$M_{DR}$")
            plt.legend()
        
        # import test data
        os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Output files")
        #os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Prepared for testing version2\\Set" + str(j+1))
        totals_test = np.loadtxt("Totals" + str(j+1) + "run" + str(i+1) + ".txt", skiprows=2)
        times_test = totals_test[:, 0]
        tot_males_test = totals_test[:, 1:]
        coords_test = np.loadtxt("CoordinateList" + str(j+1) + "run" + str(i+1) + ".txt", skiprows=2)
        local_test = np.loadtxt("LocalData" + str(j+1) + "run" + str(i+1) + ".txt", skiprows=2)
        
        # plot test data
        if make_plot:
            plt.figure()
            plt.title("Totals test data for " + "set " + str(j+1) + " run " + str(i+1))
            plt.xlabel("Day")
            plt.ylabel("Total number of individuals")
            plt.plot(times_test, tot_males_test[:, 0], label="$M_{WW}$")
            plt.plot(times_test, tot_males_test[:, 1], label="$M_{WD}$")
            plt.plot(times_test, tot_males_test[:, 2], label="$M_{DD}$")
            plt.plot(times_test, tot_males_test[:, 3], label="$M_{WR}$")
            plt.plot(times_test, tot_males_test[:, 4], label="$M_{RR}$")
            plt.plot(times_test, tot_males_test[:, 5], label="$M_{DR}$")
            plt.legend()
        
        # compare
        print("Test results for " + "set " + str(j+1) + " run " + str(i+1) + ":")
        if totals_test.shape == totals_oracle.shape:
            print("Totals data array sizes are equal")
        else:
            print("Totals data array sizes are NOT equal!")
            
        if (totals_test == totals_oracle).all():
            print("Totals data arrays are equal")
        else:
            print("Totals data arrays are NOT equal!")
            
            
        if coords_test.shape == coords_oracle.shape:
            print("Coords data array sizes are equal")
        else:
            print("Coords data array sizes are NOT equal!")
        if (coords_test == coords_oracle).all():
            print("Coords data arrays are equal")
        else:
            print("Coords data arrays are NOT equal!")
            
        
        if local_test.shape == local_oracle.shape:
            print("Local data array sizes are equal")
        else:
            print("Local data array sizes are NOT equal!")
        if (local_test == local_oracle).all():
            print("Local data arrays are equal")
        else:
            print("Local data arrays are NOT equal!")
            
        print("")
    
    print("")