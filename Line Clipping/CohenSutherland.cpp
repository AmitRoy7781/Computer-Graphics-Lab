#ifdef _WIN32
#include <windows.h>
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLUT32.lib")
#pragma warning(disable:4244)
#pragma warning(disable:4305)
#endif

#include<bits/stdc++.h>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define  PI acos(-1.0)
#include <GL/glut.h>

#define PDD pair<double,double>

using namespace std;

int Wi=640, He=480;

bool colored = false;
int boundary = 150;

int Xmin = -boundary;
int Xmax = boundary;
int Ymin = -boundary;
int Ymax = boundary;


const int LEFT      = 1;
const int RIGHT     = 2;
const int BOTTOM    = 4;
const int TOP       = 8;


void myInit (void);
void display(void);
void reshape (int w, int h);


void myInit (void)
{
    glClearColor(0.0, 0.0, 0.0,1.0);
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (-Wi/2, Wi/2-1, -He/2, He/2-1); // size of projection plane (origin at the center)
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

void GridDraw()
{
    glBegin(GL_LINES);

    glColor3f(1.0,1.0,1.0);
    // glVertex2i(-320,0);
    // glVertex2i(319,0);
    // glVertex2i(0,239);
    // glVertex2i(0,-240);

    glVertex2i(boundary,He/2-1);
    glVertex2i(boundary,-He/2);

    glVertex2i(Wi/2-1,-boundary);
    glVertex2i(-Wi/2,-boundary);

    glVertex2i(-boundary,He/2-1);
    glVertex2i(-boundary,-He/2);    

    glVertex2i(Wi/2-1,boundary);
    glVertex2i(-Wi/2,boundary);

    

    

    glEnd();

}
void drawPixel(int x,int y)
{
    glVertex2i(x,y);
}

void drawPixelZone(int x,int y,int zone)
{
    if(zone==0)
    {
        if(colored)
            glColor3f(1.0, 0.0, 0.0);///Red
        drawPixel(x,y);
    }
    else if(zone==1)
    {
        if(colored)
            glColor3f(0.0,1.0,0.0);///Green
        drawPixel(y,x);
    }
    else if(zone==2)
    {
        if(colored)
            glColor3f(0.0,0.0,1.0);///Blue
        drawPixel(-y,x);
    }
    else if(zone==3)
    {
        if(colored)
            glColor3f(1.0,1.0,0.0);///Yellow
        drawPixel(-x,y);
    }
    else if(zone==4)
    {
        if(colored)
            glColor3f(1.0,0.1,1.0);///Purple
        drawPixel(-x,-y);
    }
    else if(zone==5)
    {
        if(colored)
            glColor3f(0,1.0,1.0);///Cyan
        drawPixel(-y,-x);
    }
    else if(zone==6)
    {
        if(colored)
            glColor3f(1.0,1.0,1.0);///White
        drawPixel(y,-x);
    }
    else if(zone==7)
    {
        if(colored)
            glColor3f(1.0,0.5,0.0);///Orange
        drawPixel(x,-y);
    }
}

void drawLine0(int x,int y,int x0,int y0,int zone)
{
    int dx = x0-x, dy = y0-y;
    int d = 2*dy - dx;
    int dE = 2*dy, dNE = 2*(dy-dx);

    drawPixelZone(x,y,zone);

    while(x<x0)
    {
        if(d>0)
        {
            x++;
            y++;
            d+=dNE;
        }
        else
        {
            x++;
            d+=dE;
        }
        drawPixelZone(x,y,zone);
    }

}


int zoneDetection(int x0,int y0,int x1,int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    if(dx>=0 && dy>=0)
    {
        if(dx>=dy)      return 0;
        else    return 1;
    }
    else if(dx<0 && dy>=0)
    {
        if(dy>=abs(dx))     return 2;
        else    return 3;

    }
    else if(dx<0 && dy<0)
    {
        if(abs(dx)>=abs(dy))    return 4;
        else    return 5;
    }
    else if(dx>=0 && dy<0)
    {
        if(abs(dy)>=dx)     return 6;
        else    return 7;
    }
}

void drawLine(int x0,int y0,int x1,int y1)
{
    int zone = zoneDetection(x0,y0,x1,y1);
    if(zone==0)
        drawLine0(x0,y0,x1,y1,zone);
    else if(zone==1)
        drawLine0(y0,x0,y1,x1,zone);
    else if(zone==2)
        drawLine0(y0,-x0,y1,-x1,zone);
    else if(zone==3)
        drawLine0(-x0,y0,-x1,y1,zone);
    else if(zone==4)
        drawLine0(-x0,-y0,-x1,-y1,zone);
    else if(zone==5)
        drawLine0(-y0,-x0,-y1,-x1,zone);
    else if(zone==6)
        drawLine0(-y0,x0,-y1,x1,zone);
    else if(zone==7)
        drawLine0(x0,-y0,x1,-y1,zone);
}


int makeCode(double x,double y)
{
    int code = 0;
    
    if(y>Ymax)
        code |= TOP;
    else if(y<Ymin)
        code |= BOTTOM;

    if(x>Xmax)
        code |= RIGHT;
    else if(x<Xmin)
        code |= LEFT;

    return code;
}

void cohen_Sutherland(double x0,double y0,double x1,double y1)
{
    // glPointSize(3);
    // glColor3f(1.0,1.0,1.0);
    // glBegin(GL_POINTS);
    // glVertex2i(x0,y0);
    // glVertex2i(x1,y1);
    // glEnd();
    // glPointSize(1); 
    glBegin(GL_POINTS);
    
    int code0,code1,code;
    double x,y;

    double x0_ = x0;
    double x1_ = x1;
    double y0_ = y0;
    double y1_ = y1;

    code0 = makeCode(x0,y0);
    code1 = makeCode(x1,y1);

    while(true)
    {
        
        if ((code0 == 0) && (code1 == 0)) //fully accepted
        {

            glEnd();
            glPointSize(4);
            glColor3f(1.0,1.0,1.0);
            glBegin(GL_POINTS);
            glVertex2i(x0,y0);
            glVertex2i(x1,y1);
            glEnd();
            glPointSize(1); 
            glBegin(GL_POINTS);

            glColor3f(1.0,1.0,0.0);///Yellow
            drawLine(x0,y0,x1,y1);            
            break;
            //exit();
        }
        else if (code0 & code1)//fully rejected
        {

            glColor3f(1.0, 0.0, 0.0);///Red
            drawLine(x0,y0,x1,y1);
            break;
            //exit();
        }
        else //partial
        {
            int code; 
            double x, y; 

            if (code0) 
                code = code0; 
            else
                code = code1; 
  
            if (code & TOP) 
            { 
                
                x = x0 + (x1 - x0) * (Ymax - y0) / (y1 - y0); 
                y = Ymax; 
            } 
            else if (code & BOTTOM) 
            { 
                x = x0 + (x1 - x0) * (Ymin - y0) / (y1 - y0); 
                y = Ymin; 
            } 
            else if (code & RIGHT) 
            { 
                y = y0 + (y1 - y0) * (Xmax - x0) / (x1 - x0); 
                x = Xmax; 
            } 
            else if (code & LEFT) 
            { 
                y = y0 + (y1 - y0) * (Xmin - x0) / (x1 - x0); 
                x = Xmin; 
            } 



            if(code == code0)
            {
                glColor3f(0.5, 0.5, 0.5);
                drawLine(x0,y0,x,y);
                x0 = x;
                y0 = y;
                code0 = makeCode(x0,y0);
            }
            else
            {
                glColor3f(0.5, 0.5, 0.5);
                drawLine(x1,y1,x,y);
                x1 = x;
                y1 = y;
                code1 = makeCode(x1,y1);   
            }
  
        
        }//partial
    
    }//while loop
    glEnd();

}//end of function


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    GridDraw();
    //glBegin(GL_POINTS);


    int x0 = -200;
    int y0 = -200;
    int x1 = 200;
    int y1 = 200;

    // drawPixel(x0,y0);
    // drawPixel(x1,y1);

    srand(time(NULL));

    int numberOfLines = 100;
    for(int i=0; i<=numberOfLines; i++)
    {
        int x0 = -320 + rand()%640;
        int y0 = -240 + rand()%480;
        int x1 = -320 + rand()%640;
        int y1 = -240 + rand()%480;
        cohen_Sutherland(x0,y0,x1,y1);
    }
    
    
    //glEnd();
    glFlush();
}


int main (int argc, char **argv)
{

    glutInit (&argc, argv); // to initialize the toolkit;
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // sets the display mode
    glutInitWindowSize (Wi, He); // sets the window size
    glutInitWindowPosition (0, 0); // sets the starting position for the window
    glutCreateWindow ("Graphics Lab 2"); // creates the window and sets the title
    myInit(); // additional initializations as necessary
    glutReshapeFunc(reshape);
    glutDisplayFunc (display);
    glutMainLoop(); // go into a loop until event occurs
    return 0;
}
