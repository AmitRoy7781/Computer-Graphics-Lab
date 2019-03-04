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
#include <GL/glut.h>
#include<cstdio>
#include<iostream>
#include <iostream>
#include <ctime>
using namespace std;

int Wi=640, He=480;

//##############################Define Variables here###########################
int maxy =He/2-1 ;
int miny =-He/2 ;
int maxx =Wi/2-1 ;
int minx =-Wi/2 ;
int top = 8;
int bottom = 4;
int right_ = 2;
int left_ = 1;

//##############################################################################


void myInit (void);
void display(void);
void reshape (int w, int h);
void draw_grid(void);
//##############################Define Prototypes here###########################
int get_zone(int x_0,int y_0, int x_1, int y_1);
void drawline_midpoint(int x_0,int y_0, int x_1, int y_1,int zone);
void draw_vertex(int x, int y, int zone);
void draw_circle(int x,int y,int r);
void draw_line(int x_0,int y_0, int x_1, int y_1);
int makecode(int x,int y);
void clipLine_cohen_sutherland(int x0, int y0, int x1, int y1);
//##############################################################################

void myInit (void){
    glClearColor(0.0, 0.0, 0.0,1.0);
}

void reshape (int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (-Wi/2, Wi/2-1, -He/2, He/2-1); // size of projection plane (origin at the center)
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

//draws the grid
void draw_grid(void){
    glBegin(GL_LINES);
    glVertex2i(-140,miny);
    glVertex2i(-140,maxy);
    glVertex2i(140,miny);
    glVertex2i(140,maxy);
    glVertex2i(minx,100);
    glVertex2i(maxx,100);
    glVertex2i(minx,-100);
    glVertex2i(maxx,-100);

    glEnd();
}

//generates rand integers for origin and radius of circles and draws them
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    draw_grid();


    if(true){
        for (int i=0;i<100;i++){
            int x0 = (rand()%(2*maxx))-maxx;
            int x1 = (rand()%(2*maxx))-maxx;
            int y0 = (rand()%(2*maxy))-maxy;
            int y1 = (rand()%(2*maxy))-maxy;
            clipLine_cohen_sutherland(x0,y0,x1,y1);
        }
    }

clipLine_cohen_sutherland(-170,70,-120,140);

    glFlush();
}

void clipLine_cohen_sutherland(int x0, int y0, int x1, int y1){
    glPointSize(3);
    glColor3f(1.0,1.0,1.0);

    glBegin(GL_POINTS);
    glVertex2i(x0,y0);
    glVertex2i(x1,y1);
    glEnd();
    glPointSize(1);
    glBegin(GL_POINTS);

    int x0_ = x0;
    int x1_ = x1;
    int y0_ = y0;
    int y1_ = y1;
    int code0,code1;
    code0 = makecode(x0,y0);
    code1 = makecode(x1,y1);
    while(1){

        if((code0|code1)==0){

            glColor3f(1.0, 1.0, 0.0);
            draw_line(x0,y0,x1,y1);
            break;
        } else if(code0&code1){
            glColor3f(1.0, 0.0, 0.0);
            draw_line(x0_,y0_,x1_,y1_);
            break;
        }
        else{
            int code,x,y;
            if(code0){
                code = code0;
            } else{
                code  = code1;
            }
            if(code&top){
                y = 100;
                x = x0+ (y-y0)*(x1 - x0)/(y1-y0);

            }
            else if(code&bottom){
                y = -100;
                x = x0+ (y-y0)*(x1 - x0)/(y1-y0);
            }
            else if(code&right_){
                x = 140;
                y = y0+ (x-x0)*(y1 - y0)/(x1-x0);
            }
            else if(code&left_){
                x = -140;
                y = y0+ (x-x0)*(y1 - y0)/(x1-x0);
            }
            if(code==code0){
                glColor3f(0.5, 0.5, 0.5);
                draw_line(x0,y0,x,y);
                x0 = x;
                y0 = y;
                code0 = makecode(x0,y0);
            }
            else{
                glColor3f(0.5, 0.5, 0.5);
                draw_line(x1,y1,x,y);
                x1 = x;
                y1 = y;
                code1 = makecode(x1,y1);
            }
        }
    }
    glEnd();
}

int makecode(int x,int y){
    int code = 0;

    if(y>100){
        code+=top;
    }
    else if(y<-100){
        code+=bottom;
    }
    if(x>140){
        code+=right_;
    }
    else if(x<-140){
        code+=left_;
    }
    return code;
}


//draws line by using midpoint line drawing algorithm for zone 0 after necessary transformation
//uses different colors for different zones
void draw_line(int x_0,int y_0, int x_1, int y_1){

    if (get_zone(x_0,y_0,x_1,y_1) == 0){
        drawline_midpoint(x_0,y_0,x_1,y_1, 0);
    }
    if (get_zone(x_0,y_0,x_1,y_1) == 1){
        drawline_midpoint(y_0,x_0,y_1,x_1,1);
    }
    if (get_zone(x_0,y_0,x_1,y_1) == 2){
        drawline_midpoint(y_0,-x_0,y_1,-x_1,2);
    }
    if (get_zone(x_0,y_0,x_1,y_1) == 3){
        drawline_midpoint(-x_0,y_0,-x_1,y_1,3);
    }
    if (get_zone(x_0,y_0,x_1,y_1) == 4){
        drawline_midpoint(-x_0,-y_0,-x_1,-y_1,4);
    }
    if (get_zone(x_0,y_0,x_1,y_1) == 5){
        drawline_midpoint(-y_0,-x_0,-y_1,-x_1,5);
    }
    if (get_zone(x_0,y_0,x_1,y_1) == 6){
        drawline_midpoint(-y_0,x_0,-y_1,x_1,6);
    }
    if (get_zone(x_0,y_0,x_1,y_1) == 7){
        drawline_midpoint(x_0,-y_0,x_1,-y_1,7);
    }
}

//midpoint line drawing algorithm for zone 0
void drawline_midpoint(int x_0,int y_0, int x_1, int y_1, int zone){
    int dx = x_1 - x_0;
    int dy = y_1 - y_0;
    int d = 2*dy - dx;
    int de = 2*dy;
    int den = 2*(dy-dx);
    int x,y;
    x= x_0;
    y= y_0;

    draw_vertex(x,y,zone);
    while(x<x_1){
        if(d<0){
            x++;
            d+=de;
        }
        else{
            x++;
            y++;
            d+=den;
        }
        draw_vertex(x,y,zone);
    }
}

//draws the pixel using 8 way symmetry
void draw_vertex(int x, int y, int zone){
    if(zone==0)
        glVertex2i(x,y);
    else if(zone==1)
        glVertex2i(y,x);
    else if(zone==2)
        glVertex2i(-y,x);
    else if(zone==3)
        glVertex2i(-x,y);
    else if(zone==4)
        glVertex2i(-x,-y);
    else if(zone==5)
        glVertex2i(-y,-x);
    else if(zone==6)
        glVertex2i(y,-x);
    else if(zone==7)
        glVertex2i(x,-y);

}

// returns the zone of a line
int get_zone(int x_0,int y_0, int x_1, int y_1){
    int dx = x_1 - x_0;
    int dy = y_1 -y_0;
    if(dx>=0 && dy>=0){
        if (abs(dx)>=abs(dy))
            return 0;
        else
            return 1;
    }
    else if(dx<0 && dy>=0){
        if (abs(dx)>=abs(dy))
            return 3;
        else
            return 2;
    }
    else if(dx<0 && dy<0){
        if (abs(dx)>=abs(dy))
            return 4;
        else
            return 5;
    }
    else if(dx>=0 && dy<0){
        if (abs(dx)>=abs(dy))
            return 7;
        else
            return 6;
    }

}

int main (int argc, char **argv){
    //freopen ("input.txt","r",stdin);
    srand(time(NULL));
    //#############Take Inputs###############
    cout<<minx<<" "<<miny<<" "<<maxx<<" "<<maxy<<endl;

    //#######################################
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