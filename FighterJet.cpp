#include "CSCIx229.h"

class FighterJet{
public: 

    int shininess =   0;  // Shininess (power of two)
    float shiny   =   1;  // Shininess (value)
    float emission  =   0;  // Emission intensity (%)

    FighterJet(){
        //  Load textures
        texture[0] = LoadTexBMP("Textures/WhiteMetal.bmp");
        texture[1] = LoadTexBMP("Textures/MetalUs.bmp");
        texture[2] = LoadTexBMP("Textures/al.bmp");
        texture[3] = LoadTexBMP("Textures/glass.bmp");
        texture[4] = LoadTexBMP("Textures/engineTexture.bmp");
        texture[5] = LoadTexBMP("Textures/imageBurner.bmp");
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
        void drawFighterJet(double x,double y,double z,
                           double dx,double dy,double dz,
                           double ux,double uy, double uz, double scale, double thx, double thz)
{
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
  glTranslated(x+12,y,z);
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

  glBindTexture(GL_TEXTURE_2D,texture[5]);
  disk(shipSternX-1-0.01,0,0,1,90);

  // ----------------------------------------------------------
  // Landing Gear
  // ----------------------------------------------------------
  drawLandingGear(shipBowXend-2,-wid,0,3,0.5);
  drawLandingGear(wingXend+3,-wid,-3,3,0.5);
  drawLandingGear(wingXend+3,-wid,3,3,0.5);

  //  Undo transformations
  glPopMatrix();
}


private:
    unsigned int texture[10]; // Texture names
};