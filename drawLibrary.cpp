#include "CSCIx229.h"

// ----------------------------------------------------------
// disk
// ----------------------------------------------------------

/*
 *  Draw a disk at (x,y,z) radius r
 *  The resolution is fixed at 36 slices (10 degrees each)
 */
void disk(double x,double y,double z,double r, double th)
{
  glEnable(GL_TEXTURE_2D);
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
   glRotated(th,0,1,0);
  glScaled(r,r,r);
  //  Head & Tail
  glColor3f(1,1,1);
  glNormal3f(0,0,-1);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.5,0.5);
  glVertex3f(0,0,1);
  for(int k=0;k<=360;k+=10){
    glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
    glVertex3f(Cos(k),Sin(k),1);
  }
  glEnd();
  //  Undo transformations
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a Sphere
 *     at (x,y,z)
 *     radius (r)
 */
void Sphere(double x,double y,double z,double r, float emission, float shiny)
{
  const int d=10;
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0f,0.0f,0.01f*emission,1.0f};

  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
  glScaled(r,r,r);

  glMaterialf(GL_FRONT,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
  glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

  //  Latitude bands
  for (ph=-90;ph<90;ph+=d)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=d)
    {
      Vertex(th,ph);
      glTexCoord2f(th/360.0,ph/90.0);
      Vertex(th,ph+d);
    }
    glEnd();
  }

  //  Undo transformations
  glPopMatrix();
}

void drawAxisLines(){
  // X Axis line
  glBegin(GL_LINES);
  glColor3f(1, 0, 0); //Red
  glVertex3f(0, 0, 0);
  glVertex3f(25, 0, 0);

  // -X Line
  glVertex3f(0, 0, 0);
  glVertex3f(-25, 0, 0);

  // Y Axis line
  glColor3f(0, 1, 0); //Green
  glVertex3f(0, 0, 0);
  glVertex3f(0, 25, 0);

  // -Y line
  glVertex3f(0, 0, 0);
  glVertex3f(0, -25, 0);

  // Z Axis line
  glColor3f(0, 0, 1); //Blue
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 25);

  // -Z line
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, -25);

  glEnd();
}

void drawAxisLabels(){
  glColor3f(1, 1, 1); //White
  glRasterPos3d(25.05,0,0);
  Print("X");
  glRasterPos3d(-25.05,0,0);
  Print("-X");
  glRasterPos3d(0,25.05,0);
  Print("Y");
  glRasterPos3d(0,-25.05,0);
  Print("-Y");
  glRasterPos3d(0,0,25.05);
  Print("Z");
  glRasterPos3d(0,0,-25.05);
  Print("-Z");
}

/*
 *  Draw the engine on fighter jet
 *     at (x,y,z)
 *     radius (r)
 */
void engineSphere(double x,double y,double z,double r, double yRot, float emission, float shiny)
{
  const int d=10;
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0f,0.0f,0.01f*emission,1.0f};

  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
  glScaled(r,r,r);
  glRotated(yRot,0,1,0);

  glMaterialf(GL_FRONT,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
  glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

  //  Latitude bands
  for (ph=0;ph<50;ph+=d)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=d)
    {
      Vertex(th,ph);
      glTexCoord2f(th/360.0,ph/90.0);
      Vertex(th,ph+d);
    }
    glEnd();
  }

  //  Undo transformations
  glPopMatrix();
}

// ----------------------------------------------------------
// Skybox Cube
// ----------------------------------------------------------

/*
 *  Draw the skybox cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
void skyboxCube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, float emission, float shiny, unsigned int texture[])
{
  int mode = 0;
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0f,0.0f,0.01f*emission,1.0f};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);
  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glColor3f(1,1,1);

  //  Front
  glBindTexture(GL_TEXTURE_2D,texture[5]);
  glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
    glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
    glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
    glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
  glEnd();

  //  Back
  glBindTexture(GL_TEXTURE_2D,texture[10]);
  glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
  glEnd();

  //  Right
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
    glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
  glEnd();

  //  Left
  glBindTexture(GL_TEXTURE_2D,texture[6]);
  glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
    glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
    glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
  glEnd();

  //  Top
  glBindTexture(GL_TEXTURE_2D,texture[8]);
  glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
    glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
    glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
  glEnd();

  //  Bottom
  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
    glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
  glEnd();
  //  Undo transformations and textures
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a ballEngineSphere
 *     at (x,y,z)
 *     radius (r)
 */
void ball(double x,double y,double z,double r, float emission, float shiny, int inc)
{
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0f,0.0f,0.01f*emission,1.0f};
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glScaled(r,r,r);
  //  White ball
  glColor3f(1,1,1);
  glMaterialf(GL_FRONT,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
  glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
  //  Bands of latitude
  for (ph=-90;ph<90;ph+=inc)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=2*inc)
    {
        Vertex(th,ph);
        Vertex(th,ph+inc);
    }
    glEnd();
  }
  //  Undo transofrmations
  glPopMatrix();
}

void hangarFloorSquare(double x, double z){
  
  glColor3d(1,0,0);
  //  Save transformation
  glPushMatrix();
  glTranslated(x-3.5,0,z-3.5);

  glBegin(GL_POLYGON);
    glVertex3d(0,0,0);
    glVertex3d(0,0,7);
    glVertex3d(7,0,7);
    glVertex3d(7,0,0);
  glEnd();

  //  Undo transofrmations
  glPopMatrix();
}

void hangarFloor(double x, double z){
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);

  glColor3d(0.1,0.1,0.1);
  //  Save transformation
  glPushMatrix();
  glTranslated(x,0,z);

  glBegin(GL_POLYGON);
    glVertex3d(0,0,0);
    glVertex3d(0,0,35);
    glVertex3d(50,0,35);
    glVertex3d(50,0,0);
  glEnd();

  //  Undo transofrmations
  glPopMatrix();
  //  Disable polygon offset
  glDisable(GL_POLYGON_OFFSET_FILL);
}

void hangarFloorRectangle(double x, double z){
  glColor3d(1,0,0);
  //  Save transformation
  glPushMatrix();
  glTranslated(x-4.0,0,z-3.0);

  glBegin(GL_POLYGON);
    glVertex3d(0,0,0);
    glVertex3d(0,0,6);
    glVertex3d(8,0,6);
    glVertex3d(8,0,0);
  glEnd();

  //  Undo transofrmations
  glPopMatrix();
}

void centerRect(double x, double z){
  glColor3d(1,0,0);
  //  Save transformation
  glPushMatrix();
  glTranslated(x-4.5,0,z-5.0);

  glBegin(GL_POLYGON);
    glVertex3d(0,0,0);
    glVertex3d(0,0,9);
    glVertex3d(10,0,9);
    glVertex3d(10,0,0);
  glEnd();

  //  Undo transofrmations
  glPopMatrix();
}

void hangarLengthWall(double x, double y, double z){
   glColor3d(1,1,1);
  //  Save transformation
  glPushMatrix();
  glTranslated(x-25,y,z);

  glBegin(GL_POLYGON);
    glVertex3d(0,0,0);
    glVertex3d(50,0,0);
    glVertex3d(50,10,0);
    glVertex3d(0,10,0);
  glEnd();

  //  Undo transofrmations
  glPopMatrix();
}

void hangarWidthWall(double x, double y, double z){
  glColor3d(0.5,0.5,0.5);
  //  Save transformation
  glPushMatrix();
  glTranslated(x,y,z-17.5);

  glBegin(GL_POLYGON);
    glVertex3d(0,0,0);
    glVertex3d(0,0,35);
    glVertex3d(0,10,35);
    glVertex3d(0,10,0);
  glEnd();
  //  Undo transofrmations
  glPopMatrix();
}

// ----------------------------------------------------------
// Aircraft Hangar Roof top portion
// ----------------------------------------------------------

/*
 *  Draw the hangar roof
 *  R - scale
 *  th - rotation about y axis
 */
void hangarRoof(double x,double y,double z,double r, double th)
{
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(r,r,r);

  glColor3f(0.25,0.25,0.25);

  //Rear section of roof
  glColor3f(0.1,0.1,0.1);
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0,0,0);
    for(int k=0;k<=180;k+=10){
      glVertex3f(25*Cos(k),7*Sin(k),0);
    }
  glEnd();

  //Front section of roof
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0,0,35);
    for(int k=0;k<=180;k+=10){
      glVertex3f(25*Cos(k),7*Sin(k),35);
    }
  glEnd();

  //  Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);

  glBegin(GL_QUAD_STRIP);
    for(int k=0;k<=180;k+=10){
      glVertex3f(25*Cos(k),7*Sin(k),0);
      glVertex3f(25*Cos(k),7*Sin(k),35);
    }
  glEnd();
  //Disable blending
  glDisable(GL_BLEND);
  
  //  Undo transformations
  glPopMatrix();
}

void drawHangar(double x, double y, double z, double r){
  //  Save transformation
  glPushMatrix();
  glScaled(r,r,r);
  glTranslated(x-25,y,z-17.5);

  hangarFloorSquare(7.5,7.5);
  hangarFloorSquare(7.5,27.5);
  hangarFloorSquare(42.5,7.5);
  hangarFloorSquare(42.5,27.5);
  hangarFloor(0,0);
  hangarFloorRectangle(21,6);
  hangarFloorRectangle(29,6);
  hangarFloorRectangle(21,29);
  hangarFloorRectangle(29,29);
  centerRect(20,17.5);
  centerRect(30,17.5);
  hangarLengthWall(25,0,0);
  hangarLengthWall(25,0,35);
  hangarWidthWall(0, 0, 17.5);
  hangarWidthWall(50, 0, 17.5);
  hangarRoof(25,10,0,1,0);

  //  Undo transofrmations
  glPopMatrix();
}

