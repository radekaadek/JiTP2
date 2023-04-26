#pragma once
#include "FPoint.h"
#include <iostream>

template <typename T> class Matrix
{
    static const int size = 3;
    std::array<std::array<T, size>, size> cf;
    void setIdentity();
    void copyMx(const Matrix& rhm);
public:
    Matrix() { setIdentity(); }
    Matrix(const Matrix& rhm) { copyMx(rhm); }
    Matrix& operator = (const Matrix& rhm) { copyMx(rhm); return *this; }
    static Matrix scaleMx(T scaleX, T scaleY);
    static Matrix translateMx(T offsetX, T offsetY);
    static Matrix rotateMx(T angle);
    friend std::ostream& operator << (std::ostream& os, const Matrix& mx)
    {
    for (auto row : mx.cf)
    {
        for (auto col : row)
            os << col << " ";
        os << '\n';
    }
    return os;
    }
    friend Matrix operator * (const Matrix& lhm, const Matrix& rhm)
    {
        Matrix<T> res;
        for (int i = 0; i < size; ++i)
            res.cf[i][i] = 0;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                for (int k = 0; k < size; ++k)
                    res.cf[i][j] += lhm.cf[i][k] * rhm.cf[k][j];
        return res;
    }
    Matrix& operator *= (const Matrix& rhm);
    FPoint transform(const FPoint& pt) const;
    const T& operator() (unsigned int rowid, unsigned int colid) const;
    T& operator() (unsigned int rowid, unsigned int colid);
};

template <typename T>
void Matrix<T>::setIdentity()
{
    for (std::array<T, size>& row : cf)
        std::fill(row.begin(), row.end(), 0);
    for (int i = 0; i < size; ++i)
        cf[i][i] = 1;
}

template <typename T>
void Matrix<T>::copyMx(const Matrix& rhm)
{
    std::copy(rhm.cf.begin(), rhm.cf.end(), cf.begin());
}

template <typename T>
Matrix<T> Matrix<T>::scaleMx(T scaleX, T scaleY)
{
    Matrix<T> mx;
    mx.cf[0][0] = scaleX;
    mx.cf[1][1] = scaleY;
    return mx;
}

template <typename T>
Matrix<T> Matrix<T>::translateMx(T offsetX, T offsetY)
{
    Matrix<T> mx;
    mx.cf[0][2] = offsetX;
    mx.cf[1][2] = offsetY;
    return mx;
}

template <typename T>
Matrix<T> Matrix<T>::rotateMx(T angle)
{
    Matrix<T> mx;
    mx.cf[0][0] = cos(angle);
    mx.cf[0][1] = -sin(angle);
    mx.cf[1][0] = sin(angle);
    mx.cf[1][1] = cos(angle);
    return mx;
}

template <typename T>
Matrix<T>& Matrix<T>::operator *= (const Matrix<T>& rhm)
{
    *this = *this * rhm;
    return *this;
}

template <typename T>
FPoint Matrix<T>::transform(const FPoint& pt) const
{
    FPoint res;
    res.x = cf[0][0] * pt.x + cf[0][1] * pt.y + cf[0][2];
    res.y = cf[1][0] * pt.x + cf[1][1] * pt.y + cf[1][2];
    return res;
}

template <typename T>
const T& Matrix<T>::operator() (unsigned int rowid, unsigned int colid) const
{
    return cf[rowid][colid];
}

template <typename T>
T& Matrix<T>::operator() (unsigned int rowid, unsigned int colid)
{
    return cf[rowid][colid];
}
