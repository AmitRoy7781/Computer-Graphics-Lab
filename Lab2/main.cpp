#ifdef _WIN32
#include <windows.h>
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLUT32.lib")
#pragma warning(disable:4244)
#pragma warning(disable:4305)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>
#define  PI acos(-1.0)

using namespace std;

int Wi=640, He=480;


void myInit (void);
void display(void);
void reshape (int w, int h);

void GridDraw(void);
void drawPixel(int x,int y);
void draw_line0(int x,int y,int x0,int y0);
int zoneDetection(int x0,int y0,int x1,int y1);
void drawLine(int x0,int y0,int x1,int y1);

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


void GridDraw(void)
{
    glBegin(GL_LINES);

    glColor3f(1.0,1.0,1.0);
    glVertex2i(-320,0);
    glVertex2i(319,0);
    glVertex2i(0,239);
    glVertex2i(0,-240);


    glEnd();

}
void drawPixel2(int cx,int cy,int x,int y)
{
    glVertex2i(cx+x,cy+y);
}

void drawPixelZone(int cx,int cy,int x,int y,int zone)
{
    if(zone==0)
    {
        glColor3f(1.0, 0.0, 0.0);///Red
        drawPixel2(cx,cy,x,y);
    }
    else if(zone==1)
    {
        glColor3f(0.0,1.0,0.0);///Green
        drawPixel2(cx,cy,y,x);
    }
    else if(zone==2)
    {
        glColor3f(0.0,0.0,1.0);///Blue
        drawPixel2(cx,cy,-y,x);
    }
    else if(zone==3)
    {
        glColor3f(1.0,1.0,0.0);///Yellow
        drawPixel2(cx,cy,-x,y);
    }
    else if(zone==4)
    {
        glColor3f(1.0,0.1,1.0);///Purple
        drawPixel2(cx,cy,-x,-y);
    }
    else if(zone==5)
    {
        glColor3f(0,1.0,1.0);///Cyan
        drawPixel2(cx,cy,-y,-x);
    }
    else if(zone==6)
    {
        glColor3f(1.0,1.0,1.0);///White
        drawPixel2(cx,cy,y,-x);
    }
    else if(zone==7)
    {

        glColor3f(1.0,0.5,0.0);///Orange
        drawPixel2(cx,cy,x,-y);
    }
}


void draw8Way(int cx,int cy,int x,int y)
{
    drawPixelZone(cx,cy,x,y,0);
    drawPixelZone(cx,cy,x,y,1);
    drawPixelZone(cx,cy,x,y,2);
    drawPixelZone(cx,cy,x,y,3);
    drawPixelZone(cx,cy,x,y,4);
    drawPixelZone(cx,cy,x,y,5);
    drawPixelZone(cx,cy,x,y,6);
    drawPixelZone(cx,cy,x,y,7);    
}

void circleDrawing(int cx,int cy,int r)
{
    int d = 5 - 4*r;
    int x =0,y=r;
    draw8Way(cx,cy,x,y);


    while(x<y)
    {
        if(d<0){// dE
            d+= (8*x + 12);
            x++;
        }
        else{//dSE
            d+=(8*x-8*y+20);
            x++;
            y--;
        }
        draw8Way(cx,cy,x,y);
    }

}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    GridDraw();
    glBegin(GL_POINTS);
    int microseconds = 50000;


    int dx=5,dy=5;
    int cx = -150,cy=150,r=30;

    int Xmin = -Wi/2;
    int Xmax = Wi/2-1;
    int Ymin = -He/2;
    int Ymax = He/2-1;

    while(true)
        {

            if(cx+r >= Xmax || cx-r<= Xmin)
            {
                dx *= -1;
            }
            if(cy+r>= Ymax || cy-r<= Ymin)
            {
                dy *= -1;
            }


            glClear(GL_COLOR_BUFFER_BIT);
            GridDraw();
            glBegin(GL_POINTS);
            circleDrawing(cx,cy,29);
            circleDrawing(cx,cy,30);
            circleDrawing(cx,cy,31);
            glEnd();
            glFlush();
            usleep(microseconds);
            cx += dx;
            cy += dy;
        }

    glEnd();
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

