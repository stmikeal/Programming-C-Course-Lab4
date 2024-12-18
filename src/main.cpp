#include "sparse.hpp"
#include <vector>
#include <ctime> 
#include <random>

std::vector<std::vector<double>> &add(std::vector<std::vector<double>> &v1, std::vector<std::vector<double>> &v2) {
	for(size_t i = 0; i < 500; ++i) {
		for (size_t j = 0; j < 1000; ++j) {
			v2[i][j] += v1[i][j];
		}
	}
	return v2;
}

int main()
{
	clock_t start, end;
	std::random_device dev;
	std::uniform_int_distribution<size_t> cord_rand(0, 499);
	std::uniform_real_distribution<double> val_rand(-200.0, 200.0);

	std::vector<std::vector<double>> matrix1(500, std::vector<double>(1000));
	SparseVector<double> v1(500, 1000);
	std::vector<std::vector<double>> matrix2(500, std::vector<double>(1000));
	SparseVector<double> v2(500, 1000);

	for(size_t i = 0; i < 40; ++i) {
		auto x1 = cord_rand(dev);
		auto y1 = cord_rand(dev);
		auto x2 = cord_rand(dev);
		auto y2 = cord_rand(dev);
		auto val1 = val_rand(dev);
		auto val2 = val_rand(dev);
		matrix1[x1][y1] = val1;
		v1.put(val1, x1, y1);
		matrix2[x2][y2] = val2;
		v2.put(val2, x2, y2);
	}

	start = clock();
	add(matrix1, matrix2);
	end = clock();
	std::cout << "Add time on vector: " << std::to_string((double)(end - start) / CLOCKS_PER_SEC) << std::endl;
	start = clock();
	v1 + v2;
	end = clock();
	std::cout << "Add time on custom structure: " << std::to_string((double)(end - start) / CLOCKS_PER_SEC) << std::endl;
	// SparseVector<double> v1(2, 3);
	// v1.put(1.0, 1, 0);
	// v1.put(2.0, 1, 2);
	// SparseVector<double> v2 = v + v1;
	// v2.T();
	// std::cout << "Multiplication" << std::endl;
	// SparseVector<double> v3 = v * v2;
	// v3^3;
	// v3.map([](auto i) { return i*2; });
}