#include <cassert>
#include "Matrix.h"

void testMatrix() {
    const int mxSize = 3;
    const long double eps = 0.0000001;

    Matrix <long double> mx;

    // test identity matrix and operator()
    {
        for (int col = 0; col < mxSize; ++col)
            for (int row = 0; row < mxSize; ++row)
                if (col == row)
                    assert(mx(row, col) == 1);
                else
                    assert(mx(row, col) == 0);
    }

    // test scale matrix
    {
        mx = Matrix<long double>::scaleMx(2, 3);
        assert(mx(0, 0) == 2);
        assert(mx(1, 1) == 3);
        assert(mx(2, 2) == 1);
        for (int col = 0; col < mxSize; ++col)
            for (int row = 0; row < mxSize; ++row)
                if (col != row)
                    assert(mx(row, col) == 0);
    }

    // test translate matrix
    {
        mx = Matrix<long double>::translateMx(2, 3);
        assert(mx(0, 0) == 1);
        assert(mx(1, 1) == 1);
        assert(mx(2, 2) == 1);
        assert(mx(0, 2) == 2);
        assert(mx(1, 2) == 3);
        for (int col = 0; col < mxSize; ++col)
            for (int row = 0; row < mxSize; ++row)
                if (col != row && col != 2)
                    assert(mx(row, col) == 0);
    }

    // test rotate matrix
    {
        mx = Matrix<long double>::rotateMx(2*M_PI);
        assert(mx(0, 0) - 1 < eps);
        assert(mx(1, 1) - 1 < eps);
        assert(mx(2, 2) - 1 < eps);
        for (int col = 0; col < mxSize; ++col)
            for (int row = 0; row < mxSize; ++row)
                if (col != row)
                    assert(mx(row, col) < eps);
        mx = Matrix<long double>::rotateMx(M_PI);
        assert(mx(0, 0) + 1 < eps);
        assert(mx(1, 1) + 1 < eps);
        assert(mx(2, 2) - 1 < eps);
        for (int col = 0; col < mxSize; ++col)
            for (int row = 0; row < mxSize; ++row)
                if (col != row && col != 2)
                    assert(mx(row, col) < eps);
    }

    // test multiply
    {
        mx = Matrix<long double>::scaleMx(2, 3);
        Matrix<long double> mx2 = Matrix<long double>::translateMx(2, 3);
        Matrix<long double> mx3 = mx * mx2;
        assert(mx3(0, 0) == 2);
        assert(mx3(1, 1) == 3);
        assert(mx3(2, 2) == 1);
        assert(mx3(0, 2) == 4);
        assert(mx3(1, 2) == 9);
        for (int col = 0; col < mxSize; ++col)
            for (int row = 0; row < mxSize; ++row)
                if (col != row && col != 2)
                    assert(mx3(row, col) == 0);
    }
    
    // test point transformation
    {
        mx = Matrix<long double>::scaleMx(2, 3);
        FPoint p(1, 1);
        FPoint p2 = mx.transform(p);
        assert(p2.x == 2);
        assert(p2.y == 3);
        mx = Matrix<long double>::translateMx(-2, -3);
        p2 = mx.transform(p);
        assert(p2.x == -1);
        assert(p2.y == -2);
    }
}

int main() {

    cout << "Starting tests...\n";

    testMatrix();

    cout << "Tests passed.\n";

    return 0;
}