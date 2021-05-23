#ifndef MATRIXGAMEPROBLEM_ITERATIONMETHOD_H
#define MATRIXGAMEPROBLEM_ITERATIONMETHOD_H

#include <algorithm>
#include "matrix.h"
#include "MatrixGameResult.h"

class IterationMethod {
public:
    IterationMethod(dmatrix matrix) : matrix(std::move(matrix)) {}

    MatrixGameResult Solve(db eps) {
        int n = matrix[0].size();
        int m = matrix.size();
        dvector a(m, 0), b(n, 0);
        dvector aCnt(m, 0), bCnt(n, 0);
        int i = 0, j = 0;
        db rBottom = 0, rTop = 2 * eps, rMiddle = 0;
        aCnt[i]++;
        iterationCount = 1;

        while (rMiddle - rBottom > eps || rTop - rMiddle > eps) {
            b = b + matrix[i];
            auto mnIter = std::min_element(b.begin(), b.end());
            rBottom = *mnIter / iterationCount;
            j = int(mnIter - b.begin());
            bCnt[j]++;

            AddWithColumn(a, j);
            auto mxIter = std::max_element(a.begin(), a.end());
            rTop = *mxIter / iterationCount;
            i = int(mxIter - a.begin());
            aCnt[i]++;

            rMiddle = (rTop + rBottom) / 2;
            iterationCount++;
        }
        return MatrixGameResult{rMiddle, aCnt * (1. / iterationCount), bCnt * (1. / iterationCount)};
    }

    int GetIterationCount() const {
        return iterationCount;
    }

private:
    void AddWithColumn(dvector &v, int j) {
        for (int i = 0; i < matrix.size(); i++)
            v[i] += matrix[i][j];
    }

    dmatrix matrix;
    int iterationCount = 1;
};


#endif //MATRIXGAMEPROBLEM_ITERATIONMETHOD_H
