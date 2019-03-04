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
#define  COLORED true

using namespace std;

int Wi=640, He=480;
unsigned int microseconds = 10000;

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


void drawPixel(int cx,int cy,int x,int y)
{
    glVertex2i(cx+x,cy+y);
}

void drawPixelZone(int cx,int cy,int x,int y)
{
    int zone = zoneDetection(0,0,x,y);
    if(zone==0)
    {
        if(COLORED)
            glColor3f(1.0, 0.0, 0.0);///Red
        drawPixel(cx,cy,x,y);
    }
    else if(zone==1)
    {
        if(COLORED)
            glColor3f(0.0,1.0,0.0);///Green
        drawPixel(cx,cy,x,y);
    }
    else if(zone==2)
    {
        if(COLORED)
            glColor3f(0.0,0.0,1.0);///Blue
        drawPixel(cx,cy,x,y);
    }
    else if(zone==3)
    {
        if(COLORED)
                glColor3f(1.0,1.0,0.0);///Yellow
        drawPixel(cx,cy,x,y);
    }
    else if(zone==4)
    {
        if(COLORED)
            glColor3f(1.0,0.1,1.0);///Purple
        drawPixel(cx,cy,x,y);
    }
    else if(zone==5)
    {
        if(COLORED)
            glColor3f(0,1.0,1.0);///Cyan
        drawPixel(cx,cy,x,y);
    }
    else if(zone==6)
    {
        if(COLORED)
            glColor3f(1.0,1.0,1.0);///White
        drawPixel(cx,cy,x,y);
    }
    else if(zone==7)
    {
        if(COLORED)
            glColor3f(1.0,0.5,0.0);///Orange
        drawPixel(cx,cy,x,y);
    }
}


void draw4Way(int cx,int cy,int x,int y)
{
    drawPixelZone(cx,cy,x,y);
    drawPixelZone(cx,cy,-x,y);
    drawPixelZone(cx,cy,x,-y);
    drawPixelZone(cx,cy,-x,-y);
}

void drawElipse(int cx,int cy,int a,int b)
{
    float d = b*b - a*a*(b-0.25);
    int x = 0,y=b;
    drawPixel(cx,cy,x,y);
    while(a*a*(y-0.5) > b*b*(x+1))
    {
        if(d<0){//delE
            d += b*b*(2*x+3);
            x++;
        }
        else{//delSE
            d += (b*b*(2*x+3)-a*a*(2*y-2));
            x++;
            y--;
        }
        draw4Way(cx,cy,x,y);

        
    }
    while(y>0)//R-2
        {
            if(d<0){//delSE
                d += (a*a*(-2*y+3)+b*b*(2*x+2));
                x++;
                y--;       
            }
            else{
                d+=(a*a*(-2*y+3));
                y--;
            }
            draw4Way(cx,cy,x,y);
        }
}

void display()
{
     glClear(GL_COLOR_BUFFER_BIT);
    // glColor3f(0, 0, 0);
    // GridDraw();
     glBegin(GL_POINTS);

    int Xmin = -Wi/2;
    int Xmax = Wi/2-1;
    int Ymin = -He/2;
    int Ymax = He/2-1;

    int cx=0;
    int cy=100;
    int a=50;
    int b=50;
    int dy;
    int MOD = He/20;

    while(true)
    {

    dy = 1;

    while(cy+dy-60>=Ymin)
        {
            cy -=dy;
            //dy--;
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_POINTS);
            drawElipse(cx,cy,a,b);
            glEnd();
            glutSwapBuffers();
            if(cy%MOD==0)
                dy++;
        }


    while(b>=30)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
        drawElipse(cx,cy,a,b);
        glEnd();
        glutSwapBuffers();
        a+=1;
        b--;
        cy--;
    }

    while(a!=b)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
        drawElipse(cx,cy,a,b);
        glEnd();
        glutSwapBuffers();
        a-=1;
        b+=1;
        cy++;
    }

    a = 50;
    b = 50;
    while(cy<=100)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
        drawElipse(cx,cy,a,b);
        glEnd();
        glutSwapBuffers();
        cy+=dy;
        if(cy%MOD==0)
            dy--;
        
    }

    glEnd();
    glutSwapBuffers();

    }
}


int main (int argc, char **argv)
{

    glutInit (&argc, argv); // to initialize the toolkit;
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // sets the display mode
    glutInitWindowSize (Wi, He); // sets the window size
    glutInitWindowPosition (0, 0); // sets the starting position for the window
    glutCreateWindow ("Graphics Lab 2"); // creates the window and sets the title
    myInit(); // additional initializations as necessary
    glutReshapeFunc(reshape);
    glutDisplayFunc (display);
    glutMainLoop(); // go into a loop until event occurs
    return 0;
}

