#include "Simulation.h"

int main() {
    InputParams params;
    params.num_runs = 1;
    params.max_t = 1400;
    params.num_pat = 50;
    params.side = 1.0;
    params.mu_j = 0.05;
    params.mu_a = 0.125;
    params.beta = 100;
    params.theta = 9;
    params.comp_power = 0.0666666666666666;
    params.min_dev = 10;
    params.gamma = 0.025;
    params.xi = 0.2;
    params.e = 0.95;
    params.driver_start = 100;
    params.num_driver_M = 1000;
    params.num_driver_sites = 1;
    params.disp_rate = 0.01;
    params.max_disp = 0.2;
    params.psi = 0.0;
    params.mu_aes = 0.0;
    params.t_hide1 = 0;
    params.t_hide2 = 0;
    params.t_wake1 = 0;
    params.t_wake2 = 0;
    params.alpha0_mean = 100000;
    params.alpha0_variance = 0.0;
    params.alpha1 = 0.0;
    params.amp = 0.0;
    params.resp = 0.0;
    params.rec_start = 0;
    params.rec_end = 1400;
    params.rec_interval_global = 1;
    params.rec_interval_local = 100;
    params.rec_sites_freq = 1;
    params.set_label = 100;

    Simulation simulation(params);
    InheritanceParams inher;
    inher.gamma = params.gamma;
    inher.xi = params.xi;
    inher.e = params.e;
    simulation.set_inheritance(inher);
    simulation.set_boundary_type(Edge);
    simulation.set_dispersal_type(Radial);
    simulation.set_rainfall("rainfall.txt");
    simulation.set_coords("coords_grid.txt");
    simulation.set_release_times("rel_times.txt");
    simulation.run_reps();
    return 0;
}




