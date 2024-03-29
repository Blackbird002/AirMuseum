#include "CSCIx229.h"

class UH60{
public:
    int mode = 1;
    float emission = 0;
    UH60(){
        //  Load object
        obj = LoadOBJ("../Objects/uh60/uh60.obj");
    }

    void drawuh60(double x,double y,double z, double scale, double thx, double thy, double thz){
        //  Enable textures
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

        glPushMatrix();
        glTranslated(x,y,z);
        glScaled(scale,scale,scale);
        glRotated(thx,1,0,0);
        glRotated(thy,0,1,0);
        glRotated(thz,0,0,1);
        glCallList(obj);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }

private:
    int obj;          //  Object display list
};