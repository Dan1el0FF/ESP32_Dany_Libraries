#ifndef __KINEMATICS_H__
#define __KINEMATICS_H__

#include <cmath>
#include <array>
#include <stdio.h>
using namespace std;

class Kinematics {
    
    private: //Atributos

    int joints;
    float(*dh)[4];

    public: //Metodos

    Kinematics(); //constructor
    void setup(float _dh_parameters[][4],int _joints); // dar parametros dh y articulaciones
    void calculate_FK(float _pose[6]); // recibes x,y,z,roll,pitch,yaw
    bool calculate_IK(float _coords[4],float solutions[2][4]); //dar x,y,z,phi, recibes 2 soluciones theta_1,theta_2,theta_3,Z
    void calculate_Jacobian(float joint_vel[4],float operational_vel[6]); //dar velocidades articulares theta_1,theta_2,theta_3,Z recibes vx,vy,vz,wx,wy,wz
    void update_variables(float _joint_variables[4]); //dar theta_1,theta_2,theta_3,Z
    int best_move(float solutions[2][4]); //dar las 2 soluciones, recibes 0/1
    array<array<float, 4>, 4> getHM(const float dh_parameters[4]);
    array<array<float, 4>, 4> mult_matrix(const array<array<float, 4>, 4> &A, const array<array<float, 4>, 4> &B);
    static array<float, 3> getEulerAngles(const array<array<float, 4>, 4>& H);
    float degToRad(float deg);
    float radToDeg(float rad);
};
#endif // __KINEMATICS_H__
