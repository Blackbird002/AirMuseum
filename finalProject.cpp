#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include "CSCIx229.h"
#include "Camera.cpp"
#include "Hangar.cpp"
#include "XB70Bomber.cpp"
#include "FighterJet.cpp"
#include "MQ9.cpp"
#include "UH60.cpp"
#include "F16.cpp"
#include <iostream>
#include <cstring>

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
bool drawAxis = true;
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double scale = 15;

/*
  1 - First person 
  2 - Perspective (starts in perspective mode)
*/
int projectionMode = 2;     

double THX;
double THZ;

unsigned int texture[12]; // Texture names

double previousMouseY;
double previousMouseX;

Camera* camera;
Hangar* hangar;
XB70Bomber* bomber;
FighterJet* myJet;
MQ9* mq9;
UH60* uh60;
F16* f16;

//Light values
int one       =   1;  // Unit value
int distance  =   180;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
float emission  =   0;  // Emission intensity (%)
float ambient   =  10;  // Ambient intensity (%)
float highAmbient   =  40;  // Ambient intensity (%)
float diffuse   = 100;  // Diffuse intensity (%)
int specular  =   5;  // Specular intensity (%)
int shininess =   64;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
int th        = 90;
float ylight  =   80;  // Elevation of light
bool light = true;

int shader[10] = {0,0};

float Ambient[]   = {0.01f*ambient ,0.01f*ambient ,0.01f*ambient ,1.0f};
float AmbientHigh[]   = {0.01f*highAmbient ,0.01f*highAmbient ,0.01f*highAmbient ,1.0f};

#define Yfloor 0.01  // Y position 
float N[] = {0, -1, 0}; // Normal vector for the plane
float E[] = {0, Yfloor, 0}; // Point of the plane

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void reshape(int,int);
void init();
void mouseCallback(GLFWwindow *window, double x, double y);
void key(GLFWwindow* window,int key,int scancode,int action,int mods);
void error(int error, const char* text);
void centerWindow(GLFWwindow *window, GLFWmonitor *monitor);
void drawScene();
void generateShadow(float Position[]);
// ----------------------------------------------------------
// key() Callback function
// ----------------------------------------------------------
void key(GLFWwindow* window,int key,int scancode,int action,int mods){
  //Discard key releases (keeps PRESS and REPEAT)
  if (action==GLFW_RELEASE) return;

  //Check for shift
  //int shift = (mods & GLFW_MOD_SHIFT);

  //Exit on ESC
  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(window,1);
  else if (key==GLFW_KEY_RIGHT)
    camera->th += 2;
  else if (key==GLFW_KEY_LEFT)
    camera->th -= 2;
  //  Increase/decrease elevation
  else if (key==GLFW_KEY_UP)
    camera->ph += 2;
  else if (key==GLFW_KEY_DOWN)
    camera->ph -= 2;

  //Moving camera only in First person & FOV
  if (projectionMode == 1){
    if(key == GLFW_KEY_W )
      camera->moveForward();
    else if(key == GLFW_KEY_S)
      camera->moveBackward();  
    else if(key == GLFW_KEY_A )
      camera->strafeLeft();
    else if(key == GLFW_KEY_D )
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

/*
 * GLFW errors
 */
void error(int error, const char* text)
{
  fprintf(stderr,"Error: %s\n",text);
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
// display() Callback function
// ----------------------------------------------------------
void display(GLFWwindow* window){

  //  Clear screen and Z-buffer
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

  //  Undo previous transformations
  glLoadIdentity();
  
  if(projectionMode == 1){
    camera->firstPerson();
  }else if(projectionMode == 2){
    camera->perspectiveMode();
  }

  //Translate intensity to color vectors
  float Diffuse[]   = {0.01f*diffuse ,0.01f*diffuse ,0.01f*diffuse ,1.0f};
  float Specular[]  = {0.01f*specular,0.01f*specular,0.01f*specular,1.0f};
  //Light position
  float Position[]  = {350+distance*Cos(zh),ylight,250+distance*Sin(zh),1.0};
  //Draw light position as ball (still no lighting here)
  glColor3f(1,1,1);
  ball(Position[0],Position[1],Position[2] ,0.5,emission,shiny,inc);
  //OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  //Enable lighting
  glEnable(GL_LIGHTING);
  //Location of viewer for specular calculations
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
  //glColor sets ambient and diffuse color materials
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  //Enable light 0
  glEnable(GL_LIGHT0);
  //Set ambient, diffuse, specular components and position of light 0

  //Spot dir, cutoff, exponent
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);

  glLightfv(GL_LIGHT0,GL_AMBIENT , AmbientHigh);
  //skyboxCube(200,0,200,900,900,900,0, 0, 0, texture);  
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);

  glDisable(GL_LIGHTING);
  if(drawAxis){
    drawAxisLines();
    drawAxisLabels();
  }
  glEnable(GL_LIGHTING);

  //Draw two flying jets
  //myJet->drawFighterJet(500*Cos(th)+350,450,500*Sin(th)+250,-Sin(th),0,Cos(th),0,1,0,4,50,0,false);
  //myJet->drawFighterJet(550*Cos(th)+350,600,550*Sin(th)+250,-Sin(th),0,Cos(th),0,1,0,4,50,0,false);

  //Draw only the hangar floor
  //hangar->drawHangarFloor(25,0,17.5,scale);

  //drawScene();
  myJet->drawFighterJet(0,0,0,1,0,0,0,1,0,4,0,0,false);

  //generateShadow(Position);

  //Text Display
  glColor3f(0,1,0);
  glWindowPos2i(5,5);
  glWindowPos2i(5,65);

  glWindowPos2i(5,85);
  if(drawAxis == true)
    Print("Axis ON");
  else
    Print("Axis OFF");

  glWindowPos2i(5,105);
  Print("Camera Position: [%.1f ,%.1f ,%.1f]", camera->cameraX, camera->cameraY, camera->cameraZ);

  //Render the scene and make it visible
  ErrCheck("display");
  glFlush();
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void reshape(GLFWwindow* window,int width,int height)
{
  //Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //Set the viewport to the entire window
  glViewport(0,0, width,height);
  //Set projection
  Project(fov, asp, camera->dim, projectionMode);
}

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){

  int width,height;
  GLFWwindow* window;

  camera = new Camera(550,13,250,200,15,false);

  //Initialize GLFW
  if (!glfwInit()) Fatal("Cannot initialize glfw\n");

  //Error callback
  glfwSetErrorCallback(error);

  //Set window properties
  glfwWindowHint(GLFW_RESIZABLE,1);
  glfwWindowHint(GLFW_DOUBLEBUFFER,1);

  //  Create window and make current
  window = glfwCreateWindow(1280,720, "Riad Shash (Ray) Final Project - GLFW", nullptr, nullptr);
  if (!window) Fatal("Cannot create GLFW window\n");

  //Center the GLFW window
  centerWindow(window, glfwGetPrimaryMonitor());
  
  glfwMakeContextCurrent(window);

  //Enable VSYNC
  glfwSwapInterval(1);

  //Set callback for window resize
  glfwSetWindowSizeCallback(window,reshape);

  //Set initial window size
  glfwGetWindowSize(window,&width,&height);

  reshape(window,width,height);

  //GLFW does this automatically sets up sentcil buffer!

  //Callbacks
  glfwSetKeyCallback(window,key);
  glfwSetCursorPosCallback(window,mouseCallback);

  //Removes the mouse cursor for a more immersive experience
  if (argc > 1) glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

  //Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);

  //Load textures
  texture[0] = LoadTexBMP("Textures/WhiteMetal.bmp");
  texture[1] = LoadTexBMP("Textures/MetalUs.bmp");
  texture[2] = LoadTexBMP("Textures/al.bmp");
  texture[3] = LoadTexBMP("Textures/glass.bmp");
  texture[4] = LoadTexBMP("Textures/engineTexture.bmp");

  //For the skybox
  texture[5] = LoadTexBMP("Textures/front.bmp");
  texture[6] = LoadTexBMP("Textures/left.bmp");
  texture[7] = LoadTexBMP("Textures/right.bmp");
  texture[8] = LoadTexBMP("Textures/top.bmp");
  texture[10] = LoadTexBMP("Textures/back.bmp");

  texture[11] = LoadTexBMP("Textures/imageBurner.bmp");

  //Load shaders
  char pixVert[] = "Shaders/pixtex.vert";
  char pixFrag[] = "Shaders/pixtex.frag";
  shader[0] = CreateShaderProg(pixVert,pixFrag);
  mq9 = new MQ9();
  hangar = new Hangar(shader,10);
  bomber = new XB70Bomber();
  myJet = new FighterJet();
  uh60 = new UH60();
  f16 = new F16();

  //Main event loop
  ErrCheck("init");
  while(!glfwWindowShouldClose(window))
  {
    //  Elapsed time in seconds
    double t = glfwGetTime();
    zh = fmod(30*t,360.0);
    th = fmod(45*t,360.0);
    //  Display
    display(window);
    //  Process any events
    glfwPollEvents();
  }

  //Shut down GLFW
  glfwDestroyWindow(window);
  glfwTerminate();

  delete camera;
  delete bomber;
  delete hangar;
  delete myJet;
  delete uh60;
  delete mq9;
  delete f16;

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

void drawScene(){
  hangar->drawHangar(25,0,17.5,scale);

  
  bomber->drawBomber(450,200,350, 1,0,1, 0,1,0, 3.5, 0, 15, false);
  myJet->drawFighterJet(450,150,400,1,0,1,0,1,0,4,0,15,false);
  
  myJet->drawFighterJet(125,15.75,145,1,0,1,0,1,0,4,0,0, true);
  myJet->drawFighterJet(615,15.75,390,-1,0,-1,0,1,0,4,0,0,true);

  glUseProgram(shader[0]);
  mq9->drawMQ9(112,15,412,350,90,180,-25);
  uh60->drawuh60(420,12.3,440,8,90,180,90);
  bomber->drawBomber(380,31,250, 1,0,0, 0,1,0, 3.5, 0, 0, true);
  f16->drawF16(637.5,19,112.5,55,0,220,0,true);
  f16->drawF16(262,150,262,50,0,-40,20,false);
  glUseProgram(0);
}

void generateShadow(float Position[]){
  //Save what is glEnabled
  glPushAttrib(GL_ENABLE_BIT);
   
  glDisable(GL_LIGHTING);

  //Enable stencil operations
  glEnable(GL_STENCIL_TEST);

  // ----------------------------------------------------------
  // Step 1:  Set stencil buffer to 1 where there are shadows
  // ----------------------------------------------------------

  //Existing value of stencil buffer doesn't matter
  glStencilFunc(GL_ALWAYS,1,0xFFFFFFFF);
  //Set the value to 1 (REF=1 in StencilFunc)
  //only if Z-buffer would allow write
  glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
  //Make Z-buffer and color buffer read-only (prevent update)
  glDepthMask(0);
  glColorMask(0,0,0,0);
  //Draw flattened scene
  glPushMatrix();
  ShadowProjection(Position,E,N);
  drawScene();
  glPopMatrix();
  //Make Z-buffer and color buffer read-write
  glDepthMask(1);
  glColorMask(1,1,1,1);

  // ----------------------------------------------------------
  //  Step 2:  Draw shadow masked by stencil buffer
  // ----------------------------------------------------------

  //Set the stencil test draw where stencil buffer is > 0
  glStencilFunc(GL_LESS,0,0xFFFFFFFF);

  //Make the stencil buffer read-only
  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

  //Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0,0,0,0.5);

  //  Draw the shadow over the entire hangar floor
  glBegin(GL_QUADS);
    glVertex3f(50*scale,Yfloor,0);
    glVertex3f(50*scale,Yfloor,35*scale);
    glVertex3f(0,Yfloor,35*scale);
    glVertex3f(0,Yfloor,0);
  glEnd();
    
  //  Undo glEnables
  glPopAttrib();
}
