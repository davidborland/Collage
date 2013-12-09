///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Quat.cpp
//
// Author:      David Borland
//
// Description: C++ version of quatlib
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Quat.h"


// Set indeces
const int Quat::_W_ = 0;
const int Quat::_X_ = 1;
const int Quat::_Y_ = 2;
const int Quat::_Z_ = 3;

// Set value for Pi
const double Quat::PI = 3.14159265358979323846;
const double Quat::RADTODEG = 180.0 / PI;
const double Quat::DEGTORAD = PI / 180.0;


///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////////////////////

Quat::Quat() {
    MakeIdentity();
}

Quat::Quat(double w, double x, double y, double z) {
    Set(w, x, y, z);
}


Quat::Quat(double angle, const Vec3& axis) {
    Set(angle, axis);
}

Quat::Quat(double yaw, double pitch, double roll) {
    Set(yaw, pitch, roll);
}

Quat::Quat(const Vec3& v1, const Vec3& v2) {
    Set(v1, v2);
}

Quat::Quat(const Vec3& v) {
    Set(v);
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Set values
///////////////////////////////////////////////////////////////////////////////////////////////

void Quat::MakeIdentity() {
    Set(1.0, 0.0, 0.0, 0.0);
}

void Quat::Set(double w, double x, double y, double z) {
    quat[_W_] = w;
    quat[_X_] = x;
    quat[_Y_] = y;
    quat[_Z_] = z;
}

void Quat::Set(double angle, const Vec3& axis) {
    Vec3 tempAxis = axis;

    // If zero vector, make identity
    if (tempAxis.Magnitude() <= 0.0) {
        MakeIdentity();
        return;
    }
    
    // Normalize the vector
    tempAxis.Normalize();

    double cosA = cos(angle / 2.0);
    double sinA = sin(angle / 2.0);

    Set(cosA, 
        sinA * tempAxis.X(),
        sinA * tempAxis.Y(),
        sinA * tempAxis.Z());
}

void Quat::Set(double yaw, double pitch, double roll) {
    double halfYaw = yaw / 2.0;
    double halfPitch = pitch / 2.0;
    double halfRoll = roll / 2.0;

    double cosYaw = cos(halfYaw);
    double sinYaw = sin(halfYaw);

    double cosPitch = cos(halfPitch);
    double sinPitch = sin(halfPitch);

    double cosRoll = cos(halfRoll);
    double sinRoll = sin(halfRoll);

    Set(cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw,

        sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
        cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
        cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw);
}

void Quat::Set(const Vec3& v1, const Vec3& v2) {
    // Normalize incoming vectors
    Vec3 vec1 = v1;
    Vec3 vec2 = v2;
    vec1.Normalize();
    vec2.Normalize();

    // Get the axis from the cross product
    Vec3 axis = vec1 * vec2;

    // Get the magnitude of the cross product
    double magnitude = axis.Magnitude();
    magnitude = magnitude > 1.0 ? 1.0 : magnitude;

    // Get the rotation angle
    double theta = asin(magnitude);
    double thetaComplement = PI - theta;

    // If cos(theta) < 0, use the complement
    if (vec1.DotProduct(vec2) < 0.0) {
        theta = thetaComplement;
        thetaComplement = PI - theta;
    }

    // If angle is 0, return identity
    if (theta <= 0.0) {
        MakeIdentity();
        return;
    }

    if (thetaComplement <= 0.0) {
        // The two vectors are opposed.  Find some arbitrary axis vector.
        // First try cross product with x-axis
        if (vec1.Y() * vec1.Y() + vec1.Z() * vec1.Z() > 0.0) {
            axis.Set(0.0, vec1.Z(), -vec1.Y());
        }
        else {
            // Parallel to x-axis, use z-axis instead
            axis.Set(0.0, 0.0, 1.0);
        }
    }

    axis.Normalize();
    Set(theta, axis);
}

void Quat::Set(const Vec3& v) {
    Vec3 temp = v;
    Set(0.0, temp.X(), temp.Y(), temp.Z());
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Element access
///////////////////////////////////////////////////////////////////////////////////////////////

double& Quat::W() {
    return quat[_W_];
}

double& Quat::X() {
    return quat[_X_];
}

double& Quat::Y() {
    return quat[_Y_];
}

double& Quat::Z() {
    return quat[_Z_];
}


double Quat::W() const {
    return quat[_W_];
}

double Quat::X() const {
    return quat[_X_];
}

double Quat::Y() const {
    return quat[_Y_];
}

double Quat::Z() const {
    return quat[_Z_];
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////////////////////

const Quat Quat::operator*(const Quat& q) const {
    return Quat(quat[_W_] * q.quat[_W_] - quat[_X_] * q.quat[_X_] - quat[_Y_] * q.quat[_Y_] - quat[_Z_] * q.quat[_Z_],
                quat[_W_] * q.quat[_X_] + quat[_X_] * q.quat[_W_] + quat[_Y_] * q.quat[_Z_] - quat[_Z_] * q.quat[_Y_],
                quat[_W_] * q.quat[_Y_] + quat[_Y_] * q.quat[_W_] + quat[_Z_] * q.quat[_X_] - quat[_X_] * q.quat[_Z_],
                quat[_W_] * q.quat[_Z_] + quat[_Z_] * q.quat[_W_] + quat[_X_] * q.quat[_Y_] - quat[_Y_] * q.quat[_X_]);
}

const Vec3 Quat::operator*(const Vec3& v) const {
    Quat vQuat(v);

    Quat temp = *this * vQuat * !*this;
    
    return Vec3(temp.quat[_X_], temp.quat[_Y_], temp.quat[_Z_]);
}


Quat& Quat::operator*=(const Quat& q) {
    return (*this = *this * q);
}


const Quat Quat::operator!() const {
    double scale = 1.0 / (quat[_W_] * quat[_W_] +
                          quat[_X_] * quat[_X_] +
                          quat[_Y_] * quat[_Y_] +
                          quat[_Z_] * quat[_Z_]);

    return Quat (quat[_W_] * scale, 
                -quat[_X_] * scale,
                -quat[_Y_] * scale,
                -quat[_Z_] * scale);
}

const Quat Quat::operator~() const {
    return Quat(quat[_W_], -quat[_X_], -quat[_Y_], -quat[_Z_]);
}


bool Quat::operator==(const Quat& q) const {
    return (quat[_W_] == q.quat[_W_] &&
            quat[_X_] == q.quat[_X_] &&
            quat[_Y_] == q.quat[_Y_] &&
            quat[_Z_] == q.quat[_Z_]);
}

bool Quat::operator!=(const Quat& q) const {
    return (quat[_W_] != q.quat[_W_] &&
            quat[_X_] != q.quat[_X_] &&
            quat[_Y_] != q.quat[_Y_] &&
            quat[_Z_] != q.quat[_Z_]);
}


std::ostream& operator<<(std::ostream& os, const Quat& q) {
    return (os << "[ " 
               << q.quat[Quat::_W_] << ", ("
               << q.quat[Quat::_X_] << ", "
               << q.quat[Quat::_Y_] << ", "
               << q.quat[Quat::_Z_] 
               << ") ]");
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////////////////////

void Quat::GetAngleAxis(double& angle, Vec3& axis) const {
    double magnitude = Magnitude();
    if (magnitude <= 0.0) {
        // Return zero rotation about Z
        angle = 0.0;
        axis = Vec3(0.0, 0.0, 1.0);
        return;
    }

    double scale = 1.0 / magnitude;

    angle = 2 * acos(quat[_W_]);
    axis = Vec3(quat[_X_], quat[_Y_], quat[_Z_]) * scale;
}

void Quat::GetEulerAngles(double& yaw, double& pitch, double& roll) const {
    // Code streamlined from q_to_euler, which converts to a full 4x4 matrix before converting to Euler angles


    // For unit quat, just set s = 2.0; or set xs = quat[X] + quat[X], etc. 
    double s = 2.0 / (quat[_X_] * quat[_X_] + quat[_Y_] * quat[_Y_] + 
                      quat[_Z_] * quat[_Z_] + quat[_W_] * quat[_W_]);

    double sinYaw, cosYaw, sinPitch, cosPitch, sinRoll, cosRoll;

    sinPitch = -(quat[_X_] * quat[_Z_] - quat[_W_] * quat[_Y_]) * s;
    cosPitch = sqrt(1.0 - sinPitch * sinPitch);

    if (fabs(cosPitch) > 1e-10) {
        sinRoll = (quat[_Y_] * quat[_Z_] + quat[_W_] * quat[_X_]) * s / cosPitch;
        cosRoll = (1.0 - (quat[_X_] * quat[_X_] + quat[_Y_] * quat[_Y_]) * s) / cosPitch;
        sinYaw = (quat[_X_] * quat[_Y_] + quat[_W_] * quat[_Z_]) * s / cosPitch;
        cosYaw = (1.0 - (quat[_Y_] * quat[_Y_] + quat[_Z_] * quat[_Z_]) * s) / cosPitch;
    }
    else {
        // Gimble lock
        sinRoll = -(quat[_Y_] * quat[_Z_] - quat[_W_] * quat[_X_]) * s;
        cosRoll = 1.0 - (quat[_X_] * quat[_X_] + quat[_Z_] * quat[_Z_]) * s;
        sinYaw = 0.0;
        cosYaw = 1.0;
    }   

    yaw = atan2(sinYaw, cosYaw);
    pitch = atan2(sinPitch, cosPitch);
    roll = atan2(sinRoll, cosRoll);
}

double Quat::Magnitude() const {
    return sqrt(quat[_W_] * quat[_W_] +
                quat[_X_] * quat[_X_] +
                quat[_Y_] * quat[_Y_] +
                quat[_Z_] * quat[_Z_]);
}

void Quat::Normalize() {
    // Does not check for zero magnitude
    double scale = 1.0 / Magnitude();

    quat[_W_] *= scale;
    quat[_X_] *= scale;
    quat[_Y_] *= scale;
    quat[_Z_] *= scale;
}


double Quat::RadiansToDegrees(double rad) {
    return rad * RADTODEG;
}

double Quat::DegreesToRadians(double deg) {
    return deg * DEGTORAD;
}