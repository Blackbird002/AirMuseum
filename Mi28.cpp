#include "CSCIx229.h"

class Mi28{
public:
    int mode = 1;
    float emission = 0;
    Mi28(){
        //  Load object
        obj = LoadOBJ("Mi28.obj");
    }

    void drawMi28(double x,double y,double z, double scale, double thx, double thy, double thz){
        //  Enable textures
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

        glPushMatrix();
        glTranslated(x,y,z);
        glScaled(scale,scale,scale);
        glRotated(thx,1,0,0);
        glRotated(thy,0,1,0);
        glRotated(thz,0.50,0,0);
        glCallList(obj);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }

private:
    int obj;          //  Object display list
};
