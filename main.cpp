#include <iostream>
#include "SimplexMethod.h"
#include "MatrixGameResult.h"
#include "IterationMethod.h"

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

dvector GetDualSolution(const SimplexResult &res, int n) {
    int m = res.x.size() - n;
    auto y = dvector(n + m);
    for (int i = 1; i <= n; i++)
        y[m + i - 1] = res.c[i];
    for (int i = n + 1; i <= n + m; i++)
        y[i - n - 1] = res.c[i];
    return y;
}

dvector GetProbabilities(dvector x, db v, int n) {
    x = x * v;
    return dvector(x.begin(), x.begin() + n);
}


int main() {
    auto matrix = Input();
    auto delta = GetMin(matrix);
    if(delta < 0)
        matrix = matrix + -delta;
    auto model = ConvertToSimplexModel(matrix);         // For B player
    auto simplexMethod = SimplexMethod(model);
    auto result = simplexMethod.Solve();
    auto ax = GetDualSolution(result, model.c.size() - 1);  // A player's variables
    auto v = 1 / result.c[0];                           // Game cost
    auto p = GetProbabilities(ax, v, model.b.size());
    auto q = GetProbabilities(result.x, v, model.c.size() - 1);
    auto resWithSimplex = MatrixGameResult{v, p, q};

    auto iterationMethod = IterationMethod(matrix);
    auto eps  = 1e-3;
    auto resWithIteration = iterationMethod.Solve(eps);

    if(delta < 0){
        resWithSimplex.AddDelta(delta);
        resWithIteration.AddDelta(delta);
    }

    std::cout << "Simplex Method:\n" << resWithSimplex << std::endl <<
              "Iteration Method:\nIteration Count: " << iterationMethod.GetIterationCount() << std::endl <<
              "Eps = " << eps << std::endl <<
              resWithIteration << std::endl;


}
