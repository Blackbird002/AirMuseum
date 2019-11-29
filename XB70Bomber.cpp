#include "CSCIx229.h"

class XB70Bomber{
public: 

    int shininess =   0;  // Shininess (power of two)
    float shiny   =   1;  // Shininess (value)
    float emission  =   0;  // Emission intensity (%)

    XB70Bomber(){

        //  Load textures
        texture[0] = LoadTexBMP("Textures/WhiteMetal.bmp");
        texture[1] = LoadTexBMP("Textures/MetalUs.bmp");
        texture[2] = LoadTexBMP("Textures/al.bmp");
        texture[3] = LoadTexBMP("Textures/glass.bmp");
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
void drawBomber(double x,double y,double z,
                double dx,double dy,double dz,
                double ux,double uy, double uz, double scale, double thx, double thz, bool landGear)
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
  glTranslated(x+35*scale,y,z);
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
  // Cockpit
  // ----------------------------------------------------------
  glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  Sphere(cockpitLocX,shipFuselageHeight - 0.8,0,shipWidth - 0.25, emission, shiny);

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

    //Right inlet side
  glBegin(GL_QUADS);
    glNormal3d(0,0,1);
    glTexCoord2f(3,0.2);glVertex3d(inletSidefrontX,inletY,shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, shipWidth+5);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, -shipFuselageHeight, shipWidth+5);
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

  //Left inlet side outside
  glBegin(GL_QUADS);
    glNormal3d(0,0,-1);
    glTexCoord2f(3,0.2);glVertex3d(inletSidefrontX,inletY,-shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, -shipWidth-5);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, -shipFuselageHeight, -shipWidth-5);
    glTexCoord2f(3,0);glVertex3d(inletSidefrontX, -shipFuselageHeight, -shipWidth);
  glEnd();

  //Back inlet left bottom side
  glColor3f(0.75,0.75,0.75);
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(2.8,0.2);glVertex3f(inletSidefrontX, inletY, shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, shipWidth+5);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, inletY, 0);
    glTexCoord2f(3,0);glVertex3d(inletMiddleForntX, inletY, 0);
  glEnd();

  //Back inlet right bottom side
  glColor3f(0.75,0.75,0.75);
  glBegin(GL_POLYGON);
    glNormal3d(0,-1,0);
    glTexCoord2f(2.8,0.2);glVertex3f(inletSidefrontX, inletY, -shipWidth);
    glTexCoord2f(0,0.2);glVertex3d(inletMiddleRearX, inletY, -shipWidth-5);
    glTexCoord2f(0,0);glVertex3d(inletMiddleRearX, inletY, 0);
    glTexCoord2f(3,0);glVertex3d(inletMiddleForntX, inletY, 0);
  glEnd();

  // ----------------------------------------------------------
  // Landing Gear
  // ----------------------------------------------------------
  if(landGear){
    drawLandingGear(frontFuselageXEnd+6,-shipWidth,0,6,1);
    drawLandingGear(inletMiddleRearX+10,inletY,4,4.25,1);
    drawLandingGear(inletMiddleRearX+10,inletY,-4,4.25,1);
  }

  //  Undo transformations
  glPopMatrix();
}



private:
    unsigned int texture[5]; // Texture names
};