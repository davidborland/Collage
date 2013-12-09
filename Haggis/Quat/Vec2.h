///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Vec2.h
//
// Author:      David Borland
//
// Description: 2D vector class for C++ version of quatlib.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef VEC2_H
#define VEC2_H


#include <iostream>
#include <math.h>

#include "Vec3.h"


class Vec3;


class Vec2 {
public:
    // Constructors
    Vec2();
    Vec2(double x, double y);
    Vec2(const Vec3& v);


    // Use default copy constructor
    // Use default destructor
    // Use default assignment operator


    // Set values
    void MakeIdentity();                                                // Set to (0, 0)
    void Set(double x, double y);                                       // Set directly
    void Set(const Vec3& v);                                            // Set from X and Y components of 3D vector

        
    // Element access
    double& X();                                                        // Read/write access
    double& Y();                                                        // Read/write access

    double X() const;                                                   // Just read access
    double Y() const;                                                   // Just read access


    // Operators
    const Vec2 operator+(const Vec2& v) const;                          // Vector addition
    const Vec2 operator-(const Vec2& v) const;                          // Vector subtraction

    const Vec2 operator*(double scale) const;                           // Scale

    Vec2& operator+=(const Vec2& v);                                    // Vector addition
    Vec2& operator-=(const Vec2& v);                                    // Vector subtraction

    Vec2& operator*=(double scale);                                     // Scale

    const Vec2 operator!() const;                                       // Invert

    bool operator==(const Vec2& v) const;                               // Equality
    bool operator!=(const Vec2& v) const;                               // Inequality

    bool operator<(const Vec2& v) const;                                // Magnitude less than
    bool operator>(const Vec2& v) const;                                // Magnitude greater than
    bool operator<=(const Vec2& v) const;                               // Magnitude less than or equal to
    bool operator>=(const Vec2& v) const;                               // Magnitude greater than or equal to

    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);   // Output to a stream


    // Utilities
    double DotProduct(const Vec2& v) const;
    double Distance(const Vec2& v) const;
    double Magnitude() const;
    void Normalize();

private:
    double vec[2];

    // Indeces
    const static int _X_;
    const static int _Y_;
};


#endif