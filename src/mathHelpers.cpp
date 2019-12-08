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

 /* Multiply the current ModelView-Matrix with a shadow-projetion matrix.
 *
 * L is the position of the light source
 * E is a point within the plane on which the shadow is to be projected.  
 * N is the normal vector of the plane.
 *
 * Everything that is drawn after this call is "squashed" down to the plane.
 */
void ShadowProjection(float L[4], float E[4], float N[4])
{
   float mat[16];
   float e = E[0]*N[0] + E[1]*N[1] + E[2]*N[2];
   float l = L[0]*N[0] + L[1]*N[1] + L[2]*N[2];
   float c = e - l;
   //  Create the matrix.
   mat[0] = N[0]*L[0]+c; mat[4] = N[1]*L[0];   mat[8]  = N[2]*L[0];   mat[12] = -e*L[0];
   mat[1] = N[0]*L[1];   mat[5] = N[1]*L[1]+c; mat[9]  = N[2]*L[1];   mat[13] = -e*L[1];
   mat[2] = N[0]*L[2];   mat[6] = N[1]*L[2];   mat[10] = N[2]*L[2]+c; mat[14] = -e*L[2];
   mat[3] = N[0];        mat[7] = N[1];        mat[11] = N[2];        mat[15] = -l;
   //  Multiply modelview matrix
   glMultMatrixf(mat);
}