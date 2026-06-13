#ifndef __HM_H__
#define __HM_H__
#include <cmath>
#include <array>
using namespace std;

class HM
{
public:  // métodos

    HM(); // constructor
    array<array<float, 4>, 4> getHM(const float dh_parameters[4]);
    array<array<float, 4>, 4> mult_matrix(const array<array<float, 4>, 4> &A, const array<array<float, 4>, 4> &B);
    float degToRad(float deg);
    static array<float, 3> getEulerAngles(const array<array<float, 4>, 4>& H);
};

#endif // __HM_H__
