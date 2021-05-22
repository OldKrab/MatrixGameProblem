#include <iostream>
#include <fstream>
#include "SimplexMethod.h"

std::string GetInputFileName() {
    static std::string fileName = R"(D:\GoogleDrive\sync\source\clion\MatrixGameProblem\input.txt)";
    return fileName;
}

dmatrix Input() {
    std::ifstream fin(GetInputFileName());
    int m, n;
    fin >> n >> m;
    dmatrix matrix(m, dvector(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            fin >> matrix[i][j];
    return matrix;
}

int main() {
    auto matrix = Input();
}
