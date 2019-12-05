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
  glEnable(GL_TEXTURE_2D);
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

/*
 *  Draw landing gear with one tire
 *     at (x,y,z)
 *     h is height going down from <x,y,z>
 *     s is the scale
 */
void drawLandingGearOneTire(double x,double y,double z, double h, double s){
  glDisable (GL_LIGHTING);
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glScaled(s,s,s);
  
  planeTire(0,0-h,0,1);
  glLineWidth(3);
  glColor3d(0.5,0.5,0.5);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0-h, 0);
  glEnd();

  glLineWidth(1);

  //  Undo transformations
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/*
 *  Draw angeled landing gear with one tire
 *     at (x,y,z)
 *     h is height going down from <x,y,z>
 *     s is the scale
 */
void drawLandingGearOneTireAngled(double x,double y,double z, double h, double s, double zOffset){
  glDisable (GL_LIGHTING);
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  glScaled(s,s,s);
  
  planeTire(0,0-h,zOffset,1);
  glLineWidth(3);
  glColor3d(0.5,0.5,0.5);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0-h, zOffset);
  glEnd();

  glLineWidth(1);

  //  Undo transformations
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/*
 *  Draw a cylinder
 *  at (x,y,z)
 *  h is height
 *  r is radius
 *  s is scale
 */
void Cylinder(float x,float y,float z ,float thx,float thy ,float r,float h)
{
  int i,j;   // Counters
  int N=4*8; // Number of slices

  //  Transform
  glPushMatrix();
  glTranslated(x,y+h,z);
  glRotated(thx,1,0,0);
  glRotated(thy,0,1,0);
  glScaled(r,r,h);

  // wo end caps (fan of triangles)
  for (j=-1;j<=1;j+=2)
  {
    glNormal3d(0,0,j); 
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2d(0,0); glVertex3d(0,0,j);
    for (i=0;i<=N;i++)
    {
      float th = j*i*360.0/N;
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(Cos(th),Sin(th),j);
    }
    glEnd();
  }

  //Cylinder Body (strip of quads)
  glBegin(GL_QUADS);
  for (i=0;i<N;i++)
  {
    float th0 =  i   *360.0/N;
    float th1 = (i+1)*360.0/N;
    glNormal3d(Cos(th0),Sin(th0),0); glTexCoord2d(0,th0/90.0); glVertex3d(Cos(th0),Sin(th0),+1);
    glNormal3d(Cos(th0),Sin(th0),0); glTexCoord2d(2,th0/90.0); glVertex3d(Cos(th0),Sin(th0),-1);
    glNormal3d(Cos(th1),Sin(th1),0); glTexCoord2d(2,th1/90.0); glVertex3d(Cos(th1),Sin(th1),-1);
    glNormal3d(Cos(th1),Sin(th1),0); glTexCoord2d(0,th1/90.0); glVertex3d(Cos(th1),Sin(th1),+1);
  }
  glEnd();

  //Restore
  glPopMatrix();
}

void drawBoundaryPolesSquare(double x,double y,double z, double h){
  glPushMatrix();
  //  Offset
  glTranslated(x-3.5,y,z-3.5);
  glColor3d(1,1,1);
  Cylinder(0,0,0,90,0,0.02,0.4);
  Sphere(0,0.8,0,0.08,0,1);
  Cylinder(0,0,7,90,0,0.02,0.4);
  Sphere(0,0.8,7,0.08,0,1);
  Cylinder(7,0,7,90,0,0.02,0.4);
  Sphere(7,0.8,7,0.08,0,1);
  Cylinder(7,0,0,90,0,0.02,0.4);
  Sphere(7,0.8,0,0.08,0,1);
  
  //Restore
  glPopMatrix();
}

void drawBoundaryPolesRectangle(double x,double y,double z, double h){
  glPushMatrix();
  //  Offset
  glTranslated(x-8.0,y,z-3.0);
  glColor3d(1,1,1);
  Cylinder(0,0,0,90,0,0.02,0.4);
  Sphere(0,0.8,0,0.08,0,1);
  Cylinder(0,0,6,90,0,0.02,0.4);
  Sphere(0,0.8,6,0.08,0,1);
  Cylinder(16,0,6,90,0,0.02,0.4);
  Sphere(16,0.8,6,0.08,0,1);
  Cylinder(16,0,0,90,0,0.02,0.4);
  Sphere(16,0.8,0,0.08,0,1);
  
  //Restore
  glPopMatrix();
}

void drawBoundaryPolesCenter(double x,double y,double z, double h){
  glPushMatrix();
  //  Offset
  glTranslated(x-9.5,y,z-5.0);
  glColor3d(1,1,1);

  Cylinder(0,0,0,90,0,0.02,0.4);
  Sphere(0,0.8,0,0.08,0,1);

  Cylinder(0,0,9,90,0,0.02,0.4);
  Sphere(0,0.8,9,0.08,0,1);

  Cylinder(20,0,9,90,0,0.02,0.4);
  Sphere(20,0.8,9,0.08,0,1);

  Cylinder(20,0,0,90,0,0.02,0.4);
  Sphere(20,0.8,0,0.08,0,1);
  
  //Restore
  glPopMatrix();
}

