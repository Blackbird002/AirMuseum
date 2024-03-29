#include "CSCIx229.h"

class Hangar
{

public:
    int mode;
    int shader[10];

    Hangar(int shaderArray[], int size){
        mode = 0;
        texture[0] = LoadTexBMP("Textures/HangarTextures/hangarFloor256.bmp");
        texture[1] = LoadTexBMP("Textures/HangarTextures/redCarpet.bmp");
        texture[2] = LoadTexBMP("Textures/HangarTextures/hangarWall.bmp");
        texture[3] = LoadTexBMP("Textures/HangarTextures/hangarTopWall.bmp");
        texture[4] = LoadTexBMP("Textures/al.bmp");
        texture[5] = LoadTexBMP("Textures/wood.bmp");

        for(int i =0; i < size; i++){
            shader[i] = shaderArray[i];
        }
    }

    void drawHangar(double x, double y, double z, double r){
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
        glUseProgram(shader[0]);

        //  Save transformation
        glPushMatrix();
        glTranslated(x-25,y,z-17.5);
        glScaled(r,r,r);
        hangarLengthWall(25,0,0,1);
        hangarLengthWall(25,0,35,-1);
        hangarWidthWall(0, 0, 17.5,1);
        hangarWidthWall(50, 0, 17.5,-1);
        hangarRoof(25,10,0,1,0);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        drawBoundaryPolesSquare(7.5,0,7.5,22,shader);
        drawBoundaryPolesSquare(7.5,0,27.5,22,shader);
        drawBoundaryPolesSquare(42.5,0,7.5,22,shader);
        drawBoundaryPolesSquare(42.5,0,27.5,22,shader);
        drawBoundaryPolesRectangle(25,0,6,22,shader);
        drawBoundaryPolesRectangle(25,0,29,22,shader);
        drawBoundaryPolesCenter(25,0,17.5,22,shader);

        //  Undo transofrmations
        glPopMatrix();
        glUseProgram(0);
    }

    void drawHangarFloor(double x, double y, double z, double r){
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
        glUseProgram(shader[0]);

        //  Save transformation
        glPushMatrix();
        glTranslated(x-25,y,z-17.5);
        glScaled(r,r,r);
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

        //  Undo transofrmations
        glPopMatrix();
        glUseProgram(0);
    }

private:

    unsigned int texture[10]; // Texture names

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

        glColor3d(1,1,1);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        
        //Rear section of roof
        glBegin(GL_TRIANGLE_FAN);
            glNormal3d(0,0,1);
            glTexCoord2f(0,0);
            glVertex3f(0,0,0);
            for(int k=0;k<=180;k+=10){
                glTexCoord2f(6*-Cos(k),6*Sin(k));
                glVertex3f(25*Cos(k),7*Sin(k),0);
            }
        glEnd();

        //Front section of roof
        glBegin(GL_TRIANGLE_FAN);
            glNormal3d(0,0,-1);
            glTexCoord2f(0,0);
            glVertex3f(0,0,35);
            for(int k=0;k<=180;k+=10){
                glTexCoord2f(6*-Cos(k),6*Sin(k));
                glVertex3f(25*Cos(k),7*Sin(k),35);
            }
        glEnd();

        glColor3f(0.5,0.5,0.5);
        //  Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);

        glBindTexture(GL_TEXTURE_2D, texture[4]);
    
        //Top cover (roof) of hangar
        glBegin(GL_QUAD_STRIP);
            glTexCoord2f(0,0);
            glNormal3d(0,-1,0);
            for(int k=0;k<=180;k+=10){
                glTexCoord2f(0.1*k,0);glVertex3f(25*Cos(k),7*Sin(k),0);
                glTexCoord2f(0.1*k,1);glVertex3f(25*Cos(k),7*Sin(k),35);
            }
        glEnd();
        //Disable blending
        glDisable(GL_BLEND);

        //  Undo transformations
        glPopMatrix();
    }

    void centerRect(double x, double z){
        glColor3d(1,1,1);
        //  Save transformation
        glPushMatrix();
        glTranslated(x-4.5,0,z-5.0);

        glBegin(GL_POLYGON);
            glNormal3d(0,1,0);
            glTexCoord2f(0,0);glVertex3d(0,0,0);
            glTexCoord2f(0,4);glVertex3d(0,0,9);
            glTexCoord2f(5,4);glVertex3d(10,0,9);
            glTexCoord2f(5,0);glVertex3d(10,0,0);
        glEnd();

        //  Undo transofrmations
        glPopMatrix();
    }

    /*
    nx Z- Direction of the z componenet in normal vector
    */
    void hangarLengthWall(double x, double y, double z, double nZ){
        glColor3d(1,1,1);
        //  Save transformation
        glPushMatrix();
        glTranslated(x-25,y,z);

        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glBegin(GL_POLYGON);
            glNormal3d(0,0,nZ);
            glTexCoord2f(0,0);glVertex3d(0,0,0);
            glTexCoord2f(0,15);glVertex3d(50,0,0);
            glTexCoord2f(7,15);glVertex3d(50,10,0);
            glTexCoord2f(7,0);glVertex3d(0,10,0);
        glEnd();

        //  Undo transofrmations
        glPopMatrix();
    }

    /*
    nx - Direction of the x componenet in normal vector
    */
    void hangarWidthWall(double x, double y, double z, double nX){
        glColor3d(1,1,1);
        //  Save transformation
        glPushMatrix();
        glTranslated(x,y,z-17.5);
        glBindTexture(GL_TEXTURE_2D, texture[2]);

        glBegin(GL_POLYGON);
            glNormal3d(nX,0,0);
            glTexCoord2f(0,0);glVertex3d(0,0,0);
            glTexCoord2f(0,7);glVertex3d(0,0,35);
            glTexCoord2f(6,7);glVertex3d(0,10,35);
            glTexCoord2f(6,0);glVertex3d(0,10,0);
        glEnd();
        //  Undo transofrmations
        glPopMatrix();
    }

    void hangarFloorSquare(double x, double z){
        glColor3d(1,1,1);
        //  Save transformation
        glPushMatrix();
        glTranslated(x-3.5,0,z-3.5);
   
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBegin(GL_POLYGON);
            glNormal3d(0,1,0);
            glTexCoord2f(0,0);glVertex3d(0,0,0);
            glTexCoord2f(0,7);glVertex3d(0,0,7);
            glTexCoord2f(7,7);glVertex3d(7,0,7);
            glTexCoord2f(7,0);glVertex3d(7,0,0);
        glEnd();

        //  Undo transofrmations
        glPopMatrix();
    }

    void hangarFloor(double x, double z){
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);

        glColor3d(1,1,1);
        //  Save transformation
        glPushMatrix();
        glTranslated(x,0,z);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBegin(GL_POLYGON);
            glNormal3d(0,1,0);
            glTexCoord2f(0,0);glVertex3d(0,0,0);
            glTexCoord2f(0,20);glVertex3d(0,0,35);
            glTexCoord2f(20,20);glVertex3d(50,0,35);
            glTexCoord2f(0,20);glVertex3d(50,0,0);
        glEnd();

        //  Undo transofrmations
        glPopMatrix();
        //  Disable polygon offset
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    void hangarFloorRectangle(double x, double z){
        glColor3d(1,1,1);
        //  Save transformation
        glPushMatrix();
        glTranslated(x-4.0,0,z-3.0);

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBegin(GL_POLYGON);
            glNormal3d(0,1,0);
            glTexCoord2f(0,0);glVertex3d(0,0,0);
            glTexCoord2f(0,3);glVertex3d(0,0,6);
            glTexCoord2f(5,3);glVertex3d(8,0,6);
            glTexCoord2f(5,0);glVertex3d(8,0,0);
        glEnd();

        //  Undo transofrmations
        glPopMatrix();
    }
};