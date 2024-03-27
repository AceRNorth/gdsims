import pandas as pd
import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt

#%% Run C++ program for test case parameters from file

# ** Modify parameter table .csv filepath, output files folder path and .exe filepath as needed! **
param_csv_filepath = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Parameters - new layout v3.csv"
output_folder_path = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build"
exe_filepath = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build\\gdsimsapp.exe"

# Read input parameter set from .csv file
params = pd.read_table(param_csv_filepath, delimiter=",")
labels = params[["Parameter"]]
labels = labels.drop(index=0)

# Selecting sets to run
sets = [col for col in params if col.startswith('set ')]
for i in range(0, len(sets)):
    sets[i] = int(sets[i].removeprefix("set "))
    
# ** Select other combinations of sets by listing below and uncommenting**
#sets = [11]

for j in range(0, len(sets)):
    if ("set " + str(sets[j])) in params:
        input_data = params[["set " + str(sets[j])]]
        input_data = input_data.drop(index=0)
        input_data = input_data.rename(dict(zip(input_data.index, labels["Parameter"])), axis=0)
        display(input_data)
        input_list = input_data["set " + str(sets[j])].tolist()
        input_string = " ".join(str(i) for i in input_list)
        
        # Run C++ model with input data
        os.chdir(output_folder_path) # directory for output files
        # .exe file for the program
        proc = subprocess.Popen([exe_filepath], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
        outs, errs = proc.communicate(input=input_string)


#%% Compare test case data for all runs

# ** Modify oracle data folder path, and test data folder path as needed! **
oracle_folder_path = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Testing\\oracle data\\edge wedge"
test_data_folder_path = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\build\\output_files"

# ** Modify the list of set numbers selected as needed **
sets = [i for i in range(1, 12)]
#sets = [11]       

# ** Modify the list of num_runs in each set selected as needed **
num_runs_list = [2 for i in range(0, len(sets))]
# num_runs_list = [2, 3, 1]

make_plot = False

for j in range(0, len(sets)):
    print("Set " + str(sets[j]))
    for i in range(1, num_runs_list[j] + 1):
        #import oracle data
        os.chdir(os.path.join(oracle_folder_path, "set" + str(sets[j])))
        totals_oracle = np.loadtxt("Totals" + str(sets[j]) + "run" + str(i) + ".txt", skiprows=2)
        times_oracle = totals_oracle[:, 0]
        tot_males_oracle = totals_oracle[:, 1:]
        coords_oracle = np.loadtxt("CoordinateList" + str(sets[j]) + "run" + str(i) + ".txt", skiprows=2)
        local_oracle = np.loadtxt("LocalData" + str(sets[j]) + "run" + str(i) + ".txt", skiprows=2)
        
        # plot oracle data
        if make_plot:
            plt.figure()
            plt.title("Totals oracle data for " + "set " + str(sets[j]) + " run " + str(i))
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
        os.chdir(test_data_folder_path)
        totals_test = np.loadtxt("Totals" + str(sets[j]) + "run" + str(i) + ".txt", skiprows=2)
        times_test = totals_test[:, 0]
        tot_males_test = totals_test[:, 1:]
        coords_test = np.loadtxt("CoordinateList" + str(sets[j]) + "run" + str(i) + ".txt", skiprows=2)
        local_test = np.loadtxt("LocalData" + str(sets[j]) + "run" + str(i) + ".txt", skiprows=2)
        
        # plot test data
        if make_plot:
            plt.figure()
            plt.title("Totals test data for " + "set " + str(sets[j]) + " run " + str(i))
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
        print("Test results for " + "set " + str(sets[j]) + " run " + str(i) + ":")
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
