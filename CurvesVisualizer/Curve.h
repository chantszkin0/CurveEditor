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
    int dragingIndex = -1;
    
    /// Constructors and Destrctors
    Curve();
    Curve(double*, int);
    ~Curve();
    
    /// VAO and VBO Allocation
    void AllocBuffers ();
    
    /// Buffer Updates
    void UpdateCurves ();
    void UpdatePolylines ();
    Geometry CreateBezierCurve ();
    Geometry CreateBSplineCurve ();
    
    /// Input Changes
    void increaseSubdivision ();
    void decreaseSubdivision ();
    void togglePolyline();
    void toggleLoop();
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
    
    ///  Input Controls
    bool BezierBSpline;             //  Curve Type (FALSE = Bezier, TRUE = B-Spline)
    bool showPolyline;              //  Polyline   (FALSE = hide,   TRUE = show)
    bool closeLoop;                 //  Loop       (FALSE = open,   TRUE = close)

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
