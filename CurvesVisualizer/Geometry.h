//
//  Geometry.h
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 7/11/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#include "Vector3.h"
#include "Matrix.h"

class Geometry{
public:
    Geometry();
    Geometry(const Geometry&);
    ~Geometry();
    
    int size() const;
    bool empty() const;
    Vector3* getData() const;
    Matrix getMatrix4(int) const;
    void pushVec3(const Vector3&);
    Vector3 popVec3();
    void clear();
    
    void print() const;
    friend std::ostream& operator<< (std::ostream&, const Geometry&);
    
    Geometry operator+ (const Geometry&);
    Geometry& operator= (const Geometry&);
    Vector3& operator[] (int);
    Vector3 operator[] (int) const;
    
private:
    int vectorCount;
    Vector3* data;
};

#endif /* Geometry_h */
