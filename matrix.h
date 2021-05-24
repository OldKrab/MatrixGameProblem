#pragma once
#include <vector>
#include <iostream>
#include <iomanip>


typedef  double db;
typedef std::vector<db> dvector;
typedef std::vector<dvector> dmatrix;


template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
    for (double el : v) {
        out << std::right << std::setw(8) << el << ' ';
    }
    return out;
}

template <class T>
dvector operator+(std::vector<T>v, const std::vector<T>& b)
{
    for (int i = 0; i < v.size(); i++)
        v[i] += b[i];
    return v;
}

template<class T>
dvector operator*(std::vector<T> v, db num) {
    for (auto &i : v)
        i *= num;
    return v;
}

template<class T>
dvector operator+(std::vector<T> v, db num) {
    for (auto &i : v)
        i += num;
    return v;
}

template<class T>
dvector operator*(const std::vector<std::vector<T>>& a, const std::vector<T>& x)
{
    int m = a.size();
    dvector r(m, 0);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            r[i] += a[i][j] * x[j];
    return r;
}

template<class T>
std::vector<std::vector<T>> operator+(std::vector<std::vector<T>> a, db num)
{
    int m = a[0].size(), n = a.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            a[i][j] += num;
    return a;
}

template<class T>
dmatrix operator*(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b)
{
    dmatrix res(a.size(), dvector(b[0].size(), 0));
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b[0].size(); j++)
            for (int k = 0; k < b.size(); k++)
                res[i][j] += a[i][k] * b[k][j];
    return res;
}

template<class T>
db GetMin(const std::vector<std::vector<T>>& a){
    int m = a[0].size(), n = a.size();
    T mn = a[0][0];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if(mn > a[i][j])
                mn = a[i][j];
    return mn;
}
