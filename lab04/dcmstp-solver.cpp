#include "input.cpp"

#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
	char *file_name = argv[1];
	int time = atoi(argv[2]);
	char *method = argv[3];

	read_file(file_name);

	return 0;
}