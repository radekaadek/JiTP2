#include "Matrix.h"

int main() {
    // test identity matrix
    Matrix <long double> mx;

    cout << mx << '\n';

    // test scale matrix
    mx = Matrix<long double>::scaleMx(2, 3);
    cout << mx << '\n';

    // test translate matrix
    mx = Matrix<long double>::translateMx(2, 3);
    cout << mx << '\n';

    // test rotate matrix
    mx = Matrix<long double>::rotateMx(2*M_PI);
    cout << mx << '\n';

    // test transform
    FPoint pt(1, 1);
    cout << mx.transform(pt) << '\n';

    // test multiply
    mx = Matrix<long double>::scaleMx(2, 3);
    Matrix<long double> mx2 = Matrix<long double>::translateMx(2, 3);
    cout << mx << "*\n" << mx2 << "=\n" << mx * mx2 << '\n';

    return 0;

}