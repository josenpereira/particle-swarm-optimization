/**
    Particle Swarm Optimization Algorithm implementation
    Pso.cpp
    Purpose: Implements all members of the Pso class. Function pso_run implements
    the PSO algorithm. The goal of this algorithm is to find a solution to an
    optimization problem, finding a set of coeficients that maximize a fitness
    function provided by the user. To do so it considers a set of particles
    that move in the search space. Each particle is a solution to the optimization
    problem. By probabilistically attracting particles to their best position
    in the search space and the best position of a set of other particles, a
    good solution is found.

    @author J. N. Pereira
    @version 1.1 18/10/16 
*/

#include "Pso.h"

/**
    Draws a random number in a given interval in a uniform way.

    @param min interval start
    @param max interval end
    @return random number between min and max
*/
double uniform(double min, double max){
	return (max-min)*(double)rand()/(double)RAND_MAX + min;
}

/**
    Pso class constructor

    @param logfile name of the logfile
*/
Pso::Pso(std::string logfile){

	logfile_name = logfile;
}

/**
    Runs the PSO algorithm. It starts by initializing the set of particles.
    During the number iterations specified it updates the particles
    positions and velocities and evaluates their new fitness. It updates
    their personal and neighborhood bests. The position with the best
    fitness is stored and returned.

    @return best position found during the iterations specified
*/
double *Pso::pso_run(){
	int i;

	fit_log = fopen(logfile_name.c_str(),"w");
	particles_init();

	for(i=0;i<PSO_ITERATIONS;i++){
		update_particle_positions();
		evaluate_particles();
		#ifdef NOISY
			evaluate_best();			// if fitness is noisy reevaluate particles personal best
		#endif
		update_p_best();
		//find_neighborhood();			// if neighborhood is not static it must be reevaluated each iteration
		update_n_best();
		fprintf(fit_log,"%d %.4f\n",i,g_best_fit);
		printf("Iteration %d fitness: %.4f\n",i,g_best_fit);
	}
	fclose(fit_log);
	return g_best;
}

/**
    Initializes the set of particles with positions and velocities,
    both drawn from intervals specified in the algorithms parameters.
    A first evaluation is also made in order to find personal and 
    neighboord bests for each particle.
*/
void Pso::particles_init(void){
	int i,j;
	for(i=0;i<SWARMSIZE;i++){
		for(j=0;j<DATASIZE;j++){
			x[i][j]=uniform(MININIT,MAXINIT);	// Position initially distributed randomly
			v[i][j]=uniform(-VMAX,VMAX);		// Velocity initially distributed randomly
		}
		x_fit[i]=-1.0;
		p_best_fit[i]=-1.0;
		n_best_fit[i]=-1.0;
	}
	g_best_fit = -1.0;							// Global best fitness is initially zero
	evaluate_particles();
	update_p_best();
	find_neighborhood();						// Find particle's neighborhood
	update_n_best();							// Update neighborhood's best
}

/**
    Copies a particle from an origin to a destination.

    @param org original particle
    @param dest destination particle
*/
void Pso::copy_particle(double *org, double *dest){
	int j;
	for(j=0;j<DATASIZE;j++){
		dest[j]=org[j];
	}
}

/**
    Find a set of neighbor particles to each particle. In this
    case it implements a neighborhood with a ring topology.
*/
void Pso::find_neighborhood(void){
	int i,j;
	for(i=0;i<SWARMSIZE;i++){
		for(j=0;j<NEIGH_SIZE;j++){
			neighbor[i][j]=(i-NEIGH_SIZE/2+j)%SWARMSIZE;
			if(neighbor[i][j]<0) neighbor[i][j]+=SWARMSIZE;	// negative number wrap around
		}
	}
}

/**
    Updates each particle's personal best position found.
*/
void Pso::update_p_best(void){
	int i;
	for(i=0;i<SWARMSIZE;i++){
		if(x_fit[i]>p_best_fit[i]){
			p_best_fit[i]=x_fit[i];
			copy_particle(x[i],p_best[i]);
			p_best_times[i]=1;				// Personal best has been evaluated once
		}
	}
}

/**
    Updates each particle's neighborhood best position found.
*/
void Pso::update_n_best(void){
	int i,j,n;
	for(i=0;i<SWARMSIZE;i++){
		for(j=0;j<NEIGH_SIZE;j++){						// for each particle's neighbor
			n=neighbor[i][j];							// neighbor particle index
			if(p_best_fit[n]>n_best_fit[i]){			// if neighbor's fitness is better than personal one
				n_best_fit[i]=p_best_fit[n];
				copy_particle(p_best[n],n_best[i]);
			}
		}
		if(n_best_fit[i]>g_best_fit){
			g_best_fit=n_best_fit[i];					// update global best
			copy_particle(n_best[i],g_best);	
		}
	}
}

/**
    Updates each particle's position and velocity in a probabilistic
    way. Particles have a certain inertia (INERTIA) that forces them
    in a given path but are also atracted to their personal best and
    their neighborhood best (according to PSO parameters PWEIGHT and
    NWEIGHT).
*/
void Pso::update_particle_positions(void){
	int i,j;
	for(i=0;i<SWARMSIZE;i++){
		for(j=0;j<DATASIZE;j++){
			v[i][j]=INERTIA*v[i][j]+PWEIGHT*uniform(0.0,1.0)*(p_best[i][j]-x[i][j])+NWEIGHT*uniform(0.0,1.0)*(n_best[i][j]-x[i][j]);
			if(v[i][j]>VMAX) v[i][j]=VMAX;
			if(v[i][j]<-VMAX) v[i][j]=-VMAX;
			x[i][j]+=v[i][j];
		}
	}
}

/**
    Evaluates fitness of each particle.
*/
void Pso::evaluate_particles(void){
	int i;
	
	for(i=0;i<SWARMSIZE;i++){
		x_fit[i]=fitness(x[i]);
    //printf("Particle %d fitness: %.4f\n",i,x_fit[i]);
  }	
}

/**
    Re-evaluates each particle personal best. New best is
    obtained by averaging how many times that position was
    evaluated.
*/
void Pso::evaluate_best(void){
	int i;
	double new_fit;
		
	for(i=0;i<SWARMSIZE;i++){
		new_fit=fitness(p_best[i]);
		p_best_fit[i]=(p_best_fit[i]*p_best_times[i]+new_fit)/(p_best_times[i]+1);
		p_best_times[i]++;
	}	
}

/**
    Pso class destructor.
*/
Pso::~Pso(){

}