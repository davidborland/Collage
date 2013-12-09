///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Quat.h
//
// Author:      David Borland
//
// Description: C++ version of quatlib.  Quaternion values are w, x, y, z.
//              Angles are in radians.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef QUAT_H 
#define QUAT_H


#include <iostream>
#include <math.h>

#include "Vec3.h"


class Quat {
public:
    // Constructors
    Quat();
    Quat(double w, double x, double y, double z);                       // Set directly
    Quat(double angle, const Vec3& axis);                               // Convert from angle and axis
    Quat(double yaw, double pitch, double roll);                        // Convert from 3 euler angles (Yaw == Z, Pitch == Y, Roll == X)
    Quat(const Vec3& v1, const Vec3& v2);                               // Rotation from v1 to v2
    Quat(const Vec3& v);                                                // Set axis (W == 0)


    // Use default copy constructor
    // Use default destructor
    // Use default assignment operator

    
    // Set values
    void MakeIdentity();                                                // Set to [ 1, (0, 0, 0) ]
    void Set(double w, double x, double y, double z);                   // Set values directly
    void Set(double angle, const Vec3& axis);                           // Convert from angle and axis
    void Set(double yaw, double pitch, double roll);                    // Convert from 3 euler angles
    void Set(const Vec3& v1, const Vec3& v2);                           // Rotation from v1 to v2
    void Set(const Vec3& v);                                            // Set axis (W ==0)


    // Element access
    double& W();                                                        // Read/write access
    double& X();                                                        // Read/write access
    double& Y();                                                        // Read/write access
    double& Z();                                                        // Read/write access

    double W() const;                                                   // Just read access
    double X() const;                                                   // Just read access
    double Y() const;                                                   // Just read access
    double Z() const;                                                   // Just read access


    // Operators
    const Quat operator*(const Quat& q) const;                          // Quaternion multiplication
    const Vec3 operator*(const Vec3& v) const;                          // Rotate a vector

    Quat& operator*=(const Quat& q);                                    // Quaternion multiplication

    const Quat operator!() const;                                       // Quaternion inverse
    const Quat operator~() const;                                       // Quaternion conjugate

    bool operator==(const Quat& q) const;                               // Equality
    bool operator!=(const Quat& q) const;                               // Inequality

    friend std::ostream& operator<<(std::ostream& os, const Quat& q);   // Output to a stream


    // Utilities
    void GetAngleAxis(double& angle, Vec3& axis) const;
    void GetEulerAngles(double& yaw, double& pitch, double& roll) const;
    double Magnitude() const;
    void Normalize();
    
    const static double PI;
    static double RadiansToDegrees(double rad);
    static double DegreesToRadians(double deg);

private:
    double quat[4];

    // Indeces
    const static int _W_;
    const static int _X_;
    const static int _Y_;
    const static int _Z_;

    // Pi
    const static double RADTODEG;
    const static double DEGTORAD;

    void Make(double angle, const Vec3& axis);
};


#endif