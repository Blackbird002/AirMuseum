#include "CSCIx229.h"

class F16{
public:
    int mode = 1;
    float emission = 0;
    F16(){
        //  Load object
        obj = LoadOBJ("f16.obj");
    }

    void drawF16(double x,double y,double z, double scale, double thx, double thy, double thz, bool landingGear){
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

        if(landingGear){
            drawLandingGearOneTire(0.35,-0.11,0,3.8,0.05);
            drawLandingGearOneTireAngled(-0.22,-0.11,-0.075,3.8,0.05,-2);
            drawLandingGearOneTireAngled(-0.22,-0.11,0.075,3.8,0.05,2);
        }

        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

private:
    int obj;          //  Object display list     
};