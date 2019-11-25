#include "CSCIx229.h"
#include <iostream>

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
    bool movementRestriction;

    double xMax,xMin,zMin,zMax;

    Camera(){
        this->cameraX = 50.0;
        this->cameraY = 10.0;
        this->cameraZ = 25.0;
    }

    Camera(double x, double y, double z, double dim, double hangarScale, bool restriction){
        this->cameraX = x;
        this->cameraY = y;
        this->cameraZ = z;
        this->dim = dim;
        this->scale = hangarScale;

        xMax = scale * 49;
        xMin = scale * 1;
        zMax = scale * 34;
        zMin = scale * 1;
        movementRestriction = restriction;
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

        double newCamX = cameraX + moveUnits*crossX;
        double newCamZ = cameraZ + moveUnits*crossZ;

        //Add cross vector to camera position vector...

        //Prevent from leaving hangar in X direction
        if((cameraX + moveUnits*crossX) < xMax && (cameraX + moveUnits*crossX > xMin) && checkBounds(newCamX,newCamZ))
            cameraX += moveUnits*crossX;
        
        //Prevent from leaving hangar in Z direction
        if((cameraZ + moveUnits*crossZ) < zMax && (cameraZ + moveUnits*crossZ > zMin) && checkBounds(newCamX,newCamZ))
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

        double newCamX = cameraX + moveUnits*crossX;
        double newCamZ = cameraZ + moveUnits*crossZ;

        //Add cross vector to camera position vector 
        
        //Prevent from leaving hangar in X direction
        if((cameraX + moveUnits*crossX) < xMax && (cameraX + moveUnits*crossX > xMin) && checkBounds(newCamX,newCamZ))
            cameraX += moveUnits*crossX;
        
        //Prevent from leaving hangar in Z direction
        if((cameraZ + moveUnits*crossZ) < zMax && (cameraZ + moveUnits*crossZ > zMin) && checkBounds(newCamX,newCamZ))
            cameraZ += moveUnits*crossZ;
    }

    void forwardMovement(double bX, double bY, double bZ, double moveUnits){
        //Calculate unit vector of lookAt vector:
        double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
        bX /= mag;
        bY /= mag;
        bZ /= mag;

        double newCamX = cameraX + moveUnits*bX;
        double newCamZ = cameraZ + moveUnits*bZ;
        
        //Prevent from leaving hangar in X direction
        if((cameraX + moveUnits*bX) < xMax && (cameraX + moveUnits*bX > xMin) && checkBounds(newCamX,newCamZ))
            cameraX += moveUnits*bX;
        
        //Prevent from leaving hangar in Z direction
        if((cameraZ + moveUnits*bZ) < zMax && (cameraZ + moveUnits*bZ > zMin) && checkBounds(newCamX,newCamZ))
            cameraZ += moveUnits*bZ;
    }

    void backwardMovement(double bX, double bY, double bZ, double moveUnits){
        //Calculate unit vector of lookAt vector:
        double mag = sqrt(bX*bX + bY*bY + bZ*bZ);
        bX /= mag;
        bY /= mag;
        bZ /= mag;

        double newCamX = cameraX - moveUnits*bX;
        double newCamZ = cameraZ - moveUnits*bZ;

        //Prevent from leaving hangar in X direction
        if((cameraX - moveUnits*bX) < xMax && (cameraX - moveUnits*bX > xMin) && checkBounds(newCamX,newCamZ))
            cameraX -= moveUnits*bX;
        
        //Prevent from leaving hangar in Z direction
        if((cameraZ - moveUnits*bZ) < zMax && (cameraZ - moveUnits*bZ > zMin) && checkBounds(newCamX,newCamZ))
            cameraZ -= moveUnits*bZ;
    }

    bool checkBounds(double newX, double newZ){
        bool result = true;

        if(movementRestriction)
            return result;

        //Bottom left square
        if(newX >= 4*scale && newX <= 11*scale && newZ >= 4*scale && newZ <=11*scale){result = false;}
        //Bottom right square
        if(newX >= 4*scale && newX <= 11*scale && newZ >= 24*scale && newZ <=31*scale){result = false;}
        //Top left square
        if(newX >= 39*scale && newX <= 46*scale && newZ >= 4*scale && newZ <=11*scale){result = false;}
        //Top right square
        if(newX >= 39*scale && newX <= 46*scale && newZ >= 24*scale && newZ <=31*scale){result = false;}

        //Left rectangle
        if(newX >= 17*scale && newX <= 33*scale && newZ >= 3*scale && newZ <=9*scale){result = false;}
        //Right rectangle
        if(newX >= 17*scale && newX <= 33*scale && newZ >= 26*scale && newZ <=32*scale){result = false;}
        //Center rectangle
        if(newX >= 15*scale && newX <= 35*scale && newZ >= 13*scale && newZ <=22*scale){result = false;}
        
        return result;
    }  
};