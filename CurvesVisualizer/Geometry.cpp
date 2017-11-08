//
//  Geometry.cpp
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 7/11/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#include "Geometry.h"

Geometry::Geometry() : vectorCount(0), data(0)
{
    
}
Geometry::Geometry(const Geometry& other) : vectorCount(other.size()), data(0)
{
    *this = other;
}
Geometry::~Geometry()
{
    delete[] data;
}

int Geometry::size() const
{
    return vectorCount;
}
bool Geometry::empty() const
{
    return (vectorCount >= 0);
}
Vector3* Geometry::getData() const
{
    return data;
}
Matrix Geometry::getMatrix4(int startIndex) const
{
    if(startIndex < 0 || startIndex > vectorCount-1){
        std::cout << "ERROR::GEOMETRY::getMatrix4::INVALID_START_INDEX\n";
        return Matrix(4, 4);
    }
    int i1 = startIndex+1>=vectorCount? vectorCount-1 : startIndex+1;
    int i2 = startIndex+2>=vectorCount? vectorCount-1 : startIndex+2;
    int i3 = startIndex+3>=vectorCount? vectorCount-1 : startIndex+3;
    //std::cout << startIndex << i1 << i2 << i3 << std::endl;
    return Matrix(data[startIndex], data[i1], data[i2], data[i3]);
}
void Geometry::pushVec3(const Vector3& other)
{
    Vector3* temp = new Vector3[vectorCount+1];
    for(int i = 0; i < vectorCount; ++i)
        temp[i] = data[i];
    temp[vectorCount++] = other;
    delete[] data;
    data = temp;
}
Vector3 Geometry::popVec3()
{
    if(this->vectorCount-1 < 0){
        std::cout << "ERROR::GEOMETRY::popVec3::ARRAY_IS_EMPTY\n";
        return Vector3();
    }
    Vector3* temp = new Vector3[--vectorCount];
    for(int i = 0; i < vectorCount; ++i)
        temp[i] = data[i];
    Vector3 pop = data[vectorCount];
    delete[] data;
    data = temp;
    return pop;
}
void Geometry::clear()
{
    vectorCount = 0;
    delete[] data;
}

void Geometry::print() const
{
    std::cout << *this;
}


std::ostream& operator<< (std::ostream& os, const Geometry& other)
{
    for(int i = 0; i < other.vectorCount; ++i)
        os << other.data[i];
    return os;
}

Geometry Geometry::operator+(const Geometry& other)
{
    Vector3* temp = new Vector3[vectorCount + other.vectorCount];
    for(int i = 0; i < vectorCount; ++i)
        temp[i] = data[i];
    for(int i = 0; i < other.size(); ++i)
        temp[vectorCount+i] = other.data[i];
    delete[] data;
    data = temp;
    vectorCount += other.vectorCount;
    return *this;
}
Geometry& Geometry::operator=(const Geometry& other)
{
    vectorCount = other.vectorCount;
    Vector3* temp = new Vector3[other.vectorCount];
    for(int i = 0; i < other.size(); ++i)
        temp[i] = other.data[i];
    delete[] data;
    data = temp;
    return *this;
}
Vector3& Geometry::operator[] (int i)
{
    return data[i];
}
Vector3 Geometry::operator[] (int i) const
{
    return data[i];
}
