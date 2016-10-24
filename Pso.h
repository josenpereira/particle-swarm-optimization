/**
    Particle Swarm Optimization Algorithm header file
    Pso.h
    Purpose: Define Pso class and its public and private members.

    @author J. N. Pereira
    @version 1.1 18/10/16 
*/

#ifndef PSO_H
#define PSO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>

//#define NOISY										// Used if fitness function provided by user is noisy

#define DATASIZE 6									// Dimension of search space - particles' number of coeficients
#define SWARMSIZE 100								// Number of particles
#define TOTAL_ITERATIONS 100						// Total number of iterations of the PSO algorithm
#define MININIT -10.0                   			// Lower bound on position initialization value
#define MAXINIT 10.0                    			// Upper bound on position initialization value
#define VMAX 4.0									// Maximum particle velocity
#define NEIGH_SIZE 3								// Neighborhood size
#define INERTIA 0.6									// PSO paramenter
#define PWEIGHT 2.0									// PSO paramenter
#define NWEIGHT 2.0									// PSO paramenter
#ifdef NOISY
	#define PSO_ITERATIONS TOTAL_ITERATIONS/2
#else
	#define PSO_ITERATIONS TOTAL_ITERATIONS
#endif

double fitness(double *x);							// Must be provided by user, evaluates quality of each potential particle

double uniform(double min, double max);				// Uniform random number generator in [min,max] interval

class Pso
{
public:
	Pso(std::string logfile);

	double *pso_run(void);							// Runs pso and returns pointer to best particle's position

	~Pso();

private:
	std::string logfile_name;
	FILE *fit_log;									// Log file
	
	double x[SWARMSIZE][DATASIZE];					// Particle positions
	double v[SWARMSIZE][DATASIZE];					// Particle velocities
	double x_fit[SWARMSIZE];						// Particle's fitness
	double p_best[SWARMSIZE][DATASIZE];				// Personal best particle positions
	double p_best_fit[SWARMSIZE];					// Fitnesses of personal best positions
	int p_best_times[SWARMSIZE];					// Number of times personal best has been evaluated
	int neighbor[SWARMSIZE][NEIGH_SIZE];			// Particle's neighbors
	double n_best[SWARMSIZE][DATASIZE];				// Neighborhood best particle positions
	double n_best_fit[SWARMSIZE];					// Fitnesses of neighborhood best particle positions
	double g_best[DATASIZE];						// Global best
	double g_best_fit;								// Global best fitness
	
	void particles_init(void);						// Particle initialization
	void find_neighborhood(void);					// Find particle's neighbors
	void update_p_best(void);						// Find personal best
	void update_n_best(void);						// Find neighborhood's best
	void copy_particle(double *org, double *dest);	// Copy particle
	void update_particle_positions(void);			// Updates particles' positions and velocities
	void evaluate_particles(void);					// Evaluates fitness of each particle
	void evaluate_best(void);						// Re-evaluates each particle personal best
};

#endif