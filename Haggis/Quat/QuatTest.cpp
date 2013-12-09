///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        QuatTest.cpp
//
// Author:      David Borland
//
// Description: Program for testing Quat library
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Vec3.h"
#include "Quat.h"

#include <quat.h>


class Test {
public:
    Test() {}

    const Vec3& Read() const { return vec; }
    Vec3& ReadWrite() { return vec; }

private:
    Vec3 vec;
};


void main() {
    std::cout << std::endl << "**** Vector Operations ****" << std::endl << std::endl << std::endl;


    Test test;
    test.ReadWrite().X() = 1;
//    test.Read().X() = 2;
    std::cout << "here" << test.Read() << std::endl << std::endl;
    std::cout << test.Read().X() << std::endl << std::endl;


    // Constructors
    Vec3 v1;
    Vec3 v2(1, 2, 3);
    Vec3 v3(v2);
    Vec3 v4 = v3;

    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << v3 << std::endl;
    std::cout << v4 << std::endl << std::endl;

    // Assignment
    v1 = v4;
    std::cout << v1 << std::endl << std::endl;

    // Identity
    v4.MakeIdentity();
    std::cout << v4 << std::endl << std::endl;

    // Index
    v3.Z() = v3.X();
    v3.X() = 2.0;
    std::cout << v3 << std::endl << std::endl;

    // Dot product
    std::cout << v1.DotProduct(v3) << std::endl << std::endl;

    // Cross product
    std::cout << v1 * v3 << std::endl;
    v1 *= v3;
    std::cout << v1 << std::endl << std::endl;

    // Addition
    std::cout << v1 + v3 << std::endl << std::endl;

    // Subtraction
    std::cout << v1 - v3 << std::endl << std::endl;

    // Scaling
    std::cout << v3 * 2 << std::endl;
    v3 *= 2;
    std::cout << v3 << std::endl << std::endl;

    // Magnitude 
    std::cout << v3.Magnitude() << std::endl << std::endl;

    // Inversion
    std::cout << !v3 << std::endl << std::endl;

    // Distance
    std::cout << v3.Distance(v1) << std::endl << std::endl;

    // Normalize
    v3.Normalize();
    std::cout << v3 << std::endl << std::endl;

    // Comparison
    v1.Set(1, 1, 1);
    v2.Set(1, 1, 1);
    std::cout << v1 << "  " << v2 << " : " << (v1 == v2) << std::endl << std::endl;
    std::cout << v1 << "  " << v2 << " : " << (v1 != v2) << std::endl << std::endl;

    std::cout << (v1 < v2) << std::endl;
    std::cout << (v1 > v2) << std::endl;
    std::cout << (v1 <= v2) << std::endl;
    std::cout << (v1 >= v2) << std::endl << std::endl;

    v2.Set(2, 2, 2);    
    std::cout << (v1 < v2) << std::endl;
    std::cout << (v1 > v2) << std::endl;
    std::cout << (v1 <= v2) << std::endl;
    std::cout << (v1 >= v2) << std::endl << std::endl;


///////////////////////////////////////////////////////////////////////////////////////


    std::cout << std::endl << std::endl << "**** Quaternion Operations ****" << std::endl << std::endl << std::endl;


    // Constructors
    Quat q1;
    Quat q2(0.5, 0.1, 0.1, 0.1);
    Quat q3(q2);
    Quat q4 = q3;
    Quat q5(0.5, Vec3(0.1, 0.1, 0.1));
    Quat q6(Vec3(1.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0));

    std::cout << q1 << std::endl;
    std::cout << q2 << std::endl;
    std::cout << q3 << std::endl;
    std::cout << q4 << std::endl;
    std::cout << q5 << std::endl;
    std::cout << q6 << std::endl << std::endl;

    // Assignment
    q1 = q4;
    std::cout << q1 << std::endl << std::endl;

    // Identity
    q4.MakeIdentity();
    std::cout << q4 << std::endl << std::endl;

    // Index
    q3.Z() = q3.W();
    q3.W() = 2.0;
    std::cout << q3 << std::endl << std::endl;

    // Multiplication
    q1.MakeIdentity();
    q2 = Quat(0.1, 0.2, 0.3, 0.4);
    q3 = q1 * q2;
    std::cout << q1 << "  " << q2 << "  " << q3 << std::endl;
    std::cout << q2 * q3 << std::endl;
    std::cout << q2 * q2 << std::endl << std::endl;

    // Normalize
    q2.Normalize();
    q1.Normalize();
    std::cout << q2 << std::endl;
    std::cout << q1 << std::endl << std::endl;

    // Conjugate
    q2 = ~q2;
    std::cout << q2 << std::endl;
    std::cout << ~q2 << std::endl << std::endl;

    // Inverse
    q1 = Quat(0.4, 0.3, 0.2, 0.1);
    q2 = !q1;
    q1 = !q1;
    std::cout << q1 << std::endl;
    std::cout << q2 << std::endl;
    std::cout << !q1 << std::endl << std::endl;

    // Angle Axis
    q1 = Quat(0.9, 0.3, 0.4, 0.3);
    double angle;
    Vec3 axis;
    q1.GetAngleAxis(angle, axis);
    std::cout << q1 << std::endl;
    std::cout << angle << " " << axis << std::endl << std::endl;

    // Comparison
    q1.Set(0.5, Vec3(1, 1, 1));
    q2.Set(0.5, Vec3(1, 1, 1));
    std::cout << q1 << "  " << q2 << " : " << (q1 == q2) << std::endl << std::endl;
    std::cout << q1 << "  " << q2 << " : " << (q1 != q2) << std::endl << std::endl;


///////////////////////////////////////////////////////////////////////////////////////


    std::cout << std::endl << std::endl << "**** Test ****" << std::endl << std::endl << std::endl;
    q1 = Quat(0.5, Vec3(0, 1, 0));
    v1 = Vec3(1, 2, 3);

    std::cout << q1 << std::endl;
    std::cout << q1 * v1 << std::endl << std::endl;

q_type q;
q_vec_type vec;

q_make(q, 0, 1, 0, 0.5);
q_vec_set(vec, 1, 2, 3);

q_type  inverse;
q_type  vecQuat;
q_type  tempVecQuat;
q_type  resultQuat;
q_type destVec;

/* copy vector into temp quaternion for multiply   */
q_from_vec(vecQuat, vec);

/* invert multiplier */
q_invert(inverse, q);


/* do q * vec * q(inv)  */
q_mult(tempVecQuat, q, vecQuat);
q_mult(resultQuat, tempVecQuat, inverse);

/* back to vector land  */
q_to_vec(destVec, resultQuat);


q_print(q);
q_vec_print(destVec);

std::cout << std::endl << std::endl;

q_vec_type ypr;
q_vec_type ypr2;
ypr[0] = 0.9;
ypr[1] = 0.2;
ypr[2] = 0.5;
std::cout << ypr[0] << " " << ypr[1] << " " << ypr[2] << std::endl;
q_from_euler(q, ypr[0], ypr[1], ypr[2]);
q_print(q);
printf("\n");
q_to_euler(ypr2, q);
std::cout << ypr2[0] << " " << ypr2[1] << " " << ypr2[2] << std::endl;

q1 = Quat(ypr[0], ypr[1], ypr[2]);
std::cout << q1 << std::endl;
q1.GetEulerAngles(ypr2[0], ypr2[1], ypr2[2]);
std::cout << ypr2[0] << " " << ypr2[1] << " " << ypr2[2] << std::endl;

}