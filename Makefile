CXX      = mpic++
CXXFLAGS = -O2 -Wall -std=c++23
OBJS     = main_mpi.o matrix_io.o

all: main_mpi

main_mpi: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o main_mpi C_mpi.txt

.PHONY: all clean