#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>

void fill_cyclic_border(int height, int width, int** a);
void print_out(int height, int width, int **a, std::ostream &out);
void printf_alive_cells(int height, int width, int **a, std::ostream &out);



int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int dy[8] = {1, 1, 1, 0, 0, -1, -1, -1}; 

int main() {

	int numberOfSteps;
	int numberOfThreads;

	std::cout << "Enter the number of steps\n";

	std::cin >> numberOfSteps;

	std::cout << "Enter number of threads\n";

	std::cin >> numberOfThreads;

	std::ifstream in("field.in");

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

	fill_cyclic_border(height, width, lifeField);

	double start = omp_get_wtime();

	std::ofstream out("life.dat");

	omp_set_num_threads(numberOfThreads);

	for (int k = 0; k < numberOfSteps; ++k) {
		// out << "Number of iteration: " << k << "\n";	
		// print_out(width, height, lifeField, out);

		#pragma omp parallel for collapse(2) schedule(dynamic)
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				int neighbours = 0;
				for (int m = 0; m < 8; ++m) {
					neighbours += lifeField[i + 1 + dy[m]][j + 1 + dx[m]];
				}
				if (neighbours == 3 || neighbours == 2 && lifeField[i + 1][j + 1]) {
s					buffer[i][j] = 1;
				} else {
					buffer[i][j] = 0;
				}
			}
		} 
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				lifeField[i + 1][j + 1] = buffer[i][j];
			}
		}
		fill_cyclic_border(height, width, lifeField);
		
		

	}

	double finish = omp_get_wtime();
	printf("%s%f\n", "Calculating time: ", finish - start);
	


	printf_alive_cells(width, height, lifeField, out);
	out << "\n";


	out.close();

	for (int i = 0; i < height + 2; ++i) {
		delete []lifeField[i];
	}
	delete []lifeField;

	for (int i = 0; i < height; ++i) {
		delete []buffer[i];
	}
	delete []buffer;

	return 0;
}

void fill_cyclic_border(int height, int width, int** a) {
	for (int i = 1; i < height + 1; ++i) {
		a[i][0] = a[i][width];
		a[i][width + 1] = a[i][1];
	}

	for (int i = 1; i < width + 1; ++i) {
		a[0][i] = a[height][i];
		a[height + 1][i] = a[1][i];
	}

	a[0][0] = a[height][width];
	a[height + 1][width + 1] = a[1][1];
	a[0][width + 1] = a[height][1];
	a[height + 1][0] = a[1][width];
}

void print_out(int height, int width, int **a, std::ostream &out) {

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (a[i][j]) {
				out << "+ ";
			} else {
				out << ". ";
			}
		}
		out << "\n";
	}
	out << "\n\n\n";

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
