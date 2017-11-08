//
//  Curve.cpp
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 8/11/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#include "Curve.h"
#include "Shader.h"

Curve::Curve() : BezierBSpline(true), showPolyline(true)
{
    Shader = loadShader();
    AllocBuffers();
}
Curve::Curve(double* arr, int n) : BezierBSpline(true), showPolyline(true)
{
    Shader = loadShader();
    AllocBuffers();
    ControlPoints = Geometry(arr, n);
    UpdatePolylines();
    UpdateCurves();
}
Curve::~Curve()
{

}

void Curve::AllocBuffers ()
{
    // Allocate Polyline Buffer & Create
    glGenVertexArrays(1, &PolylineVAO);
    glGenBuffers(1, &PolylineVBO);
    // Allocate Curve Buffer & Create
    glGenVertexArrays(1, &CurveVAO);
    glGenBuffers(1, &CurveVBO);
}

void Curve::UpdateCurves ()
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
void Curve::UpdatePolylines ()
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
        geoMatrix = ControlPoints.getMatrix4(stride, closeLoop);
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
    if(ControlPoints.size()!=0 && !closeLoop){
        for(int i = 0; i < 3; ++i)
            extraKnot.pushVec3(ControlPoints.getData()[0]);
        extraKnot = extraKnot + ControlPoints;
    }else{
        extraKnot = ControlPoints;
    }
    for(int stride = 0; stride < extraKnot.size(); ++stride){
        geoMatrix = extraKnot.getMatrix4(stride, closeLoop);
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
        UpdateCurves();
    }
}
void Curve::decreaseSubdivision ()
{
    if(SubdivisionSegment > 1){
        SubdivisionSegment--;
        UpdateCurves();
    }
}
void Curve::togglePolyline()
{
    showPolyline = !showPolyline;
}
void Curve::toggleLoop()
{
    closeLoop = !closeLoop;
    if(closeLoop)
        ControlPoints.pushVec3(ControlPoints.getFirst());
    else
        ControlPoints.popVec3();
    UpdatePolylines();
    UpdateCurves();
}
void Curve::switchCurve()
{
    BezierBSpline = !BezierBSpline;
    UpdateCurves();
}

/*
 *  Control Points
 */
void Curve::drag (int index, const Vector3& position)
{
    if(closeLoop)
        ControlPoints[ControlPoints.size()-1] = ControlPoints.getFirst();
    ControlPoints[index] = position;
    UpdatePolylines();
    UpdateCurves();
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
    if(closeLoop){
        std::cout << "Could not add control points, since it is a closed loop!\n";
        return;
    }
    ControlPoints.pushVec3(other);
    UpdatePolylines();
    UpdateCurves();
}
void Curve::removeControlPoint ()
{
    if(closeLoop){
        std::cout << "Could not remove control points, since it is a closed loop!\n";
        return;
    }
    ControlPoints.popVec3();
    UpdatePolylines();
    UpdateCurves();
}

/*
 *  Draw
 */
void Curve::draw ()
{
    // Use Shader Progarm
    glUseProgram(Shader);
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
