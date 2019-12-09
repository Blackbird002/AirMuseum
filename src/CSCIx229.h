#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) (float)cos(3.1415926/180*(th))
#define Sin(th) (float)sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim, int projectionMode);
void ErrCheck(const char* where);
void findDispVector(double x1, double y1, double z1, double x2, double y2, double z2, double* vX, double* vY, double* vZ);
void findNormalVector(double x1, double y1, double z1, double x2, double y2, double z2, double* uX, double* uY, double* uZ);
void Vertex(int th,int ph);
void Sphere(double x,double y,double z,double r, float emission, float shiny);
void drawAxisLines();
void drawAxisLabels();
int LoadOBJ(const char* file);
void ball(double x,double y,double z,double r, float emission, float shiny, int inc);
void engineSphere(double x,double y,double z,double r, double yRot, float emission, float shiny);
void disk(double x,double y,double z,double r, double th);
void hangarFloorSquare(double x, double z);
void hangarFloor(double x, double z);
void hangarFloorRectangle(double x, double z);
void planeTire(double x,double y,double z,double r);
void drawLandingGear(double x,double y,double z, double h, double s);
void drawLandingGearOneTire(double x,double y,double z, double h, double s);
void drawLandingGearOneTireAngled(double x,double y,double z, double h, double s, double zOffset);
void PrintShaderLog(int obj,char* file);
void PrintProgramLog(int obj);
int CreateShader(GLenum type,char* file);
int CreateShaderProg(char* VertFile,char* FragFile);
void ShadowProjection(float L[4], float E[4], float N[4]);
void Cylinder(float x,float y,float z ,float thx,float thy ,float r,float h);
char* ReadText(char *file);
void drawBoundaryPolesSquare(double x,double y,double z, double h, int shader[]);
void drawBoundaryPolesRectangle(double x,double y,double z, double h, int shader[]);
void drawBoundaryPolesCenter(double x,double y,double z, double h, int shader[]);
void drawBoundaryRope(double x,double y,double z, double thY, double xOffset, double yOffset);
void drawLandingGearRearBomber(double x,double y,double z, double h, double s);
void skyboxCube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, float emission, float shiny, unsigned int texture[]);

#ifdef __cplusplus
}
#endif

#endif
