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
#include <GL/glut.h>
#define  PI acos(-1.0)

using namespace std;

int Wi=640, He=480;

void myInit (void);
void display(void);
void reshape (int w, int h);

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


void GridDraw()
{
    glBegin(GL_LINES);

    glColor3f(1.0,1.0,1.0);
    glVertex2i(-320,0);
    glVertex2i(319,0);
    glVertex2i(0,239);
    glVertex2i(0,-240);


    glEnd();

}

void myLineDraw()
{

    double x0=-100,y0=100,x2=100,y2=-100;

    for(double x=x0,y=y0;;)
    {
        if(x0<x2)
        {
            x+=0.001;
            if(x>x2)
                break;
        }
        else
        {
            x-=0.001;
            if(x<x2)
                break;
        }
        if(y0<y2)
        {
            y+=0.001;
            if(y>y2)
                break;
        }
        else
        {
            y-=0.001;
            if(y<y2)
                break;
        }

        glVertex2f(x,y);
    }


}

void drawPixel(int x,int y)
{
    glVertex2i(x,y);
}

void drawPixelZone(int x,int y,int zone)
{
    if(zone==0)
    {
        glColor3f(1.0, 0.0, 0.0);///Red
        drawPixel(x,y);
    }
    else if(zone==1)
    {
        glColor3f(0.0,1.0,0.0);///Green
        drawPixel(y,x);
    }
    else if(zone==2)
    {
        glColor3f(0.0,0.0,1.0);///Blue
        drawPixel(-y,x);
    }
    else if(zone==3)
    {
        glColor3f(1.0,1.0,0.0);///Yellow
        drawPixel(-x,y);
    }
    else if(zone==4)
    {
        glColor3f(1.0,0.1,1.0);///Purple
        drawPixel(-x,-y);
    }
    else if(zone==5)
    {
        glColor3f(0,1.0,1.0);///Cyan
        drawPixel(-y,-x);
    }
    else if(zone==6)
    {
        glColor3f(1.0,1.0,1.0);///White
        drawPixel(y,-x);
    }
    else if(zone==7)
    {

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
        if(dx>=dy)		return 0;
        else	return 1;
    }
    else if(dx<0 && dy>=0)
    {
        if(dy>=abs(dx))		return 2;
        else	return 3;

    }
    else if(dx<0 && dy<0)
    {
        if(abs(dx)>=abs(dy))	return 4;
        else 	return 5;
    }
    else if(dx>=0 && dy<0)
    {
        if(abs(dy)>=dx)		return 6;
        else 	return 7;
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


void task1()
{
    srand(time(NULL));
    for(int i=0; i<=70; i++)
    {
        int x0 = -320 + rand()%640;
        int y0 = -240 + rand()%480;
        int x1 = -320 + rand()%640;
        int y1 = -240 + rand()%480;

        drawLine(x0,y0,x1,y1);
    }
}

void task2()
{
        srand(time(NULL));

        for(int k=1;k<=10;k++)
        {
        int x = -320 + rand()%640;
        int y = -240 + rand()%480;
        int r  = 50+abs(rand()%50);


        drawLine(x,y,x+r,y);
        drawLine(x,y,x,y+r);
        drawLine(x,y,x-r,y);
        drawLine(x,y,x,y-r);


        for(int i=1; i<=30; i++)
        {
            int h = r*cos(i*PI/12.0);
            int v = r*sin(i*PI/12.0);

            drawLine(x,y,x+h,y+v);
        }

        }

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    //GridDraw();
    glBegin(GL_POINTS);
    //myLineDraw();

    //task1();
    task2();

    glEnd();
    glFlush();
}


int main (int argc, char **argv)
{

    glutInit (&argc, argv); // to initialize the toolkit;
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // sets the display mode
    glutInitWindowSize (Wi, He); // sets the window size
    glutInitWindowPosition (0, 0); // sets the starting position for the window
    glutCreateWindow ("Line Drawing"); // creates the window and sets the title
    myInit(); // additional initializations as necessary
    glutReshapeFunc(reshape);
    glutDisplayFunc (display);
    glutMainLoop(); // go into a loop until event occurs
    return 0;
}

