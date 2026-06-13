#include "HM.h"

HM::HM(void){}
array<array<float, 4>, 4> HM::getHM(const float dh_parameters[4]) {

    float t = dh_parameters[0]; //angulo theta
    float d = dh_parameters[1];
    float a = dh_parameters[2]; //angulo alpha
    float r = dh_parameters[3];
    t=degToRad(t);
    a=degToRad(a);
    array<array<float, 4>,4> H={{
        {cos(t), -cos(a)*sin(t),  sin(a)*sin(t), r*cos(t)},
        {sin(t),  cos(a)*cos(t), -sin(a)*cos(t), r*sin(t)},
        {0,       sin(a),         cos(a),        d},
        {0,       0,              0,             1}
    }};
    return H;
}
array<array<float, 4>, 4> HM::mult_matrix(const array<array<float, 4>, 4>& A, const array<array<float, 4>, 4>& B) {
    array<array<float, 4>, 4> result = {};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
array<float, 3> HM::getEulerAngles(const std::array<std::array<float, 4>, 4>& H) {
    float roll_z = atan2(H[2][1], H[2][2]) * 180.0 / M_PI;
    float pitch_y = atan2(-H[2][0], sqrt(H[2][1] * H[2][1] + H[2][2] * H[2][2])) * 180.0 / M_PI;
    float yaw_x = atan2(H[1][0], H[0][0]) * 180.0 / M_PI;
    return {roll_z, pitch_y, yaw_x};
}
float HM::degToRad(float deg){
    return deg*M_PI/180.0;
}
