/**
    Particle Swarm Optimization test
    pso_test.cpp
    Purpose: Implement a simple optimization problem (sphere function)
    and run PSO to find a solution.

    @author J. N. Pereira
    @version 1.1 18/10/16 
*/

#include "Pso.h"

/**
    Calculates the fitness of a candidate solution. In this
    case this fitness corresponds to the value of the sphere
    function.

    @param x candidate solution
    @return fitness of candidate solution
*/
double fitness(double *x){
	double perf = 0.0;
	int i;

	for (i = 0; i < DATASIZE; i++){
		perf += pow(x[i],2);
	}
	if(perf==0) perf=0.00001;
	return 1/perf;
}


// Run PSO algorithm to optimize fitness function.
int main(int argc, char** argv){

	double *result;
	double fit;
	int i;

	Pso *pso = new Pso(std::string("log.txt"));

	srand(time(NULL));
	result=pso->pso_run();
	fit=fitness(result);
	for(i=0;i<DATASIZE;i++){
		printf("%.4f\n",result[i]);
	}
	printf("Fitness: %.4f\n",fit);

	delete pso;
	return 0;
}