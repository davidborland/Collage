///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Vec3.cpp
//
// Author:      David Borland
//
// Description: 3D vector class for C++ version of quatlib.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Vec3.h"


// Set indeces
const int Vec3::_X_ = 0;
const int Vec3::_Y_ = 1;
const int Vec3::_Z_ = 2;


///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////////////////////

Vec3::Vec3() {
    MakeIdentity();
}

Vec3::Vec3(double x, double y, double z) {
    Set(x, y, z);
}

Vec3::Vec3(const Vec2& v) {
    Set(v);
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Set values
///////////////////////////////////////////////////////////////////////////////////////////////

void Vec3::MakeIdentity() {
    Set(0.0, 0.0, 0.0);
}

void Vec3::Set(double x, double y, double z) {
    vec[_X_] = x;
    vec[_Y_] = y;
    vec[_Z_] = z;
}

void Vec3::Set(const Vec2& v) {
    vec[_X_] = v.X();
    vec[_Y_] = v.Y();
    vec[_Z_] = 0.0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Element access
///////////////////////////////////////////////////////////////////////////////////////////////

double& Vec3::X() {
    return vec[_X_];
}

double& Vec3::Y() {
    return vec[_Y_];
}

double& Vec3::Z() {
    return vec[_Z_];
}


double Vec3::X() const {
    return vec[_X_];
}

double Vec3::Y() const {
    return vec[_Y_];
}

double Vec3::Z() const {
    return vec[_Z_];
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////////////////////

const Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(vec[_X_] + v.vec[_X_],
                vec[_Y_] + v.vec[_Y_],
                vec[_Z_] + v.vec[_Z_]);
}

const Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(vec[_X_] - v.vec[_X_],
                vec[_Y_] - v.vec[_Y_],
                vec[_Z_] - v.vec[_Z_]);
}


const Vec3 Vec3::operator*(const Vec3& v) const {
    return Vec3(vec[_Y_] * v.vec[_Z_] - vec[_Z_] * v.vec[_Y_],
                vec[_Z_] * v.vec[_X_] - vec[_X_] * v.vec[_Z_],
                vec[_X_] * v.vec[_Y_] - vec[_Y_] * v.vec[_X_]);
}

const Vec3 Vec3::operator*(double scale) const {
    return Vec3(vec[_X_] * scale, 
                vec[_Y_] * scale,
                vec[_Z_] * scale);
}


Vec3& Vec3::operator+=(const Vec3& v) {
    return (*this = *this + v);
}

Vec3& Vec3::operator-=(const Vec3& v) {
    return (*this = *this - v);
}


Vec3& Vec3::operator*=(const Vec3& v) {
    return (*this = *this * v);
}

Vec3& Vec3::operator*=(double scale) {
    return (*this = *this * scale);
}


const Vec3 Vec3::operator!() const {
    return Vec3(-vec[_X_], -vec[_Y_], -vec[_Z_]);
}


bool Vec3::operator==(const Vec3& v) const {
    return (vec[_X_] == v.vec[_X_] &&
            vec[_Y_] == v.vec[_Y_] &&
            vec[_Z_] == v.vec[_Z_]);
}

bool Vec3::operator!=(const Vec3& v) const {
    return (vec[_X_] != v.vec[_X_] &&
            vec[_Y_] != v.vec[_Y_] &&
            vec[_Z_] != v.vec[_Z_]);
}


bool Vec3::operator<(const Vec3& v) const {
    return (Magnitude() < v.Magnitude());
}

bool Vec3::operator>(const Vec3& v) const {
    return (Magnitude() > v.Magnitude());
}

bool Vec3::operator<=(const Vec3& v) const {
    return (Magnitude() <= v.Magnitude());
}

bool Vec3::operator>=(const Vec3& v) const {
    return (Magnitude() >= v.Magnitude());
}


std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    return (os << "(" 
               << v.vec[Vec3::_X_] << ", "
               << v.vec[Vec3::_Y_] << ", "
               << v.vec[Vec3::_Z_] 
               << ")");
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////////////////////

double Vec3::DotProduct(const Vec3& v) const {
    return (vec[_X_] * v.vec[_X_] +
            vec[_Y_] * v.vec[_Y_] +
            vec[_Z_] * v.vec[_Z_]);
}

double Vec3::Distance(const Vec3& v) const {
    Vec3 diff = *this - v;
    return diff.Magnitude();
}

double Vec3::Magnitude() const {
    return sqrt(vec[_X_] * vec[_X_] +
                vec[_Y_] * vec[_Y_] + 
                vec[_Z_] * vec[_Z_]);
}

void Vec3::Normalize() {
    double magnitude = Magnitude();

    if (magnitude <= 0.0) {
//        std::cout << "Vec3::Normalize() : Vector has 0 magnitude." << std::endl;
        return;
    }

    double scale = 1.0 / magnitude;

    vec[_X_] *= scale;
    vec[_Y_] *= scale;
    vec[_Z_] *= scale;
}