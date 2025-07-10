# -*- coding: utf-8 -*-
"""
Created on Tue Jul  1 14:38:21 2025

@author: biol0117
"""
import os
import subprocess
import numpy as np
import pytest

@pytest.fixture
def custom_args(request):
    set_label = request.config.getoption("--set_label")
    boundary_type = request.config.getoption("--boundary_type")
    dispersal_type = request.config.getoption("--dispersal_type")
    return set_label, boundary_type, dispersal_type

def test_gdsimsapp(custom_args):
    set_label, boundary_type, dispersal_type = custom_args

    # Create input
    params_filename = "../test/oracle/params_sets/params_set" + set_label + ".txt"
    input_string = "100" + "\n" + params_filename + "\n" + "y" +"\n" + "y" + "\n"
    
    if set_label == "14" or set_label == "15":
        input_string += "3" + "\n" + "../includes/rainfall.txt" + "\n" # paths relative to build directory
    if set_label == "17":
        input_string += "5" + "\n" + "../includes/rel_times.txt" + "\n"
    
    boundary = "t"
    if boundary_type == "toroid":
        boundary = "t"
    elif boundary_type == "edge":
        boundary = "e"
    dispersal = "d"
    if dispersal_type == "distance_kernel":
        dispersal = "d"
    elif dispersal_type == "radial":
        dispersal = "r"

    input_string += "1" + "\n" + boundary + "\n" + "2" + "\n" + dispersal + "\n" 
    input_string += "0" + "\n"
        
    # Run C++ model with input data
    proc = subprocess.Popen([f"./gdsimsapp"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    outs, errs = proc.communicate(input=input_string)     
    
    # Check errors
    params = np.loadtxt(params_filename, ndmin=1)
    num_runs = int(params[0])
    assert None == errs, "Unexpected errors"
    for i in range(1, num_runs + 1):
        run_label = str(i)
        assert os.path.isfile(f"./output_files/Totals{set_label}run{run_label}.txt"), f"Could not find Totals{set_label}run{run_label}.txt"
        assert os.path.isfile(f"./output_files/CoordinateList{set_label}run{run_label}.txt"), f"Could not find CoordinateList{set_label}run{run_label}.txt"
        assert os.path.isfile(f"./output_files/LocalData{set_label}run{run_label}.txt"), f"Could not find LocalData{set_label}run{run_label}.txt"

    # Compare to oracle
    for i in range(1, num_runs + 1):
        run_label = str(i)
        actual = np.loadtxt(f"./output_files/Totals{set_label}run{run_label}.txt", skiprows=2, ndmin=2)
        expected = np.loadtxt(f"../test/oracle/{boundary_type}_{dispersal_type}/set{set_label}/Totals{set_label}run{run_label}.txt", skiprows=2, ndmin=2)
        np.testing.assert_equal(expected, actual, f"Totals{set_label}run{run_label}.txt does not match file on test/oracle/{boundary_type}_{dispersal_type}/set{set_label}")

        actual = np.loadtxt(f"./output_files/CoordinateList{set_label}run{run_label}.txt", skiprows=2, ndmin=2)
        expected = np.loadtxt(f"../test/oracle/{boundary_type}_{dispersal_type}/set{set_label}/CoordinateList{set_label}run{run_label}.txt", skiprows=2, ndmin=2)
        np.testing.assert_equal(expected, actual, f"CoordinateList{set_label}run{run_label}.txt does not match file on test/oracle/{boundary_type}_{dispersal_type}/set{set_label}")

        actual = np.loadtxt(f"./output_files/LocalData{set_label}run{run_label}.txt", skiprows=2, ndmin=2)
        expected = np.loadtxt(f"../test/oracle/{boundary_type}_{dispersal_type}/set{set_label}/LocalData{set_label}run{run_label}.txt", skiprows=2, ndmin=2)
        np.testing.assert_equal(expected, actual, f"LocalData{set_label}run{run_label}.txt does not match file on test/oracle/{boundary_type}_{dispersal_type}/set{set_label}")
