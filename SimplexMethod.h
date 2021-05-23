#ifndef MATRIXGAMEPROBLEM_SIMPLEXMETHOD_H
#define MATRIXGAMEPROBLEM_SIMPLEXMETHOD_H

#include <cfloat>
#include <fstream>
#include <iostream>
#include <vector>


typedef double db;
typedef std::vector<db> dvector;
typedef std::vector<dvector> dmatrix;

std::ostream &operator<<(std::ostream &out, const dvector &v) {
    for(auto el:v)
        out << el << ' ';
    return out;
}

struct SimplexModel {
    dvector c, b;
    dmatrix a;
    bool isMax = false;
};

struct SimplexResult {
    dvector x, c;
};

std::ostream &operator<<(std::ostream &out, const SimplexResult &result) {
    out << "F = " << result.c[0];
    for (int i = 1; i < result.c.size(); i++)
        if (result.c[i] != 0)
            out << " + " << result.c[i] << "*x" << i;
    out << "\nX = { ";
    for (auto el:result.x)
        out << el << ' ';
    out << "}";
    return out;
}

class SimpexMethod {
public:
    explicit SimpexMethod(SimplexModel sm) : sm(std::move(sm)) {}

    SimplexResult Solve() {
        ConvertModelToTable();

        std::vector<int> main_basis(m - 1), not_main_basis(n - 1);
        for (auto i = 0; i < n - 1; ++i)
            not_main_basis[i] = i;
        for (auto i = n - 1; i < n + m - 2; ++i)
            main_basis[i - n + 1] = i;

        auto k = GetPermissiveCol();
        auto r = -1;
        if (k != -1) r = GetPermissiveRow(k);

        while (k != -1 && r != -1) {
            // k-th column, r-th row are permissive
            std::swap(main_basis[r - 1], not_main_basis[k - 1]);
            // Modify permissive element
            table[r][k] = 1. / table[r][k];
            // Modify permissive column
            for (int i = 0; i < m; i++)
                if (i != r)
                    table[i][k] *= -table[r][k];
            // Modify other elements
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    if (i != r && j != k)
                        table[i][j] += table[r][j] * table[i][k];
            // Modify permissive row
            for (int j = 0; j < n; j++)
                if (j != k)
                    table[r][j] *= table[r][k];

            k = GetPermissiveCol();
            if (k != -1) r = GetPermissiveRow(k);
        }

        SimplexResult sa{dvector(n + m - 2), dvector(n + m - 1)};
        for (int i = 0; i < m - 1; i++) {
            sa.x[main_basis[i]] = table[i + 1][0];
        }
        sa.c[0] = table[0][0];
        for (int i = 0; i < n - 1; i++) {
            sa.c[not_main_basis[i]+1] = table[0][i+1];
        }
        return sa;
    }


private:
    int GetPermissiveCol() {
        int col = -1;
        db mn = 0;
        for (int i = 1; i < table[0].size(); i++)
            if (table[0][i] < mn) {
                mn = table[0][i];
                col = i;
            }
        return col;
    }

    int GetPermissiveRow(int k) {
        int row = -1;
        db min_value = DBL_MAX;
        for (int i = 1; i < table.size(); i++)
            if (table[i][k] > 0 && table[i][0] / table[i][k] < min_value) {
                row = i;
                min_value = table[i][0] / table[i][k];
            }
        return row;
    }

    dmatrix ConvertModelToTable() {
        m = (int) sm.b.size() + 1, n = (int) sm.c.size();
        table.resize(m, dvector(n));
        for (int j = 0; j < n; j++)
            table[0][j] = -sm.c[j] * sm.isMax;
        for (int i = 1; i < m; i++)
            table[i][0] = sm.b[i - 1];
        for (int i = 1; i < m; i++)
            for (int j = 1; j < n; j++)
                table[i][j] = sm.a[i - 1][j - 1];
        return table;
    }

    SimplexModel sm;
    dmatrix table;
    int n = 0, m = 0;
};

#endif //MATRIXGAMEPROBLEM_SIMPLEXMETHOD_H
