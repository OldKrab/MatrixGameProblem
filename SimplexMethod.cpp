#include <cfloat>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#define FILE_INPUT 1
#define FILE_NAME "D:\\source\\plus_plus\\igri\\in.txt"

using namespace std;

typedef double db;
typedef vector<db> dvector;
typedef vector<dvector> dmatrix;

// Получить разрешающий столбец
int get_permissive_col(const dvector& L)
{
    int col = -1;
    db mn = 0;
    for (int i = 1; i < L.size(); i++)
        if (L[i] < mn) {
            mn = L[i];
            col = i;
        }
    return col;
}

// Получить разрешающую строку
int get_permissive_row(const dmatrix& table, int k)
{
    int row = -1;
    db min_value = DBL_MAX;
    for (int i = 1; i < table.size(); i++)
        if (table[i][k] > 0 && table[i][0] / table[i][k] < min_value) {
            row = i;
            min_value = table[i][0] / table[i][k];
        }
    return row;
}

// Решить симплекс-методом
dmatrix solve_simplex(dmatrix table, db* basis)
{
    auto n = table[0].size();
    auto m = table.size();
    auto k = 0;
    vector<int> main_basis(m - 1), not_main_basis(n - 1);

    for (int i = 0; i < n - 1; i++)
        not_main_basis[i] = i;
    for (int i = n - 1; i < n + m - 2; i++)
        main_basis[i - n + 1] = i;
    while ((k = get_permissive_col(table[0])) != -1) {
        // k-ый столбец разрешающий
        // r-ая строка разрешающая
        int r = get_permissive_row(table, k);
        if (r == -1)
            return table;
        swap(main_basis[r - 1], not_main_basis[k - 1]);

        // Преобразуем разрешающий элемент
        table[r][k] = 1. / table[r][k];
        // Преобразуем разрешающий столбец
        for (int i = 0; i < m; i++)
            if (i != r)
                table[i][k] *= -table[r][k];
        // Преобразуем остальные элементы
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (i != r && j != k)
                    table[i][j] += table[r][j] * table[i][k];
        // Преобразуем разрешающую строку
        for (int j = 0; j < n; j++)
            if (j != k)
                table[r][j] *= table[r][k];
    }
    for (int i = 0; i < m - 1; i++)
        basis[main_basis[i]] = table[i + 1][0];
    return table;
}

dmatrix create_table(const dvector& c, const dvector& b, const dmatrix& a, int is_max)
{
    int m = a.size() + 1, n = c.size();
    dmatrix table(m, dvector(n));
    for (int j = 0; j < n; j++)
        table[0][j] = -c[j] * is_max;
    for (int i = 1; i < m; i++)
        table[i][0] = b[i - 1];
    for (int i = 1; i < m; i++)
        for (int j = 1; j < n; j++)
            table[i][j] = a[i - 1][j - 1];
    return table;
}
