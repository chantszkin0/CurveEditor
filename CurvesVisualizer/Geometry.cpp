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
Geometry::Geometry(double* arr, int n) : vectorCount(0), data(0)
{
    // Check for unmatch dimension
    if(n%3){
        std::cout << "ERROR::GEOMETRY::UNMATCHED_ARRAY_SIZE::MUST_BE_COLLECTION_OF_VECTOR3\n";
        return;
    }
    // Convert array to geometry
    vectorCount = n/3;
    data = new Vector3[vectorCount];
    for(int i = 0; i < vectorCount; ++i){
        data[i] = Vector3(arr[i*3], arr[i*3+1], arr[i*3+2]);
    }
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
Vector3 Geometry::getFirst() const
{
    return data[0];
}
Matrix Geometry::getMatrix4(int startIndex, bool loop) const
{
    int i0, i1, i2, i3;
    if(!loop && (startIndex < 0 || startIndex > vectorCount-1)){
        std::cout << "ERROR::GEOMETRY::getMatrix4::INVALID_START_INDEX\n";
        return Matrix(4, 4);
    }
    if(loop){
        i0 = (startIndex==vectorCount-1)? 0 : startIndex;
        i1 = (data[i0+1]==data[vectorCount-1])? i0+2 : i0+1;
        i2 = (data[i1+1]==data[vectorCount-1])? i1+2 : i1+1;
        i3 = (data[i2+1]==data[vectorCount-1])? i2+2 : i2+1;
        return Matrix(data[i0%(vectorCount)], data[i1%(vectorCount)], data[i2%(vectorCount)], data[i3%(vectorCount)]);
    }
    i0 = startIndex;
    i1 = startIndex+1>=vectorCount? vectorCount-1 : startIndex+1;
    i2 = startIndex+2>=vectorCount? vectorCount-1 : startIndex+2;
    i3 = startIndex+3>=vectorCount? vectorCount-1 : startIndex+3;
    //std::cout << i0 << i1 << i2 << i3 << std::endl;
    return Matrix(data[i0], data[i1], data[i2], data[i3]);
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
