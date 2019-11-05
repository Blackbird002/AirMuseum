/*
 *  Set projection
 */
#include "CSCIx229.h"

void Project(double fov, double asp, double dim, int projectionMode)
{
  //  Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Undo previous transformations
  glLoadIdentity();

  if(projectionMode == 1){
    //  First person
    gluPerspective(fov,asp,dim/15,10*dim);
  }if (projectionMode == 2){
    //  Perspective transformation
    gluPerspective(fov,asp,dim/20,4*dim);
  }else if (projectionMode == 3){
  }
  
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();
}
