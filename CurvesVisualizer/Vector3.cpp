//
//  Vector3.cpp
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 25/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#include "Vector3.h"
#include <cmath>
#include <limits>



/*
 *
 *  Constructors and Destructor
 *
 */
Vector3::Vector3()
{
    x = y = z = 0.0;
}
Vector3::Vector3(double xa, double ya, double za)
{
    x = xa;
    y = ya;
    z = za;
}
Vector3::Vector3(const Vector3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}
Vector3::~Vector3(){}

/*
 *
 *  Member functions
 *
 */
double Vector3::distance(const Vector3& a, const Vector3& b)
{
    double xx = (a.getx()-b.getx())*(a.getx()-b.getx());
    double yy = (a.gety()-b.gety())*(a.gety()-b.gety());
    double zz = (a.getz()-b.getz())*(a.getz()-b.getz());
    return sqrt(xx+yy+zz);
}
void Vector3::print() const
{
    std::cout << *this;
}


/*
 *
 *  Operator Overloading
 *
 */
Vector3 Vector3::operator+(const Vector3& other) const
{
    double x = this->x + other.x;
    double y = this->y + other.y;
    double z = this->z + other.z;
    Vector3 temp(x, y, z);
    return temp;
}
Vector3 Vector3::operator-(const Vector3& other) const
{
    double x = this->x - other.x;
    double y = this->y - other.y;
    double z = this->z - other.z;
    Vector3 temp(x, y, z);
    return temp;
}
Vector3 Vector3::operator* (float scalar)
{
    return scalar * (*this);
}
Vector3& Vector3::operator+= (const Vector3& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}
Vector3& Vector3::operator-= (const Vector3& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}
Vector3& Vector3::operator= (const Vector3& other)
{
    if(this != &other){
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }
    return *this;
}
bool Vector3::operator== (const Vector3& other) const
{
    bool x_boolean = fabs(this->x - other.x) < std::numeric_limits<double>::epsilon();
    bool y_boolean = fabs(this->y - other.y) < std::numeric_limits<double>::epsilon();
    bool z_boolean = fabs(this->z - other.z) < std::numeric_limits<double>::epsilon();
    return (x_boolean && y_boolean && z_boolean);
}
