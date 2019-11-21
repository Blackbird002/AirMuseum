#include "CSCIx229.h"

class Camera{

public:
    // What the camera is looking at vector
    double cameraLookX , cameraLookY , cameraLookZ;

    // XZ position of the camera in 1st person (eye)
    double cameraX=50, cameraY =10 , cameraZ=25;

    double dim=200;   // Dimension of orthogonal box

    double th=321;         //  Azimuth of view angle (y)
    double ph=29;         //  Elevation of view angle (x)

    double scale;

    Camera(){
        this->cameraX = 50.0;
        this->cameraY = 10.0;
        this->cameraZ = 25.0;
    }

    Camera(double x, double y, double z, double dim, double hangarScale){
        this->cameraX = x;
        this->cameraY = y;
        this->cameraZ = z;
        this->dim = dim;
        this->scale = hangarScale;
    }
    
    double getDim(){return dim;}
    double getTh(){return th;}
    double getPh(){return ph;}
    

    void firstPerson(){
        //First person
        cameraLookX = 2*dim*Sin(th);
        cameraLookZ = -2*dim*Cos(th);
        cameraLookY = 2*dim*Sin(ph);

        gluLookAt(cameraX,cameraY,cameraZ,
                cameraX+cameraLookX,cameraY + cameraLookY,cameraZ+cameraLookZ,
                0,1,0); 
    }

    void perspectiveMode(){
        //  Perspective - set eye position
        double Ex = -2*(dim/2)*Sin(th)*Cos(ph);
        double Ey = +2*(dim/2)        *Sin(ph);
        double Ez = +2*(dim/2)*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }

    void moveForward(){
        forwardMovement(cameraLookX, cameraLookY, cameraLookZ, 2);
    }

    void moveBackward(){
        backwardMovement(cameraLookX,cameraLookY, cameraLookZ, 2);  
    }

    void strafeLeft(){
        leftStrafeMovement(cameraLookX, cameraLookY, cameraLookZ, 2);
    }

    void strafeRight(){
        rightStrafeMovement(cameraLookX, cameraLookY, cameraLookZ, 2);
    }

    bool outOfBounds(){
        bool result = false;
        if(cameraX <= (1*scale) || cameraX >= (49*scale))
            result = true;
        
        if(cameraZ <= (1*scale) || cameraZ >= (34*scale))
            result = true;

        return result;
    }

    private:

    void leftStrafeMovement(double bX, double bY, double bZ, double moveUnits){
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

    void rightStrafeMovement(double bX, double bY, double bZ, double moveUnits){
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

    void forwardMovement(double bX, double bY, double bZ, double moveUnits){
        //Calculate unit vector of lookAt vector:
        double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
        bX /= mag;
        bY /= mag;
        bZ /= mag;

        cameraX += moveUnits*bX;
        cameraZ += moveUnits*bZ;
    }

    void backwardMovement(double bX, double bY, double bZ, double moveUnits){
        //Calculate unit vector of lookAt vector:
        double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
        bX /= mag;
        bY /= mag;
        bZ /= mag;

        cameraX -= moveUnits*bX;
        cameraZ -= moveUnits*bZ;
    }

    
};