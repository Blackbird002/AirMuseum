#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include "CSCIx229.h"
#include "Camera.cpp"
#include <iostream>
// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------


int currentScene = 1;
bool drawAxis = true;
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio

/*
  1 - First person 
  2 - Perspective (starts in perspective mode)
*/
int projectionMode = 2;     

double THX;
double THZ;

// Light values
int light     =   0;  //  Lighting
int one       =   1;  // Unit value
float distance  =  70.0f; // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
float emission  =   0;  // Emission intensity (%)
float ambient   =  25.0f;  // Ambient intensity (%)
float diffuse   = 100.0f;  // Diffuse intensity (%)
float specular  =   0.0f;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
float zh        =  90.0f;  // Light azimuth
float ylight  =   0.0f;  // Elevation of light
int move      =   1;  //  Move light
unsigned int texture[12]; // Texture names

double previousMouseY;
double previousMouseX;

Camera* camera;

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void reshape(int,int);
void idle();
void init();
void mouseCallback(GLFWwindow *window, double x, double y);
void key(GLFWwindow* window,int key,int scancode,int action,int mods);
void error(int error, const char* text);
static void FighterJet(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, double scale, double thx, double thz);
static void XB70Bomber(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, double scale, double thx, double thz);
void centerWindow(GLFWwindow *window, GLFWmonitor *monitor);
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
  //int shift = (mods & GLFW_MOD_SHIFT);

  //  Exit on ESC
  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(window,1);
  else if (key==GLFW_KEY_RIGHT)
    camera->th += 2.5;
  else if (key==GLFW_KEY_LEFT)
    camera->th -= 2.5;
  //  Increase/decrease elevation
  else if (key==GLFW_KEY_UP)
    camera->ph += 2.5;
  else if (key==GLFW_KEY_DOWN)
    camera->ph -= 2.5;

  //Moving camera only in First person & FOV
  if (projectionMode == 1){
    if(key == GLFW_KEY_W)
      camera->moveForward();
    else if(key == GLFW_KEY_S)
      camera->moveBackward();  
    else if(key == GLFW_KEY_A)
      camera->strafeLeft();
    else if(key == GLFW_KEY_D)
      camera->strafeRight();
      //  Change field of view angle
    else if (key == GLFW_KEY_Z){
        fov--;   
    }else if (key == GLFW_KEY_X){
        fov++;
    }
  }
  
  // Reproject
  Project(fov, asp, camera->dim, projectionMode);
}

void mouseCallback(GLFWwindow *window, double x, double y)
{
  double deltaY = previousMouseY - y;
  double deltaX = previousMouseX - x;
  //std::cout<<"Mouse X: " <<x <<" " <<"Mouse Y: " <<y <<std::endl;

  if(camera->ph < 91.0 && (deltaY > 0))
    camera->ph += 3;

  if(camera->ph > -91.0 && (deltaY) < 0)
    camera->ph -= 3;

  if(deltaX < 0)
    camera->th += 3;

  if(deltaX > 0)
    camera->th -= 3;

  previousMouseY = y;
  previousMouseX = x;
  // Reproject
  Project(fov, asp, camera->dim, projectionMode);
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
  int mode = 0;
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
  float Emission[]  = {0.0f,0.0f,0.01f*emission,1.0f};
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
  Sphere(cockpitX,cockpitY,0,0.9, emission, shiny);

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
  engineSphere(shipSternX,0,0,1,-90, emission, shiny);

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
  int mode = 0;
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
  float Emission[]  = {0.0f,0.0f,0.01f*emission,1.0f};
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
  Sphere(cockpitLocX,shipFuselageHeight - 0.8,0,shipWidth - 0.25, emission, shiny);

  //  Undo transformations
  glPopMatrix();
}


// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display(GLFWwindow* window){
  //  Clear screen and Z-buffer
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  // Reset transformations
  glLoadIdentity();

  if(projectionMode == 1){
    camera->firstPerson();
  }else if(projectionMode == 2){
    camera->perspectiveMode();
  }

  //  Flat or smooth shading
  glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

  //  Light switch
  if (light)
  {
    //  Translate intensity to color vectors
    float Ambient[]   = {0.01f*ambient ,0.01f*ambient ,0.01f*ambient ,1.0};
    float Diffuse[]   = {0.01f*diffuse ,0.01f*diffuse ,0.01f*diffuse ,1.0};
    float Specular[]  = {0.01f*specular,0.01f*specular,0.01f*specular,1.0};
    //  Light position
    float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0f};
    //  Draw light position as ball (still no lighting here)
    glColor3f(1,1,1);
    ball(Position[0],Position[1],Position[2] ,0.25, emission, shiny, inc);
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

  drawHangar(25,0,17.5,15);
  //skyboxCube(0,0,0,250,250,250,0, emission, shiny, texture);  
  
  //  Display parameters
  glColor3f(0,1,0);
  glWindowPos2i(5,5);
  Print("Angle=%.1f,%.1f Dim=%.1f FOV=%d Light=%s",camera->th,camera->ph,camera->dim,fov,light?"On":"Off");
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

void reshape(GLFWwindow* window,int width,int height)
{
  //  Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Set projection
  Project(fov, asp, camera->dim, projectionMode);
}

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){

  int width,height;
  GLFWwindow* window;

  camera = new Camera(30,15,40);

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

  //Center the GLFW window
  centerWindow(window, glfwGetPrimaryMonitor());
  
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
  // Options, removes the mouse cursor for a more immersive experience
  //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

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
  delete camera;

  // ANSI C requires main to return int
  return 0;
}

//https://vallentin.io/2014/02/07/glfw-center-window
void centerWindow(GLFWwindow *window, GLFWmonitor *monitor)
{
  if (!monitor)
    return;

  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  if (!mode)
    return;

  int monitorX, monitorY;
  glfwGetMonitorPos(monitor, &monitorX, &monitorY);

  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  glfwSetWindowPos(window,
                    monitorX + (mode->width - windowWidth) / 2,
                    monitorY + (mode->height - windowHeight) / 2);
}
