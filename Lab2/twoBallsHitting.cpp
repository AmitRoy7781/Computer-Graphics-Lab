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
#define  COLORED false

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
        if(COLORED)
            glColor3f(1.0, 0.0, 0.0);///Red
        drawPixel2(cx,cy,x,y);
    }
    else if(zone==1)
    {
        if(COLORED)
            glColor3f(0.0,1.0,0.0);///Green
        drawPixel2(cx,cy,y,x);
    }
    else if(zone==2)
    {
        if(COLORED)
            glColor3f(0.0,0.0,1.0);///Blue
        drawPixel2(cx,cy,-y,x);
    }
    else if(zone==3)
    {
        if(COLORED)
            glColor3f(1.0,1.0,0.0);///Yellow
        drawPixel2(cx,cy,-x,y);
    }
    else if(zone==4)
    {
        if(COLORED)
            glColor3f(1.0,0.1,1.0);///Purple
        drawPixel2(cx,cy,-x,-y);
    }
    else if(zone==5)
    {
        if(COLORED)
            glColor3f(0,1.0,1.0);///Cyan
        drawPixel2(cx,cy,-y,-x);
    }
    else if(zone==6)
    {
        if(COLORED)
            glColor3f(1.0,1.0,1.0);///White
        drawPixel2(cx,cy,y,-x);
    }
    else if(zone==7)
    {
        if(COLORED)
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

int dis(int x0,int y0,int x1,int y1)
{
    return((x0-x1) * (x0-x1) + (y0-y1)*(y0-y1));
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    //GridDraw();
    glBegin(GL_POINTS);
    int microseconds = 10000;


    int dx1=5,dy1=-5;
    int cx1 = -70,cy1=150,r1=50;

    int dx2=-5,dy2=5;
    int cx2 = 150,cy2=-150,r2=60;

    int Xmin = -Wi/2;
    int Xmax = Wi/2-1;
    int Ymin = -He/2;
    int Ymax = He/2-1;

    while(true)
        {

            if(cx1+r1 >= Xmax || cx1-r1<= Xmin || 
                dis(cx1,cy1,cx2,cy2)<=(r1+r2)*(r1+r2))
            {
                dx1 *= -1;
            }
            if(cy1+r1>= Ymax || cy1-r1<= Ymin ||
                dis(cx1,cy1,cx2,cy2)<=(r1+r2)*(r1+r2))
            {
                dy1 *= -1;
            }



            if(cx2+r2 >= Xmax || cx2-r2<= Xmin
                || dis(cx1,cy1,cx2,cy2)<=(r1+r2)*(r1+r2))
            {
                dx2 *= -1;
            }
            if(cy2+r2>= Ymax || cy2-r2<= Ymin
                || dis(cx1,cy1,cx2,cy2)<=(r1+r2)*(r1+r2))
            {
                dy2 *= -1;
            }
            glClear(GL_COLOR_BUFFER_BIT);
            //GridDraw();
            glBegin(GL_POINTS);

            if(dx1==5 && dy1==5)
                glColor3f(1.0, 0.0, 0.0);///Red
            else if(dx1==5 && dy1==-5)
                glColor3f(0.0,1.0,0.0);///Green
            else if(dx1==-5 && dy1==5)
                glColor3f(0.0,0.0,1.0);///Blue
            else
                glColor3f(1.0,1.0,1.0);///White
            
            circleDrawing(cx1,cy1,r1-1);
            circleDrawing(cx1,cy1,r1);
            circleDrawing(cx1,cy1,r1+1);

            if(dx2==5 && dy2==5)
                glColor3f(1.0, 0.0, 0.0);///Red
            else if(dx2==5 && dy2==-5)
                glColor3f(0.0,1.0,0.0);///Green
            else if(dx2==-5 && dy2==5)
                glColor3f(0.0,0.0,1.0);///Blue
            else
                glColor3f(1.0,1.0,1.0);///White
            circleDrawing(cx2,cy2,r2-1);
            circleDrawing(cx2,cy2,r2);
            circleDrawing(cx2,cy2,r2+1);
            glEnd();
            glutSwapBuffers();
            // usleep(microseconds);
            cx1 += dx1;
            cy1 += dy1;

            cx2 += dx2;
            cy2 += dy2;
        }

    glEnd();
    glutSwapBuffers();
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

