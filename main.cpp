#include <iostream>
#include <fstream>
#include <exception>
#include "SimplexMethod.h"

std::string GetInputFileName() {
    static std::string fileName = R"(D:\GoogleDrive\sync\source\clion\MatrixGameProblem\input.txt)";
    return fileName;
}

SimplexModel ConvertToSimplexModel(const dmatrix &matrix) {
    SimplexModel model;
    model.isMax = true;
    model.a = matrix;
    model.c.resize(matrix[0].size() + 1, 1);
    model.c[0] = 0;
    model.b.resize(matrix.size(), 1);
    return model;
}

dmatrix Input() {
    std::ifstream fin(GetInputFileName());
    int m = 0, n = 0;
    fin >> n >> m;
    dmatrix matrix(m, dvector(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            fin >> matrix[i][j];
    if (fin.fail())
        throw std::runtime_error("Bad Input data");
    return matrix;
}

int main() {
    auto matrix = Input();
}
