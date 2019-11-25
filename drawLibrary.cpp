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

  /*
  //  Bottom
  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
    glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
  glEnd();
  */
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

/*
 *  Draw a plane tire
 *     at (x,y,z)
 *     radius (r)
 *     width w
 *     rotation about
 */
void planeTire(double x,double y,double z, double r){
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslated(x,y,z-0.25);
    glRotated(0,0,1,0);
  glScaled(r,r,r);
  glColor3f(0,0,0);

  glNormal3f(0,0,-1);
  glBegin(GL_TRIANGLE_FAN);
    //glTexCoord2f(0.5,0.5);
    glVertex3f(0,0,0);
    for(int k=0;k<=360;k+=10){
      //glTexCoord2d(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
      glVertex3f(Cos(k),Sin(k),0);
    }
  glEnd();

  glNormal3f(0,0,1);
  glBegin(GL_TRIANGLE_FAN);
  //glTexCoord2f(0.5,0.5);
  glVertex3f(0,0,0.5);
  for(int k=0;k<=360;k+=10){
    //glTexCoord2d(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
    glVertex3f(Cos(k),Sin(k),0.5);
  }
  glEnd();

  glBegin(GL_QUAD_STRIP);
    for(int k=0;k<=360;k+=10){
      //glTexCoord2d(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
      glVertex3f(Cos(k),Sin(k),0);
      //glTexCoord2d(Cos(k),Sin(k));
      glVertex3f(Cos(k),Sin(k),0.5);
    }
  glEnd();

  //  Undo transformations
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw landing gear
 *     at (x,y,z)
 *     h is height going down from <x,y,z>
 *     s is the scale
 */
void drawLandingGear(double x,double y,double z, double h, double s){
  glDisable (GL_LIGHTING);
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glScaled(s,s,s);
  
  planeTire(0,0-h,0+0.5,1);
  glLineWidth(4);
  glColor3d(0.5,0.5,0.5);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0-h, 0);
    glVertex3f(0,0-h,0.5);
    glVertex3f(0,0-h,-0.5);
  glEnd();
  glLineWidth(1);
  planeTire(0,0-h,0-0.5,1);

  //  Undo transformations
  glPopMatrix();
  glEnable(GL_LIGHTING);
}