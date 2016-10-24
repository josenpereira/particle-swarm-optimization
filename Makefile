all: pso_test
pso_test: pso_test.cpp Pso.o
my_pso.o: Pso.cpp
clean:
	rm -f *.o pso_test
