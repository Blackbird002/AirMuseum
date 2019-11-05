#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include "CSCIx229.h"
#include <iostream>
// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------

// What the camera is looking at vector
double cameraLookX , cameraLookY , cameraLookZ;

// XZ position of the camera in 1st person (eye)
double cameraX=50, cameraY =10 , cameraZ=25;

int th=321;         //  Azimuth of view angle (y)
int ph=29;         //  Elevation of view angle (x)
double dim=150;   // Dimension of orthogonal box
int currentScene = 1;
bool drawAxis = false;
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio

/*
  1 - First person 
  2 - Perspective (starts in perspective mode)
*/
int projectionMode = 1;     

double THX;
double THZ;

// Light values
int light     =   0;  //  Lighting
int one       =   1;  // Unit value
int distance  =  70; // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  25;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
int move      =   1;  //  Move light
unsigned int texture[12]; // Texture names

//Texture settings
int mode=0;       //  Texture mode

double previousMouseY;

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void reshape(int,int);
void idle();
void drawAxisLines();
void drawAxisLabels();
void init();
void strafeLeft(double bX, double bY, double bZ, double moveUnits);
void strafeRight(double bX, double bY, double bZ, double moveUnits);
void moveForward(double bX, double bY, double bZ, double moveUnits);
void moveBackward(double bX, double bY, double bZ, double moveUnits);
void mouseCallback(GLFWwindow *window, double x, double y);
void findDispVector(double x1, double y1, double z1, double x2, double y2, double z2, double* vX, double* vY, double* vZ);
void findNormalVector(double x1, double y1, double z1, double x2, double y2, double z2, double* uX, double* uY, double* uZ);
void key(GLFWwindow* window,int key,int scancode,int action,int mods);
void error(int error, const char* text);
static void ball(double x,double y,double z,double r);
static void Sphere(double x,double y,double z,double r);
static void engineSphere(double x,double y,double z,double r,double yRot);
static void FighterJet(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, double scale, double thx, double thz);
static void XB70Bomber(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, double scale, double thx, double thz);
static void skyboxCube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th);
static void disk(double x,double y,double z,double r, double th);

/*
 * GLFW errors
 */
void error(int error, const char* text)
{
    fprintf(stderr,"Error: %s\n",text);
}

// ----------------------------------------------------------
// key() Callback function
// ----------------------------------------------------------
void key(GLFWwindow* window,int key,int scancode,int action,int mods){
  //  Discard key releases (keeps PRESS and REPEAT)
  if (action==GLFW_RELEASE) return;

  //  Check for shift
  int shift = (mods & GLFW_MOD_SHIFT);

  //  Exit on ESC
  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(window,1);
  else if (key==GLFW_KEY_RIGHT)
    th += 2.5;
  else if (key==GLFW_KEY_LEFT)
    th -= 2.5;
  //  Increase/decrease elevation
  else if (key==GLFW_KEY_UP)
    ph += 2.5;
  else if (key==GLFW_KEY_DOWN)
    ph -= 2.5;

  //Moving camera only in First person & FOV
  if (projectionMode == 1){
    if(key == GLFW_KEY_W)
      moveForward(cameraLookX, cameraLookY, cameraLookZ, 2);
    else if(key == GLFW_KEY_S)
      moveBackward(cameraLookX, cameraLookY, cameraLookZ, 2);  
    else if(key == GLFW_KEY_A)
      strafeLeft(cameraLookX, cameraLookY, cameraLookZ, 2);
    else if(key == GLFW_KEY_D)
      strafeRight(cameraLookX, cameraLookY, cameraLookZ, 2);
      //  Change field of view angle
    else if (key == GLFW_KEY_Z){
        fov--;   
    }else if (key == GLFW_KEY_X){
        fov++;
    }
  }
  
  // Reproject
  Project(fov, asp, dim, projectionMode);
}

void mouseCallback(GLFWwindow *window, double x, double y)
{
  double deltaY = previousMouseY - y;
  //std::cout<<"Mouse X: " <<x <<" " <<"Mouse Y: " <<y <<std::endl;

  if(ph < 90 && (deltaY > 0))
    ph += 1;

  if(ph > -90 && (deltaY) < 0)
    ph -= 1;

  th = x;

  previousMouseY = y;
  // Reproject
  Project(fov, asp, dim, projectionMode);
}	

// ----------------------------------------------------------
// disk
// ----------------------------------------------------------

/*
 *  Draw a disk at (x,y,z) radius r
 *  The resolution is fixed at 36 slices (10 degrees each)
 */
static void disk(double x,double y,double z,double r, double th)
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


// ----------------------------------------------------------
// Skybox Cube
// ----------------------------------------------------------

/*
 *  Draw the skybox cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void skyboxCube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
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

void strafeLeft(double bX, double bY, double bZ, double moveUnits){
  double aX = 0;
  double aY = 1;
  double aZ = 0;

  //Calculate unit vector of lookAt vector:
  double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
  bX /= mag;
  bY /= mag;
  bZ /= mag;

  //Calculate cross product of vector a and b (a x b):
  double crossX = (aY*bZ) - (aZ*bY);
  //double crossY = (aY*bX) - (aX*bZ);
  double crossZ = (aX*bZ) - (aY*bX);

  //Add cross vector to camera position vector 
  cameraX += moveUnits*crossX;
  //cameraY += moveUnits*crossY;
  cameraZ += moveUnits*crossZ;
}

void strafeRight(double bX, double bY, double bZ, double moveUnits){
  double aX = 0;
  double aY = -1;
  double aZ = 0;

  //Calculate unit vector of lookAt vector:
  double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
  bX /= mag;
  bY /= mag;
  bZ /= mag;

  //Calculate cross product of vector a and b (a x b):
  double crossX = (aY*bZ) - (aZ*bY);
  //double crossY = (aY*bX) - (aX*bZ);
  double crossZ = (aX*bZ) - (aY*bX);

  //Add cross vector to camera position vector 
  cameraX += moveUnits*crossX;
  //cameraY += moveUnits*crossY;
  cameraZ += moveUnits*crossZ;
}

void moveForward(double bX, double bY, double bZ, double moveUnits){

  //Calculate unit vector of lookAt vector:
  double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
  bX /= mag;
  bY /= mag;
  bZ /= mag;

  cameraX += moveUnits*bX;
  cameraZ += moveUnits*bZ;
}

void moveBackward(double bX, double bY, double bZ, double moveUnits){

  //Calculate unit vector of lookAt vector:
  double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
  bX /= mag;
  bY /= mag;
  bZ /= mag;

  cameraX -= moveUnits*bX;
  cameraZ -= moveUnits*bZ;
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(int th,int ph)
{
  double x = Cos(th)*Cos(ph);
  double y = Sin(th)*Cos(ph);
  double z =         Sin(ph);
  glNormal3d(x,y,z);
  glTexCoord2d(th/360.0,ph/180.0+0.5);
  glVertex3d(x,y,z);
}

/*
 *  Draw a ballEngineSphere
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
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
 *  Draw a Sphere
 *     at (x,y,z)
 *     radius (r)
 */
static void Sphere(double x,double y,double z,double r)
{
  const int d=10;
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};

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

/*
 *  Draw a Sphere
 *     at (x,y,z)
 *     radius (r)
 */
static void engineSphere(double x,double y,double z,double r, double yRot)
{
  const int d=10;
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};

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
//  FighterJet
// ----------------------------------------------------------

/*
 *  Draw solid fighter jet
 *    front nose at (x,y,z)
 *    nose points towards (dx,dy,dz)
 *    UP vector (ux,uy,uz) 
 *    Rotation about X - roll (thx)
 *    Rotation about Z - pitch (thz) 
 */
static void FighterJet(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, double scale, double thx, double thz)
{
  //-20
  // Dimensions used to size airplane
  const double wid = 1;   //The "width of the plane's "Fuselage"
  const double shipBowXfront = 0;    //X of front nose
  const double shipBowXend = -4;     //X of end nose
  const double shipSternX = -20;    //X end of ship
  const double cockpitX = -4;        //X center of cockpit
  const double cockpitY = 0.70;

  const double wingXfront = -8;
  const double wingXend = -19;
  const double wingXfrontFold = -17;
  const double wingZ = 10;

  const double canardXend = wingXfront;
  const double canardXfront = shipBowXend;
  const double canardZ = 4;

  const double wingLinefrontX = -16;
  const double wingLineXend = -20;
  const double wingLineZ = wingZ;
  
  //  Unit vector in direction of flght
  double D0 = sqrt(dx*dx+dy*dy+dz*dz);
  double X0 = dx/D0;
  double Y0 = dy/D0;
  double Z0 = dz/D0;

  //  Unit vector in "up" direction
  double D1 = sqrt(ux*ux+uy*uy+uz*uz);
  double X1 = ux/D1;
  double Y1 = uy/D1;
  double Z1 = uz/D1;

  //  Cross product gives the third vector
  double X2 = Y0*Z1-Y1*Z0;
  double Y2 = Z0*X1-Z1*X0;
  double Z2 = X0*Y1-X1*Y0;

  //  Rotation matrix
  double mat[16];
  mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
  mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
  mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
  mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

  //For normal vector calculations
  double aX, aY, aZ;
  double bX, bY, bZ;
  double nX, nY, nZ;

  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

  //  Save current transforms
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glMultMatrixd(mat);
  glRotated(thx,1,0,0);
  glRotated(thz,0,0,1);
  glScaled(scale,scale,scale);

  // ----------------------------------------------------------
  // Front nose (4 sided)
  // ---------------------------------------------------------
  glColor3f(0.8,0.8,0.8);
  findDispVector(shipBowXend,wid,wid,shipBowXfront,0.0,0.0,&aX,&aY,&aZ);
  findDispVector(shipBowXend,wid,wid,shipBowXend,-wid,wid,&bX,&bY,&bZ);
  findNormalVector(aX,aY,aZ,bX,bY,bZ,&nX,&nY,&nZ);
  glBindTexture(GL_TEXTURE_2D,texture[2]);

  //left triangle (-Z)
  glBegin(GL_TRIANGLES);
    glNormal3d(nX,nY,-nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipBowXfront, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipBowXend, wid, wid);
    glTexCoord2f(0,0);glVertex3d(shipBowXend,-wid, wid);

    //right triangle (Z+)
    glNormal3d(nX,nY,nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipBowXfront, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipBowXend, wid,-wid);
    glTexCoord2f(0,0);glVertex3d(shipBowXend,-wid,-wid);

    findDispVector(shipBowXend,wid,wid,shipBowXfront,0.0,0.0,&aX,&aY,&aZ);
    findDispVector(shipBowXend,wid,wid,shipBowXend,wid,-wid,&bX,&bY,&bZ);
    findNormalVector(aX,aY,aZ,bX,bY,bZ,&nX,&nY,&nZ);

    //Top triangle
    glNormal3d(nX,nY,nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipBowXfront, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipBowXend, wid, wid);
    glTexCoord2f(0,0);glVertex3d(shipBowXend, wid,-wid);

    //Bottom triangle
    glNormal3d(nX,-nY,nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipBowXfront, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipBowXend,-wid, wid);
    glTexCoord2f(0,0);glVertex3d(shipBowXend,-wid,-wid);
  glEnd();

  // ----------------------------------------------------------
  // Front tip
  // ---------------------------------------------------------
  glLineWidth(2);
  glBegin(GL_LINES);
    glNormal3d(1,0,0);
    glColor3f(1, 0, 0); 
    glVertex3f(shipBowXfront, 0, 0);
    glVertex3f(shipBowXfront+2, 0, 0);
  glEnd();
  glLineWidth(1);

  // ----------------------------------------------------------
  // Fuselage
  // ---------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
    glNormal3d(0,0,1);
    glTexCoord2f(3,0.5);glVertex3d(shipBowXend, wid, wid);
    glTexCoord2f(3,0);glVertex3d(shipBowXend,-wid, wid);
    glTexCoord2f(0,0);glVertex3d(shipSternX,-wid, wid);
    glTexCoord2f(0,0.5);glVertex3d(shipSternX, wid, wid);

    glNormal3d(0,0,-1);
    glTexCoord2f(3,0.5);glVertex3d(shipBowXend, wid,-wid);
    glTexCoord2f(3,0);glVertex3d(shipBowXend,-wid,-wid);
    glTexCoord2f(0,0);glVertex3d(shipSternX,-wid,-wid);
    glTexCoord2f(0,0.5);glVertex3d(shipSternX, wid,-wid);

    glNormal3d(0,1,0);
    glTexCoord2f(3,0.5);glVertex3d(shipBowXend, wid, wid);
    glTexCoord2f(3,0);glVertex3d(shipBowXend, wid,-wid);
    glTexCoord2f(0,0);glVertex3d(shipSternX, wid,-wid);
    glTexCoord2f(0,0.5);glVertex3d(shipSternX, wid, wid);

    glNormal3d(0,-1,0);
    glTexCoord2f(3,0.5);glVertex3d(shipBowXend,-wid, wid);
    glTexCoord2f(3,0);glVertex3d(shipBowXend,-wid,-wid);
    glTexCoord2f(0,0);glVertex3d(shipSternX,-wid,-wid);
    glTexCoord2f(0,0.5);glVertex3d(shipSternX,-wid, wid);

    glNormal3d(-1,0,0);
    glTexCoord2f(3,0.5);glVertex3d(shipSternX,-wid, wid);
    glTexCoord2f(3,0);glVertex3d(shipSternX, wid, wid);
    glTexCoord2f(0,0);glVertex3d(shipSternX, wid,-wid);
    glTexCoord2f(0,0.5);glVertex3d(shipSternX,-wid,-wid);
  glEnd();

  // ----------------------------------------------------------
  // Cockpit
  // ---------------------------------------------------------
  glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  Sphere(cockpitX,cockpitY,0,0.9);

  // ----------------------------------------------------------
  // Canards
  // ---------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glColor3f(0.8,0.8,0.8);
  glBegin(GL_TRIANGLES);
    //Right Canard (upper)
    glNormal3d(0,1,0);
    glTexCoord2f(0,1);glVertex3d(canardXend, 0, wid);
    glTexCoord2f(1,0);glVertex3d(canardXend, 0, canardZ);
    glTexCoord2f(0,0);glVertex3d(canardXfront, 0, wid);

    //Right Canard (lower)
    glNormal3d(0,-1,0);
    glTexCoord2f(0,1);glVertex3d(canardXend, -0.001, wid);
    glTexCoord2f(1,0);glVertex3d(canardXend, -0.001, canardZ);
    glTexCoord2f(0,0);glVertex3d(canardXfront, -0.001, wid);

    //Left Canard (upper)
    glNormal3d(0,1,0);
    glTexCoord2f(1,1);glVertex3d(canardXend, 0, -wid);
    glTexCoord2f(0,0);glVertex3d(canardXend, 0, -canardZ);
    glTexCoord2f(0,1);glVertex3d(canardXfront, 0, -wid);

    //Left Canard (lower)
    glNormal3d(0,-1,0);
    glTexCoord2f(1,1);glVertex3d(canardXend, -0.001, -wid);
    glTexCoord2f(0,0);glVertex3d(canardXend, -0.001, -canardZ);
    glTexCoord2f(0,1);glVertex3d(canardXfront, -0.001, -wid);
  glEnd();

  // ----------------------------------------------------------
  // Wing tips
  // ---------------------------------------------------------
  glLineWidth(2);
  glColor3f(1,0,0);
  glBegin(GL_LINES);
    //Right wing line
    glNormal3d(0,1,0);
    glVertex3d(wingLineXend, -wid, wingLineZ);
    glVertex3d(wingLinefrontX, -wid, wingLineZ);

    glNormal3d(0,-1,0);
    glVertex3d(wingLineXend, -wid-0.01, wingLineZ);
    glVertex3d(wingLinefrontX, -wid-0.01, wingLineZ);

    //Left wing line
    glNormal3d(0,1,0);
    glVertex3d(wingLineXend, -wid, -wingLineZ);
    glVertex3d(wingLinefrontX, -wid, -wingLineZ);

    glNormal3d(0,-1,0);
    glVertex3d(wingLineXend, -wid-0.01, -wingLineZ);
    glVertex3d(wingLinefrontX, -wid-0.01, -wingLineZ);
  glEnd();
  glLineWidth(1);
 

  // ----------------------------------------------------------
  // Vertical tail
  // ----------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[2]);

  //  Vertical tail (Z+)
  glColor3f(0.8,0.8,0.8);
  glBegin(GL_POLYGON);
  glNormal3d(0,0,1);
    glTexCoord2f(0,1);glVertex3d(shipSternX, 6.0, 0.001);
    glTexCoord2f(0,0);glVertex3d(shipSternX, 1.0, 0.001);
    glTexCoord2f(1,0);glVertex3d(wingXend+4, 1.0, 0.001);
  glEnd();

  //  Vertical tail (-Z)
  glColor3f(0.8,0.8,0.8);
  glBegin(GL_POLYGON);
    glNormal3d(0,0,-1);
    glTexCoord2f(0,1);glVertex3d(shipSternX, 6.0, 0.0);
    glTexCoord2f(0,0);glVertex3d(shipSternX, 1.0, 0.0);
    glTexCoord2f(1,0);glVertex3d(wingXend+4, 1.0, 0.0);
  glEnd();

  // ----------------------------------------------------------
  // Wings
  // ----------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[2]);

  glColor3f(0.8,0.8,0.8);
  //Right wing (upper)
  glBegin(GL_POLYGON);
    glNormal3d(0,1,0);
    glTexCoord2f(0,0);glVertex3d(wingXend, -wid, wid);
    glTexCoord2f(0,2);glVertex3d(wingXend, -wid, wingZ);
    glTexCoord2f(2,2);glVertex3d(wingXfrontFold, -wid, wingZ);
    glTexCoord2f(3,0);glVertex3d(wingXfront, -wid, wid);
  glEnd();

  //Right wing (lower)
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(0,0);glVertex3d(wingXend, -wid-0.01, wid);
    glTexCoord2f(0,2);glVertex3d(wingXend, -wid-0.01, wingZ);
    glTexCoord2f(2,2);glVertex3d(wingXfrontFold-0.01, -wid, wingZ);
    glTexCoord2f(3,0);glVertex3d(wingXfront, -wid-0.01, wid);
  glEnd();

  //Left wing (upper)
  glBegin(GL_POLYGON);
    glNormal3d(0,1,0);
    glTexCoord2f(0,0);glVertex3d(wingXend, -wid, -wid);
    glTexCoord2f(0,2);glVertex3d(wingXend, -wid, -wingZ);
    glTexCoord2f(2,2);glVertex3d(wingXfrontFold, -wid, -wingZ);
    glTexCoord2f(3,0);glVertex3d(wingXfront, -wid, -wid);
  glEnd();

  //Left wing (lower)
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(0,0);glVertex3d(wingXend, -wid-0.01, -wid);
    glTexCoord2f(0,2);glVertex3d(wingXend, -wid-0.01, -wingZ);
    glTexCoord2f(2,2);glVertex3d(wingXfrontFold, -wid-0.01, -wingZ);
    glTexCoord2f(3,0);glVertex3d(wingXfront, -wid-0.01, -wid);
  glEnd();

  // ----------------------------------------------------------
  // Rear engine
  // ----------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[4]);
  engineSphere(shipSternX,0,0,1,-90);

  glBindTexture(GL_TEXTURE_2D,texture[11]);
  disk(shipSternX-1-0.01,0,0,1,90);

  //  Undo transformations
  glPopMatrix();
}

// ----------------------------------------------------------
//  XB70 Bomber jet
// ----------------------------------------------------------

/*
 *  Draw XB70 Bomber jet
 *    front nose at (x,y,z)
 *    nose points towards (dx,dy,dz)
 *    UP vector (ux,uy,uz) 
 *    scale of the plane xyz (scale)
 *    Rotation about X - roll (thx)
 *    Rotation about Z - pitch (thz) 
 */
void XB70Bomber(double x,double y,double z,
                double dx,double dy,double dz,
                double ux,double uy, double uz, double scale, double thx, double thz)
{
  const double shipFrontNoseX = 1.0;
  const double shipRearNoseX = -9.0;
  const double shipWidth = 2.0;
  const double cockpitLocX = shipRearNoseX - shipWidth + 2.25;
  const double shipRearX = -60.0;
  const double wingsFrontX = -20.0;
  const double wingsRearX = -60.0;
  const double shipFuselageHeight = 2.0;
  const double frontFuselageXEnd = wingsFrontX;

  const double foldingWingZ = 15;
  const double foldingWingXfront = -50;
  const double foldingWingYHeight = 5;

  const double verticalsZ = 10;
  const double verticalsHeight = 7;
  const double verticalsXfront = -50;

  const double canardFrontX = shipRearNoseX - 2;
  const double canardRearX = canardFrontX - 6;
  const double canardZ = 5.5;

  //Inlets
  const double inletMiddleForntX = wingsFrontX - 3;
  const double inletMiddleRearX = shipRearX;
  const double inletY = -shipFuselageHeight - 1.5;
  const double inletSidefrontX = wingsFrontX - 6;

  //  Unit vector in direction of flght
  double D0 = sqrt(dx*dx+dy*dy+dz*dz);
  double X0 = dx/D0;
  double Y0 = dy/D0;
  double Z0 = dz/D0;

  //  Unit vector in "up" direction
  double D1 = sqrt(ux*ux+uy*uy+uz*uz);
  double X1 = ux/D1;
  double Y1 = uy/D1;
  double Z1 = uz/D1;

  //  Cross product gives the third vector
  double X2 = Y0*Z1-Y1*Z0;
  double Y2 = Z0*X1-Z1*X0;
  double Z2 = X0*Y1-X1*Y0;

  //  Rotation matrix
  double mat[16];
  mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
  mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
  mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
  mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

  //  Save current transforms
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glMultMatrixd(mat);
  glRotated(thx,1,0,0);
  glRotated(thz,0,0,1);
  glScaled(scale,scale,scale);

  //For normal vector calculations
  double aX, aY, aZ;
  double bX, bY, bZ;
  double nX, nY, nZ;

  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  //glBindTexture(GL_TEXTURE_2D,texture[0]);

  // ----------------------------------------------------------
  // Nose
  // ----------------------------------------------------------

  //  Front Nose (4 sided)
  glColor3f(0.7,0.7,0.7);
  glBindTexture(GL_TEXTURE_2D,texture[2]);
    
    //Right side nose
  glBegin(GL_TRIANGLES);
    findDispVector(shipRearNoseX, shipFuselageHeight, shipWidth, shipFrontNoseX, 0.0,0.0, &aX,&aY,&aZ);
    findDispVector(shipRearNoseX, shipFuselageHeight, shipWidth, shipRearNoseX, -shipFuselageHeight, shipWidth, &bX, &bY, &bZ);
    findNormalVector(aX,aY,aZ,bX,bY,bZ,&nX,&nY,&nZ);
    glNormal3d(nX,nY,-nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipFrontNoseX, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipRearNoseX, shipFuselageHeight, shipWidth);
    glTexCoord2f(0,0);glVertex3d(shipRearNoseX,-shipFuselageHeight, shipWidth);

    //Right side nose
    glNormal3d(nX,nY,nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipFrontNoseX, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipRearNoseX, shipFuselageHeight,-shipWidth);
    glTexCoord2f(0,0);glVertex3d(shipRearNoseX,-shipFuselageHeight,-shipWidth);

    //Top side nose
    findDispVector(shipRearNoseX, shipFuselageHeight, shipWidth, shipFrontNoseX, 0.0,0.0, &aX,&aY,&aZ);
    findDispVector(shipRearNoseX, shipFuselageHeight, shipWidth, shipRearNoseX, shipFuselageHeight, -shipWidth, &bX,&bY,&bZ);
    findNormalVector(aX,aY,aZ,bX,bY,bZ,&nX,&nY,&nZ);

    glNormal3d(nX,nY,nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipFrontNoseX, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipRearNoseX, shipFuselageHeight, shipWidth);
    glTexCoord2f(0,0);glVertex3d(shipRearNoseX, shipFuselageHeight,-shipWidth);

    //Bottom side nose3
    glNormal3d(nX,-nY,nZ);
    glTexCoord2f(0.5,0.5);glVertex3d(shipFrontNoseX, 0.0, 0.0);
    glTexCoord2f(0,0.5);glVertex3d(shipRearNoseX,-shipFuselageHeight, shipWidth);
    glTexCoord2f(0,0);glVertex3d(shipRearNoseX,-shipFuselageHeight,-shipWidth);
  glEnd();
  
  // ----------------------------------------------------------
  // Fuselage
  // ----------------------------------------------------------

  //  Fuselage (front section)
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_QUADS);
    glColor3f(1,1,1);
    //Right side
    glNormal3d(0,0,1);
    glTexCoord2f(1,0.5); glVertex3d(shipRearNoseX, shipFuselageHeight, shipWidth);
    glTexCoord2f(1,0);glVertex3d(shipRearNoseX,-shipFuselageHeight, shipWidth);
    glTexCoord2f(0,0);glVertex3d(frontFuselageXEnd,-shipFuselageHeight, shipWidth);
    glTexCoord2f(0,0.5);glVertex3d(frontFuselageXEnd, shipFuselageHeight, shipWidth);
  glEnd();

    //Left side
  glBegin(GL_QUADS);
    glNormal3d(0,0,-1);
    glTexCoord2f(1,0.5);glVertex3d(shipRearNoseX, shipFuselageHeight,-shipWidth);
    glTexCoord2f(1,0);glVertex3d(shipRearNoseX,-shipFuselageHeight,-shipWidth);
    glTexCoord2f(0,0);glVertex3d(frontFuselageXEnd,-shipFuselageHeight,-shipWidth);
    glTexCoord2f(0,0.5);glVertex3d(frontFuselageXEnd, shipFuselageHeight,-shipWidth);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[0]);
    //Top side
  glBegin(GL_QUADS);
    glNormal3d(0,1,0);
    glTexCoord2f(1,1);glVertex3d(shipRearNoseX, shipFuselageHeight, -shipWidth);
    glTexCoord2f(0,1);glVertex3d(frontFuselageXEnd, shipFuselageHeight, -shipWidth);
    glTexCoord2f(0,0);glVertex3d(frontFuselageXEnd, shipFuselageHeight, shipWidth);
    glTexCoord2f(1,0);glVertex3d(shipRearNoseX, shipFuselageHeight, shipWidth);
  glEnd();
    
  glBegin(GL_QUADS);
    //Bottom side
    glNormal3d(0,-1,0);
    glTexCoord2f(0,0);glVertex3d(shipRearNoseX,-shipFuselageHeight, shipWidth);
    glTexCoord2f(0,1);glVertex3d(shipRearNoseX,-shipFuselageHeight,-shipWidth);
    glTexCoord2f(5,1);glVertex3d(shipRearX,-shipFuselageHeight,-shipWidth);
    glTexCoord2f(5,0);glVertex3d(shipRearX,-shipFuselageHeight, shipWidth);
  glEnd();

  //Calculation of normal for second fuselage section
  
  findDispVector(frontFuselageXEnd, shipFuselageHeight, -shipWidth, shipRearX, -shipFuselageHeight, -shipWidth, &aX, &aY, &aZ);
  findDispVector(shipRearX, -shipFuselageHeight, -shipWidth, shipRearX, -shipFuselageHeight, shipWidth, &bX, &bY, &bZ);
  findNormalVector(aX,aY,aZ,bX,bY,bZ,&nX,&nY,&nZ);


  //Rear fuselage section:
  glBegin(GL_QUADS);
    //Second section top side (biggest part)
    glNormal3d(nX,nY,nZ);
    glTexCoord2f(4,0);glVertex3d(frontFuselageXEnd, shipFuselageHeight, shipWidth);
    glTexCoord2f(4,1);glVertex3d(frontFuselageXEnd, shipFuselageHeight,-shipWidth);
    glTexCoord2f(0,1);glVertex3d(shipRearX, -shipFuselageHeight,-shipWidth);
    glTexCoord2f(0,0);glVertex3d(shipRearX, -shipFuselageHeight, shipWidth);
  glEnd();

  //Rear fuselage sides
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_POLYGON);
    //Right side
    glNormal3d(0,0,1);
    glTexCoord2f(5,1);glVertex3d(frontFuselageXEnd, shipFuselageHeight, shipWidth);
    glTexCoord2f(0,0);glVertex3d(shipRearX, -shipFuselageHeight, shipWidth);
    glTexCoord2f(5,0);glVertex3d(frontFuselageXEnd, -shipFuselageHeight, shipWidth);
  glEnd();

  glBegin(GL_POLYGON);
    //Left side
    glNormal3d(0,0,-1);
    glTexCoord2f(0,1);glVertex3d(frontFuselageXEnd, shipFuselageHeight, -shipWidth);
    glTexCoord2f(0,0);glVertex3d(frontFuselageXEnd, -shipFuselageHeight, -shipWidth);
    glTexCoord2f(5,0);glVertex3d(shipRearX, -shipFuselageHeight, -shipWidth);
  glEnd();

  // ----------------------------------------------------------
  // Front tip
  // ---------------------------------------------------------
  glLineWidth(2);
  glBegin(GL_LINES);
    glNormal3d(1,0,0);
    glColor3f(1, 0, 0); 
    glVertex3f(shipFrontNoseX, 0, 0);
    glVertex3f(shipFrontNoseX+2, 0, 0);
  glEnd();
  glLineWidth(1);

  // ----------------------------------------------------------
  // Wings
  // ----------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  //Right wing (bottom portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight, shipWidth);
    glTexCoord2f(0,2);glVertex3d(wingsRearX, -shipFuselageHeight, foldingWingZ);
    glTexCoord2f(2,2);glVertex3d(foldingWingXfront, -shipFuselageHeight, foldingWingZ);
    glTexCoord2f(3,0);glVertex3d(wingsFrontX, -shipFuselageHeight, shipWidth);
  glEnd();

   //Right (upper portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(0,1,0);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight+0.01, shipWidth);
    glTexCoord2f(0,2);glVertex3d(wingsRearX, -shipFuselageHeight+0.01, foldingWingZ);
    glTexCoord2f(2,2);glVertex3d(foldingWingXfront, -shipFuselageHeight+0.01, foldingWingZ);
    glTexCoord2f(3,0);glVertex3d(wingsFrontX, -shipFuselageHeight+0.01, shipWidth);
  glEnd();

  //Left wing (bottom portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight, -shipWidth);
    glTexCoord2f(0,2);glVertex3d(wingsRearX, -shipFuselageHeight, -foldingWingZ);
    glTexCoord2f(2,2);glVertex3d(foldingWingXfront, -shipFuselageHeight, -foldingWingZ);
    glTexCoord2f(3,0);glVertex3d(wingsFrontX, -shipFuselageHeight, -shipWidth);
  glEnd();

  //Left wing (upper portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(0,1,0);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight+0.01, -shipWidth);
    glTexCoord2f(0,2);glVertex3d(wingsRearX, -shipFuselageHeight+0.01, -foldingWingZ);
    glTexCoord2f(2,2);glVertex3d(foldingWingXfront, -shipFuselageHeight+0.01, -foldingWingZ);
    glTexCoord2f(3,0);glVertex3d(wingsFrontX, -shipFuselageHeight+0.01, -shipWidth);
  glEnd();

  // ----------------------------------------------------------
  // Folding Wings
  // ----------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[0]);

  findDispVector(foldingWingXfront, -shipFuselageHeight, foldingWingZ, wingsRearX, -shipFuselageHeight, foldingWingZ, &aX,&aY,&aZ);
  findDispVector(wingsRearX, -shipFuselageHeight, foldingWingZ, wingsRearX, -foldingWingYHeight-shipFuselageHeight, foldingWingZ+2, &bX,&bY,&bZ);
  findNormalVector(aX,aY,aZ,bX,bY,bZ,&nX,&nY,&nZ);

  //Right folding wing (upper portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(nX,nY,nZ);
    glTexCoord2f(0,0.5);glVertex3d(wingsRearX, -shipFuselageHeight, foldingWingZ);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -foldingWingYHeight-shipFuselageHeight, foldingWingZ+3);
    glTexCoord2f(1,0);glVertex3d(foldingWingXfront, -shipFuselageHeight, foldingWingZ);
  glEnd();

  //Right folding wing (bottom portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(-nX,-nY,-nZ);
    glTexCoord2f(0,0.5);glVertex3d(wingsRearX, -shipFuselageHeight, foldingWingZ);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -foldingWingYHeight-shipFuselageHeight-0.01, foldingWingZ+3);
    glTexCoord2f(1,0);glVertex3d(foldingWingXfront, -shipFuselageHeight, foldingWingZ);
  glEnd();

  //Left folding wing (upper portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(nX,nY,-nZ);
    glTexCoord2f(0,0.5);glVertex3d(wingsRearX, -shipFuselageHeight, -foldingWingZ);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -foldingWingYHeight-shipFuselageHeight, -foldingWingZ-3);
    glTexCoord2f(1,0);glVertex3d(foldingWingXfront, -shipFuselageHeight, -foldingWingZ);
  glEnd();

  //Left folding wing (bottom portion)
  glColor3f(1,1,1);
  glBegin(GL_POLYGON);
    glNormal3d(nX,-nY,nZ);
    glTexCoord2f(0,0.5);glVertex3d(wingsRearX, -shipFuselageHeight, -foldingWingZ);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -foldingWingYHeight-shipFuselageHeight-0.01, -foldingWingZ-3);
    glTexCoord2f(1,0);glVertex3d(foldingWingXfront, -shipFuselageHeight, -foldingWingZ);
  glEnd();

  // ----------------------------------------------------------
  // Vertical stabilizers
  // ----------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  //Righ verical (Z+)
  glColor3f(0.8,0.8,0.8);
  glBegin(GL_POLYGON);
    glNormal3d(0,0,1);
    glTexCoord2f(0,1);glVertex3d(wingsRearX, verticalsHeight, verticalsZ);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight,verticalsZ);
    glTexCoord2f(1,0);glVertex3d(verticalsXfront, -shipFuselageHeight, verticalsZ);
  glEnd();

  //Righ verical (-Z)
  glColor3f(0.8,0.8,0.8);
  glBegin(GL_POLYGON);
    glNormal3d(0,0,-1);
    glTexCoord2f(0,1);glVertex3d(wingsRearX, verticalsHeight, verticalsZ-0.01);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight,verticalsZ-0.01);
    glTexCoord2f(1,0);glVertex3d(verticalsXfront, -shipFuselageHeight, verticalsZ-0.01);
  glEnd();

  //Left verical (Z+)
  glColor3f(0.8,0.8,0.8);
  glBegin(GL_POLYGON);
    glNormal3d(0,0,1);
    glTexCoord2f(0,1);glVertex3d(wingsRearX, verticalsHeight, -verticalsZ);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight,-verticalsZ);
    glTexCoord2f(1,0);glVertex3d(verticalsXfront, -shipFuselageHeight, -verticalsZ);
  glEnd();

  //Left verical (-Z)
  glColor3f(0.8,0.8,0.8);
  glBegin(GL_POLYGON);
    glNormal3d(0,0,-1);
    glTexCoord2f(0,1);glVertex3d(wingsRearX, verticalsHeight, -verticalsZ-0.01);
    glTexCoord2f(0,0);glVertex3d(wingsRearX, -shipFuselageHeight,-verticalsZ-0.01);
    glTexCoord2f(1,0);glVertex3d(verticalsXfront, -shipFuselageHeight, -verticalsZ-0.01);
  glEnd();

  // ----------------------------------------------------------
  // Canards
  // ----------------------------------------------------------

  //Right Canard (bottom portion)
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glColor3f(0.75,0.75,0.75);
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(1,0);glVertex3d(canardRearX,0,shipWidth);
    glTexCoord2f(0.8,0.5);glVertex3d(canardRearX+1,0,canardZ);
    glTexCoord2f(0.3,0.5);glVertex3d(canardRearX+4,0,canardZ);
    glTexCoord2f(0,0);glVertex3d(canardFrontX,0,shipWidth);
  glEnd();

  //Right Canard (upper portion)
  glBegin(GL_POLYGON);
    glNormal3d(0,1,0);
    glTexCoord2f(1,0);glVertex3d(canardRearX,0+0.01,shipWidth);
    glTexCoord2f(0.8,0.5);glVertex3d(canardRearX+1,0+0.01,canardZ);
    glTexCoord2f(0.3,0.5);glVertex3d(canardRearX+4,0+0.01,canardZ);
    glTexCoord2f(0,0);glVertex3d(canardFrontX,0+0.01,shipWidth);  //Frontmost section
  glEnd();

  //Left Canard (bottom portion)
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(1,0);glVertex3d(canardRearX,0,-shipWidth);
    glTexCoord2f(0.8,0.5);glVertex3d(canardRearX+1,0,-canardZ);
    glTexCoord2f(0.3,0.5);glVertex3d(canardRearX+4,0,-canardZ);
    glTexCoord2f(0,0);glVertex3d(canardFrontX,0,-shipWidth);
  glEnd();

  //Left Canard (upper portion)
  glBegin(GL_POLYGON);
    glTexCoord2f(1,0);glNormal3d(0,1,0);
    glVertex3d(canardRearX,0+0.01,-shipWidth);
    glTexCoord2f(0.8,0.5);glVertex3d(canardRearX+1,0+0.01,-canardZ);
    glTexCoord2f(0.3,0.5);glVertex3d(canardRearX+4,0+0.01,-canardZ);
    glTexCoord2f(0,0);glVertex3d(canardFrontX,0+0.01,-shipWidth);
  glEnd();

  // ----------------------------------------------------------
  // Inlets
  // ----------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  //Inlet middle, left and right side
  glColor3f(1,1,1); //white
  glBegin(GL_QUADS);
    //Middle inlet
    glTexCoord2f(3,0.2);glVertex3d(inletMiddleForntX,inletY,0);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, 0);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, -shipFuselageHeight, 0);
    glTexCoord2f(3,0);glVertex3d(inletMiddleForntX, -shipFuselageHeight,0);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[0]);
    //Right inlet side
  glBegin(GL_QUADS);
    glNormal3d(0,0,1);
    glTexCoord2f(3,0.2);glVertex3d(inletSidefrontX,inletY,shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, shipWidth);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, -shipFuselageHeight, shipWidth);
    glTexCoord2f(3,0);glVertex3d(inletSidefrontX, -shipFuselageHeight, shipWidth);
  glEnd();

    //Left inlet side
  glBegin(GL_QUADS);
    glNormal3d(0,0,-1);
    glTexCoord2f(3,0.2);glVertex3d(inletSidefrontX,inletY,-shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, -shipWidth);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, -shipFuselageHeight, -shipWidth);
    glTexCoord2f(3,0);glVertex3d(inletSidefrontX, -shipFuselageHeight, -shipWidth);
  glEnd();

  //Back inlet left bottom side
  glColor3f(0.75,0.75,0.75);
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(2.8,0.2);glVertex3f(inletSidefrontX, inletY, shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, shipWidth);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, inletY, 0);
    glTexCoord2f(3,0);glVertex3d(inletMiddleForntX, inletY, 0);
  glEnd();

  //Back inlet right bottom side
  glColor3f(0.75,0.75,0.75);
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(2.8,0.2);glVertex3f(inletSidefrontX, inletY, -shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, -shipWidth);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, inletY, 0);
    glTexCoord2f(3,0);glVertex3d(inletMiddleForntX, inletY, 0);
  glEnd();

  // ----------------------------------------------------------
  // Cockpit
  // ----------------------------------------------------------
  glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  Sphere(cockpitLocX,shipFuselageHeight - 0.8,0,shipWidth - 0.25);

  //  Undo transformations
  glPopMatrix();
}


// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display(GLFWwindow* window){
  //  Clear screen and Z-buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  // Reset transformations
  glLoadIdentity();

  if(projectionMode == 1){
    //First person
    cameraLookX = 2*dim*Sin(th);
    cameraLookZ = -2*dim*Cos(th);
    cameraLookY = 2*dim*Sin(ph);
    gluLookAt(cameraX,cameraY,cameraZ,
              cameraX+cameraLookX,cameraLookY + cameraLookY,cameraZ+cameraLookZ,
              0,1,0); 
  }else if(projectionMode == 2){
    //  Perspective - set eye position
    double Ex = -2*(dim/2)*Sin(th)*Cos(ph);
    double Ey = +2*(dim/2)        *Sin(ph);
    double Ez = +2*(dim/2)*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
  }

  //  Flat or smooth shading
  glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

  //  Light switch
  if (light)
  {
    //  Translate intensity to color vectors
    float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
    //  Light position
    float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
    //  Draw light position as ball (still no lighting here)
    glColor3f(1,1,1);
    ball(Position[0],Position[1],Position[2] , 0.25);
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Location of viewer for specular calculations
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position);
  }
  else{
    glDisable(GL_LIGHTING);
  }

  if(drawAxis){
    drawAxisLines();
    drawAxisLabels();
  }

  switch(currentScene){
    case 1:
      XB70Bomber(10,-5,0 , 1,0,0, 0,1,0, 1.6, 0, 5);
      FighterJet(-120,-5.5,0, 1,0,0, 0,1,0, 1.6, 0, 5);
      FighterJet(10,-5,-50 , 1,0,0, 0,1,0, 1.6, 0, 5);
      FighterJet(10,-5,50 , 1,0,0, 0,1,0, 1.6, 0, 5);
      FighterJet(-40,-5,90 , 1,0,0, 0,1,0, 1.6, 0, 5);
      FighterJet(-40,-5,-90 , 1,0,0, 0,1,0, 1.6, 0, 5);
      FighterJet(90,20,0, 1,0,0, 0,1,0, 1.6, 0, 5);
      skyboxCube(0,0,0,250,250,250,0);
      break;
    case 2:
      FighterJet(10,5,50, 1,0,0, 0,1,0,1.6, 25,25);
      FighterJet(10,5,-50, 1,0,0, 0,1,0,1.6, -25,25);
      FighterJet(-10,50,0, 1,0,0, 0,1,0,1.6, 0, 25);
      XB70Bomber(20,20,0 , 1,0,0, 0,1,0,1.6, 0, 10);
      FighterJet(20,-40,0, 1,0,0, 0,-1,0,1.6, 0, 25);
      skyboxCube(0,0,0,250,250,250,0);
      break;
    case 3:
      XB70Bomber(10,-5,0 , 1,0,0, 0,1,0, 1.6, THX+90, 0);
      FighterJet(10,-5,-40 , 1,0,0, 0,1,0, 1.6, -THX, 0);
      FighterJet(10,-5,40 , 1,0,0, 0,1,0, 1.6, THX, 0);
      FighterJet(10,30,0 , 1,0,0, 0,1,0, 1.6, 0, THX);
      skyboxCube(0,0,0,250,250,250,0);
      break; 
  }
  
  //  Display parameters
  glColor3f(0,1,0);
  glWindowPos2i(5,5);
  Print("Angle=%d,%d  Dim=%.1f FOV=%d Light=%s",th,ph,dim,fov,light?"On":"Off");
  if (light)
  {
    glWindowPos2i(5,45);
    Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
    glWindowPos2i(5,25);
    Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
  }
  glWindowPos2i(5,65);
  Print("Camera Mode: ");
  if(projectionMode == 1)
    Print(" First Person  ");
  else if (projectionMode == 2)
    Print(" Perspective  ");

  glWindowPos2i(5,85);
  if(drawAxis == true)
    Print("Axis ON");
  else
    Print("Axis OFF");

  //  Render the scene and make it visible
  ErrCheck("display");
  glFlush();
  glfwSwapBuffers(window);
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

void reshape(GLFWwindow* window,int width,int height)
{
  //  Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Set projection
  Project(fov, asp, dim, projectionMode);
}

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){

  int width,height;
  GLFWwindow* window;

  //  Initialize GLFW
  if (!glfwInit()) Fatal("Cannot initialize glfw\n");

  //  Error callback

  glfwSetErrorCallback(error);
  //  Set window properties
  glfwWindowHint(GLFW_RESIZABLE,1);
  glfwWindowHint(GLFW_DOUBLEBUFFER,1);

  //  Create window and make current
  window = glfwCreateWindow(1280,720, "Riad Shash (Ray) - GLFW", nullptr, nullptr);
  if (!window) Fatal("Cannot create GLFW window\n");
  glfwMakeContextCurrent(window);

  //  Enable VSYNC
  glfwSwapInterval(1);

  //  Set callback for window resize
  glfwSetWindowSizeCallback(window,reshape);

  //  Set initial window size
  glfwGetWindowSize(window,&width,&height);

  reshape(window,width,height);

  //Callbacks
  glfwSetKeyCallback(window,key);
  glfwSetCursorPosCallback(window,mouseCallback);

  //  Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);

  //  Load textures
  texture[0] = LoadTexBMP("Textures/WhiteMetal.bmp");
  texture[1] = LoadTexBMP("Textures/MetalUs.bmp");
  texture[2] = LoadTexBMP("Textures/al.bmp");
  texture[3] = LoadTexBMP("Textures/glass.bmp");
  texture[4] = LoadTexBMP("Textures/engineTexture.bmp");

  //  For the skybox
  texture[5] = LoadTexBMP("Textures/front.bmp");
  texture[6] = LoadTexBMP("Textures/left.bmp");
  texture[7] = LoadTexBMP("Textures/right.bmp");
  texture[8] = LoadTexBMP("Textures/top.bmp");
  texture[9] = LoadTexBMP("Textures/bottom.bmp");
  texture[10] = LoadTexBMP("Textures/back.bmp");

  //Afterburner texture from: http://www.lockonfiles.com/files/file/1967-gys-f-15c-inner-afterburner-texture/
  texture[11] = LoadTexBMP("Textures/imageBurner.bmp");

  //  Event loop
  ErrCheck("init");
  while(!glfwWindowShouldClose(window))
  {
    //  Display
    display(window);
    //  Process any events
    glfwPollEvents();
  }
  //  Shut down GLFW
  glfwDestroyWindow(window);
  glfwTerminate();

  // ANSI C requires main to return int
  return 0;
}
