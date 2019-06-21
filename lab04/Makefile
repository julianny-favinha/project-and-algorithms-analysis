CPP=g++ -std=c++17
FLAGS=-O3 -Wall -Werror
TARGET=dcmstp-solver
FILES=dcmstp-solver.cpp input.cpp agm.cpp grasp.cpp output.cpp

all: clean dcmstp-solver

dcmstp-solver: dcmstp-solver.cpp
	$(CPP) $(FLAGS) -o $(TARGET) $(FILES)

clean:
	$(RM) dcmstp-solver