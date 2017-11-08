//
//  Curve.cpp
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 8/11/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#include "Curve.h"

Curve::Curve(unsigned int shaderProgram)
{
    Shader = shaderProgram;
    // Allocate Polyline Buffer & Create
    glGenVertexArrays(1, &PolylineVAO);
    glGenBuffers(1, &PolylineVBO);
    CreatePolyline();
    // Allocate Curve Buffer & Create
    glGenVertexArrays(1, &CurveVAO);
    glGenBuffers(1, &CurveVBO);
    CreateCurve();
}
Curve::~Curve()
{
    
}

void Curve::CreateCurve ()
{
    if(BezierBSpline)
        CurvePoints = CreateBSplineCurve();
    else
        CurvePoints = CreateBezierCurve();
    
    glBindVertexArray(CurveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CurveVBO);
    
    glBufferData(GL_ARRAY_BUFFER, CurvePoints.size()*sizeof(Vector3), CurvePoints.getData(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vector3), (void*)0);
}
void Curve::CreatePolyline ()
{
    // Create Polyline
    glBindVertexArray(PolylineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, PolylineVBO);
    
    glBufferData(GL_ARRAY_BUFFER, ControlPoints.size()*sizeof(Vector3), ControlPoints.getData(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vector3), (void*)0);
}
Geometry Curve::CreateBezierCurve ()
{
    Geometry curve;
    Matrix geoMatrix;
    Matrix bezierMatrix = Matrix(BEZIER, 4, 4);
    for(int stride = 0; stride < ControlPoints.size(); stride+=3){
        geoMatrix = ControlPoints.getMatrix4(stride);
        for(int i = 0; i < SubdivisionSegment+1; ++i){
            double t = i * (1.0/(double)SubdivisionSegment);
            double power[4] = {1, t, t*t, t*t*t};
            PowerBasis = Matrix(power, 4, 1);
            Matrix temp = geoMatrix * bezierMatrix * PowerBasis;
            curve.pushVec3(Vector3(temp.getVector(0)));
        }
    }
    return curve;
}
Geometry Curve::CreateBSplineCurve()
{
    Geometry curve;
    Geometry extraKnot;
    Matrix geoMatrix;
    Matrix bSplineMatrix = Matrix(BSPLINE, 4, 4);
    if(ControlPoints.size()!=0){
        for(int i = 0; i < 3; ++i)
            extraKnot.pushVec3(ControlPoints.getData()[0]);
        extraKnot = extraKnot + ControlPoints;
    }
    for(int stride = 0; stride < extraKnot.size(); ++stride){
        geoMatrix = extraKnot.getMatrix4(stride);
        if(stride)
        for(int i = 0; i < SubdivisionSegment+1; ++i){
            double t = i * (1.0/(double)SubdivisionSegment);
            double power[4] = {1, t, t*t, t*t*t};
            PowerBasis = Matrix(power, 4, 1);
            Matrix temp = geoMatrix * bSplineMatrix * PowerBasis;
            curve.pushVec3(Vector3(temp.getVector(0)));
        }
    }
    return curve;
}

void Curve::increaseSubdivision ()
{
    if(SubdivisionSegment < 50){
        SubdivisionSegment++;
        CreateCurve();
    }
}
void Curve::decreaseSubdivision ()
{
    if(SubdivisionSegment > 1){
        SubdivisionSegment--;
        CreateCurve();
    }
}
void Curve::switchCurve()
{
    BezierBSpline = !BezierBSpline;
    CreateCurve();
}

/*
 *  Control Points
 */
void Curve::drag (int index, const Vector3& position)
{
    ControlPoints[index] = position;
    CreatePolyline();
    CreateCurve();
}
int Curve::getHoverControlPointIndex (double xpos, double ypos)
{
    Vector3 cursorPos = {xpos, ypos, 0};
    for(int i = 0; i < ControlPoints.size(); i++){
        if(Vector3::distance(cursorPos, ControlPoints[i]) < controlRadius)
            return i;
    }
    return -1;
}
void Curve::appendControlPoint (const Vector3& other)
{
    ControlPoints.pushVec3(other);
    CreatePolyline();
    CreateCurve();
}
void Curve::removeControlPoint ()
{
    ControlPoints.popVec3();
    CreatePolyline();
    CreateCurve();
}

/*
 *  Draw
 */
void Curve::draw ()
{
    // Draw Polyline
    if(showPolyline)
        drawPolyline();
    // Draw Curve
    glUniform1i(glGetUniformLocation(Shader, "isPolyline"), 0);
    glBindVertexArray(CurveVAO);
    glDrawArrays(GL_LINE_STRIP, 0, CurvePoints.size());
}
void Curve::drawPolyline ()
{
    glUniform1i(glGetUniformLocation(Shader, "isPolyline"), 1);
    // Draw Polyline
    glBindVertexArray(PolylineVAO);
    glDrawArrays(GL_LINE_STRIP, 0, ControlPoints.size());
    glPointSize(7.5);
    glDrawArrays(GL_POINTS, 0, ControlPoints.size());
}
