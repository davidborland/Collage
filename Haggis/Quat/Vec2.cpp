///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Vec2.cpp
//
// Author:      David Borland
//
// Description: 2D vector class for C++ version of quatlib.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Vec2.h"


// Set indeces
const int Vec2::_X_ = 0;
const int Vec2::_Y_ = 1;


///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////////////////////

Vec2::Vec2() {
    MakeIdentity();
}

Vec2::Vec2(double x, double y) {
    Set(x, y);
}

Vec2::Vec2(const Vec3& v) {
    Set(v);
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Set values
///////////////////////////////////////////////////////////////////////////////////////////////

void Vec2::MakeIdentity() {
    Set(0.0, 0.0);
}

void Vec2::Set(double x, double y) {
    vec[_X_] = x;
    vec[_Y_] = y;
}

void Vec2::Set(const Vec3& v) {
    vec[_X_] = v.X();
    vec[_Y_] = v.Y();
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Element access
///////////////////////////////////////////////////////////////////////////////////////////////

double& Vec2::X() {
    return vec[_X_];
}

double& Vec2::Y() {
    return vec[_Y_];
}


double Vec2::X() const {
    return vec[_X_];
}

double Vec2::Y() const {
    return vec[_Y_];
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////////////////////

const Vec2 Vec2::operator+(const Vec2& v) const {
    return Vec2(vec[_X_] + v.vec[_X_],
                vec[_Y_] + v.vec[_Y_]);
}

const Vec2 Vec2::operator-(const Vec2& v) const {
    return Vec2(vec[_X_] - v.vec[_X_],
                vec[_Y_] - v.vec[_Y_]);
}


const Vec2 Vec2::operator*(double scale) const {
    return Vec2(vec[_X_] * scale, 
                vec[_Y_] * scale);
}


Vec2& Vec2::operator+=(const Vec2& v) {
    return (*this = *this + v);
}

Vec2& Vec2::operator-=(const Vec2& v) {
    return (*this = *this - v);
}


Vec2& Vec2::operator*=(double scale) {
    return (*this = *this * scale);
}


const Vec2 Vec2::operator!() const {
    return Vec2(-vec[_X_], -vec[_Y_]);
}


bool Vec2::operator==(const Vec2& v) const {
    return (vec[_X_] == v.vec[_X_] &&
            vec[_Y_] == v.vec[_Y_]);
}

bool Vec2::operator!=(const Vec2& v) const {
    return (vec[_X_] != v.vec[_X_] &&
            vec[_Y_] != v.vec[_Y_]);
}


bool Vec2::operator<(const Vec2& v) const {
    return (Magnitude() < v.Magnitude());
}

bool Vec2::operator>(const Vec2& v) const {
    return (Magnitude() > v.Magnitude());
}

bool Vec2::operator<=(const Vec2& v) const {
    return (Magnitude() <= v.Magnitude());
}

bool Vec2::operator>=(const Vec2& v) const {
    return (Magnitude() >= v.Magnitude());
}


std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return (os << "(" 
               << v.vec[Vec2::_X_] << ", "
               << v.vec[Vec2::_Y_] 
               << ")");
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////////////////////

double Vec2::DotProduct(const Vec2& v) const {
    return (vec[_X_] * v.vec[_X_] +
            vec[_Y_] * v.vec[_Y_]);
}

double Vec2::Distance(const Vec2& v) const {
    Vec2 diff = *this - v;
    return diff.Magnitude();
}

double Vec2::Magnitude() const {
    return sqrt(vec[_X_] * vec[_X_] +
                vec[_Y_] * vec[_Y_]);
}

void Vec2::Normalize() {
    double magnitude = Magnitude();

    if (magnitude <= 0.0) {
        std::cout << "Vec2::Normalize() : Vector has 0 magnitude." << std::endl;
        return;
    }

    double scale = 1.0 / magnitude;

    vec[_X_] *= scale;
    vec[_Y_] *= scale;
}