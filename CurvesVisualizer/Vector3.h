//
//  Vector3.h
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 25/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#ifndef Vector3_h
#define Vector3_h

#include <iostream>

class Vector3 {
public:
    // Constructors and Destructor
    Vector3();
    Vector3(double xa, double ya = 0, double za = 0);
    Vector3(const Vector3& other);
    ~Vector3();
    
    // Member functions
    double getx() const    {return x;}
    double gety() const    {return y;}
    double getz() const    {return z;}
    void print() const;
    
    // Operator overloading
    Vector3 operator+ (const Vector3& other) const;
    Vector3 operator- (const Vector3& other) const;
    friend Vector3 operator* (float scalar, const Vector3& other)
    {
        Vector3 temp(other);
        temp.x *= scalar;  temp.y *= scalar;  temp.z *= scalar;
        return temp;
    }
    Vector3 operator* (float scalar);
    Vector3& operator+= (const Vector3& other);
    Vector3& operator-= (const Vector3& other);
    void operator= (const Vector3& other);
    bool operator== (const Vector3& other) const;
    friend std::ostream& operator<< (std::ostream& os, const Vector3& other)
    {
        os << "[" << other.x << ", " << other.y << ", " << other.z << "]\n";
        return os;
    }
    
private:
    // Private data
    double x, y, z;
};


#endif /* Vector3_h */
