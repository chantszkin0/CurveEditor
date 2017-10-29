//
//  Curve.h
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 26/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#ifndef Curve_h
#define Curve_h

#include <math.h>

class Curve
{
public:
    bool showPolyline = true;
    int dragingIndex = -1;
    Curve(unsigned int shaderProgram)
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
    ~Curve()
    {
        //delete[] curvePoints;
    }
    void CreateCurve ()
    {
        curvePointCount = 0;
        for(int i = 0; i < controlPointCount; i+=3){
            //  Append regular cubic bezier curve
            int p0i = (i > controlPointCount)? controlPointCount:i;
            int p1i = (i+1 > controlPointCount)? controlPointCount:i+1;
            int p2i = (i+2 > controlPointCount)? controlPointCount:i+2;
            int p3i = (i+3 > controlPointCount)? controlPointCount:i+3;
            if(i == 0 && controlPointCount < 4){
                std::cout << "First\n";
                CreateBezierPath(controlPoints[p0i], controlPoints[--p1i], controlPoints[--p2i], controlPoints[--p3i]);
            }else if(i == 3 && controlPointCount == 4){
                std::cout << "4\n";
                CreateBezierPath(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
            }else if(controlPointCount > 4){
                std::cout << "Others\n";
                p0i = (p0i == controlPointCount)? p0i-1:p0i;
                p1i = (p1i == controlPointCount)? p1i-1:p1i;
                p2i = (p2i == controlPointCount)? p2i-1:p2i;
                p3i = (p3i == controlPointCount)? p3i-1:p3i;
                CreateBezierPath(controlPoints[p0i], controlPoints[p1i], controlPoints[p2i], controlPoints[p3i]);
            }
            std::cout << p0i << p1i << p2i << p3i << std::endl;
        }
//        for(int i = 0; i < curvePointCount; i++)
//            std::cout << curvePoints[i];
        //std::cout << curvePointCount;
        
        glBindVertexArray(CurveVAO);
        glBindBuffer(GL_ARRAY_BUFFER, CurveVBO);
        
        glBufferData(GL_ARRAY_BUFFER, curvePointCount*sizeof(Vector3), curvePoints, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vector3), (void*)0);
    }
    void CreatePolyline ()
    {
        // Create Polyline
        glBindVertexArray(PolylineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, PolylineVBO);
        
        glBufferData(GL_ARRAY_BUFFER, controlPointCount*sizeof(Vector3), controlPoints, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vector3), (void*)0);
    }
    void CreateBezierPath (Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
    {
        if(SubdivisionSegment == 0)
            return;
        double sub = 1.0/(double)SubdivisionSegment;
        for(int i = 0; i < SubdivisionSegment+1; ++i){
            double t = i * sub;
            Vector3 temp = ((1-t)*(1-t)*(1-t)*p0 + 3*(1-t)*(1-t)*t*p1 + 3*(1-t)*t*t*p2 + t*t*t*p3);
            curvePoints[curvePointCount++] = temp;
        }
    }
    void increaseSubdivision ()
    {
        if(SubdivisionSegment < 50){
            SubdivisionSegment++;
            CreateCurve();
        }
    }
    void decreaseSubdivision ()
    {
        if(SubdivisionSegment > 1){
            SubdivisionSegment--;
            CreateCurve();
        }
    }
    
    /// Control Points
    void drag (int index, const Vector3& position)
    {
        controlPoints[index] = position;
        CreatePolyline();
        CreateCurve();
    }
    int getHoverControlPointIndex (double xpos, double ypos)
    {
        for(int i = 0; i < controlPointCount; i++){
            double xx = (xpos-controlPoints[i].getx())*(xpos-controlPoints[i].getx());
            double yy = (ypos-controlPoints[i].gety())*(ypos-controlPoints[i].gety());
            double distance = sqrt(xx+yy);
            if(distance < controlRadius)
                return i;
        }
        return -1;
    }
    void appendControlPoint (const Vector3& other)
    {
        if(controlPointCount+1 > 1024)
            return;
        controlPoints[controlPointCount++] = other;
        CreatePolyline();
        CreateCurve();
    }
    void removeControlPoint ()
    {
        if(controlPointCount-1 < 1)
            return;
        controlPointCount--;
        CreatePolyline();
        CreateCurve();
    }
    
    /// Draw
    void draw ()
    {
        // Draw Polyline
        if(showPolyline)
            drawPolyline();
        // Draw Curve
        glUniform1i(glGetUniformLocation(Shader, "isPolyline"), 0);
        glBindVertexArray(CurveVAO);
        glDrawArrays(GL_LINE_STRIP, 0, curvePointCount);
    }
    void drawPolyline ()
    {
        glUniform1i(glGetUniformLocation(Shader, "isPolyline"), 1);
        // Draw Polyline
        glBindVertexArray(PolylineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, controlPointCount);
        glPointSize(7.5);
        glDrawArrays(GL_POINTS, 0, controlPointCount);
    }
    
private:
    unsigned int Shader;
    unsigned int PolylineVAO, PolylineVBO;
    unsigned int CurveVAO, CurveVBO;

    //  Edit ControlPoints
    float controlRadius = 0.05;
    
    //  Points
    int controlPointCount = 0;
    Vector3 controlPoints[1024];
    int curvePointCount;
    int SubdivisionSegment = 10;
    Vector3 curvePoints[10240];
};


#endif /* Curve_h */
