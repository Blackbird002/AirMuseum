#include "CSCIx229.h"

void findDispVector(double x1, double y1, double z1, double x2, double y2, double z2, double* vX, double* vY, double* vZ){
    *vX = x2 - x1;
    *vY = y2 - y1;
    *vZ = z2 - z1;
}

void findNormalVector(double x1, double y1, double z1, double x2, double y2, double z2, double* uX, double* uY, double* uZ){
    double crossX = (y1*z2) - (z1*y2);
    double crossY = (z1*x2) - (x1*z2);
    double crossZ = (x1*y2) - (y1*x2);

    //Normalize the vector(finds the unit vector of the normal)
    double mag = sqrt(crossX*crossX + crossY*crossY + crossZ*crossZ);
    *uX = crossX /= mag;
    *uY = crossY /= mag;
    *uZ = crossZ /= mag;
}

/*
 *  Draw vertex in polar coordinates with normal and texture
 */
void Vertex(int th,int ph)
{
    double x = Cos(th)*Cos(ph);
    double y = Sin(th)*Cos(ph);
    double z =         Sin(ph);
    glNormal3d(x,y,z);
    glTexCoord2d(th/360.0,ph/180.0+0.5);
    glVertex3d(x,y,z);
}