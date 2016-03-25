#include <fstream>
#include <string>

void printf_alive_cells(int height, int width, int **a, std::ostream &out);

int main(int argc, char **argv) {

	char *inputFile;
	char *outputFile;

	if (argc > 1) {
		inputFile = std::string(argv[1]);
	} else {
		inputFile = "field.in";
		outputFile = "field.out";
	}
	if (argc > 2) {
		outputFile = std::string(argv[2]);
	} else {
		outputFile = "field.out";
	}

	std::ifstream in(inputFile);

	int height, width;
	in >> height >> width;

	int **lifeField = new int*[height + 2];
	for (int i = 0; i < height + 2; ++i) {
		lifeField[i] = new int[width + 2];
	}

	int **buffer = new int*[height];
	for (int i = 0; i < height; ++i) {
		buffer[i] = new int[width];
	}

	for (int i = 1; i < height + 1; ++i) {
		for (int j = 1; j < width + 1; ++j) {
			in >> lifeField[i][j]; 
		}
	}
	in.close();


	std::ofstream out(outputFile);

	printf_alive_cells(height, width, lifeField, out);

	out.close();

	for (int i = 0; i < height + 2; ++i) {
		delete []lifeField[i];
	}
	delete []lifeField;

	return 0;
}


void printf_alive_cells(int height, int width, int **a, std::ostream &out) {
	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (a[i][j]) {
				out << i + 1 << ' ' << j + 1 << '\n';
			}
		}
	}

}