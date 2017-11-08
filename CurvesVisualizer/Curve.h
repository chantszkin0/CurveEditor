//
//  Curve.h
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 26/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#ifndef Curve_h
#define Curve_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif

#include <math.h>
#include "Matrix.h"
#include "Geometry.h"

//  Matrices
const double BEZIER[16] = {1, -3, 3, -1, 0, 3, -6, 3, 0, 0, 3, -3, 0, 0, 0, 1};
const double BSPLINE[16] = {1.0/6.0, -3.0/6.0, 3.0/6.0, -1.0/6.0, 4.0/6.0, 0, -6.0/6.0, 3.0/6.0, 1.0/6.0, 3.0/6.0, 3.0/6.0, -3.0/6.0, 0, 0, 0, 1.0/6.0};

class Curve
{
public:
    bool showPolyline = true;
    int dragingIndex = -1;
    
    Curve(unsigned int shaderProgram);
    ~Curve();
    
    void CreateCurve ();
    void CreatePolyline ();
    Geometry CreateBezierCurve ();
    Geometry CreateBSplineCurve ();
    
    void increaseSubdivision ();
    void decreaseSubdivision ();
    void switchCurve();
    
    /// Control Points
    void drag (int index, const Vector3& position);
    int getHoverControlPointIndex (double xpos, double ypos);
    void appendControlPoint (const Vector3& other);
    void removeControlPoint ();
    
    /// Draw
    void draw ();
    void drawPolyline ();
    
private:
    unsigned int Shader;
    unsigned int PolylineVAO, PolylineVBO;
    unsigned int CurveVAO, CurveVBO;
    
    ///  Curve Type (FALSE = Bezier, TRUE = B-Spline)
    bool BezierBSpline;

    ///  Edit ControlPoints
    float controlRadius = 0.05;
    
    ///  Points
    int SubdivisionSegment = 5;
    
    ///  Matrices
    Geometry ControlPoints;
    Geometry CurvePoints;
    Matrix SplineBasis;
    Matrix PowerBasis;
    
};

#endif /* Curve_h */
