///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Vec3.h
//
// Author:      David Borland
//
// Description: 3D vector class for C++ version of quatlib.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef VEC3_H
#define VEC3_H


#include <iostream>
#include <math.h>

#include "Vec2.h"


class Vec2;


class Vec3 {
public:
    // Constructors
    Vec3();                                                             // Set to (0, 0, 0)
    Vec3(double x, double y, double z);                                 // Set directly
    Vec3(const Vec2& v);                                                // Set from 2D vector (Z == 0)


    // Use default copy constructor
    // Use default destructor
    // Use default assignment operator


    // Set values
    void MakeIdentity();                                                // Set to (0, 0, 0)
    void Set(double x, double y, double z);                             // Set directly
    void Set(const Vec2& v);                                            // Set from 2D vector (Z == 0)

        
    // Element access
    double& X();                                                        // Read/write access
    double& Y();                                                        // Read/write access
    double& Z();                                                        // Read/write access

    double X() const;                                                   // Just read access
    double Y() const;                                                   // Just read access
    double Z() const;                                                   // Just read access


    // Operators
    const Vec3 operator+(const Vec3& v) const;                          // Vector addition
    const Vec3 operator-(const Vec3& v) const;                          // Vector subtraction

    const Vec3 operator*(const Vec3& v) const;                          // Cross product
    const Vec3 operator*(double scale) const;                           // Scale

    Vec3& operator+=(const Vec3& v);                                    // Vector addition
    Vec3& operator-=(const Vec3& v);                                    // Vector subtraction

    Vec3& operator*=(const Vec3& v);                                    // Cross product
    Vec3& operator*=(double scale);                                     // Scale

    const Vec3 operator!() const;                                       // Invert

    bool operator==(const Vec3& v) const;                               // Equality
    bool operator!=(const Vec3& v) const;                               // Inequality

    bool operator<(const Vec3& v) const;                                // Magnitude less than
    bool operator>(const Vec3& v) const;                                // Magnitude greater than
    bool operator<=(const Vec3& v) const;                               // Magnitude less than or equal to
    bool operator>=(const Vec3& v) const;                               // Magnitude greater than or equal to

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);   // Output to a stream


    // Utilities
    double DotProduct(const Vec3& v) const;
    double Distance(const Vec3& v) const;
    double Magnitude() const;
    void Normalize();

private:
    // All calculations are done with this
    double vec[3];                    

    // Indeces
    const static int _X_;
    const static int _Y_;
    const static int _Z_;
};


#endif