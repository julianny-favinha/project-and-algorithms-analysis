CPP=g++
FLAGS=-O3
TARGET=bnb-fs
FILES=bnb-fs.cpp optimization.cpp input.cpp

all: clean bnb-fs

bnb-fs: bnb-fs.cpp
	$(CPP) $(FLAGS) -o $(TARGET) $(FILES)

clean:
	$(RM) bnb-fs