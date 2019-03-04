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

#include<bits/stdc++.h>
using namespace std;

using namespace std;

int Wi=640, He=480;

void myInit (void);
void display(void);
void reshape (int w, int h);

void myInit (void){
    glClearColor(0.0, 0.0, 0.0,1.0);
}

void reshape (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  // gluOrtho2D(-5, 74, -5, 54);
  // gluOrtho2D(-5, 74, -5, 54);
  int percentage = 4;
  gluOrtho2D (-Wi/percentage, Wi/percentage-1, -He/percentage, He/percentage-1); // size of projection plane (origin at the center)
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

#define MAX 100
#define PII pair<int,int>


struct Point{
	Point(int aa,int bb)
	{
		x=aa;
		y=bb;
	}
	Point()
	{

	}
	int x,y;
};

struct Edge_table{
	double ymin;
	double ymax;
	double x;
	double dx;
	double dy;
	double sign;
	double new_x;
	double new_y;
	double antislope;

	 bool operator < (Edge_table temp) const
	{
		if(x<temp.x)return true;
		return false;
	}
};


int n,X_MIN,X_MAX,Y_MIN,Y_MAX;
Point point[MAX];
Edge_table edge_table[MAX+1];
std::map<PII,int> mp;
std::map<int,std::vector<Edge_table> > active_edge_table;

int min(int a,int b){
	if(b<a) return b;
	else return a;
}

int max(int a, int b){
	if(a>b) return a;
	else return b;
}


void BIG_POINT(int x,int y)
{
	
	glEnd();
            
            glBegin(GL_POINTS);

            int intersection = mp[PII(x,y)];
            if(intersection==0)
    		{
    			glColor3f(0.5,0.5,0.5);///Ash
    		}
    	else if(intersection==1)
    		{
    			glColor3f(1.0,1.0,0.0);///Yellow
    		}
    	else if(intersection==2)
    		{
    			glBegin(GL_POINTS);
            	glPointSize(10);
    			glColor3f(1.0,0.0,0.0);///Red
    		}

            glVertex2i(x,y);
            glEnd();
             
            glBegin(GL_POINTS);
            glColor3f(1.0,1.0,1.0);
}

bool sort_edge_table(const Edge_table &a, const Edge_table &b){
	if(a.ymin<b.ymin) return true;
	else if(a.ymin == b.ymin){
		if(a.x<b.x) return true;
		if(a.x==b.x) return true;
		else return false;
	}
	return false;
} 

bool sort_X_Based(const Edge_table &a, const Edge_table &b)
{
	if(a.x<=b.x)	return true;
	return false;
}



void input(){
	
	freopen("in.txt","r",stdin);
	scanf("%d",&n);
	for(int i=0;i<n;i++){
		int a,b;
		scanf("%d %d",&a,&b);
		point[i].x=a;
		point[i].y=b;
		Y_MIN = min(Y_MIN,b);
		Y_MAX = max(Y_MAX,b); 
	}
}

void make_edge_table(){
	int guni=0;
	for(int i=0;i<n;i++){
		Point a=point[i];
		Point b=point[(i+1)%n];
		if(a.y<b.y){
			// printf("%d %d\n",a.y,b.y);
			edge_table[guni].ymin = a.y;
			edge_table[guni].ymax = b.y;
			edge_table[guni].x = a.x;
			edge_table[guni].dx = (double)(b.x-a.x);
			edge_table[guni].dy = (double)(b.y - a.y);
			edge_table[guni].new_x = a.x;
			edge_table[guni].new_y = a.y;
			edge_table[guni].antislope = edge_table[guni].dx/edge_table[guni].dy;
			
			if(((b.x-a.x)*(b.y-a.y))>=0){
				edge_table[guni].sign=1;
			}
			else{
				edge_table[guni].sign=-1;
			}
			guni++;
			
		}
		else if(a.y>b.y){
			// printf("%d %d\n",a.y,b.y);
			edge_table[guni].ymin = b.y;
			edge_table[guni].ymax = a.y;
			edge_table[guni].x = b.x;
			edge_table[guni].dx = (b.x-a.x);
			edge_table[guni].dy = (b.y - a.y);
			edge_table[guni].new_x = b.x;
			edge_table[guni].new_y = b.y;
			edge_table[guni].antislope = edge_table[guni].dx/edge_table[guni].dy;
			if(((b.x-a.x)*(b.y-a.y))>=0){
				edge_table[guni].sign=1;
			}
			else{
				edge_table[guni].sign=-1;
		    }
		    
		    guni++;
		}
		mp[PII(edge_table[guni-1].x,edge_table[guni-1].ymin)] += 1;		
	}
	n = guni;
	std::sort(edge_table,edge_table+n,sort_edge_table);
	for(int i=0;i<n;i++){
		
		active_edge_table[edge_table[i].ymin].push_back(edge_table[i]);
	}
}


void make_active_Edge_Table()
{
	for(int i=Y_MIN;i<Y_MAX;i++)
	{
		if(active_edge_table[i].size())
		{
			sort(active_edge_table[i].begin(),
				active_edge_table[i].end(),
				sort_X_Based);
		printf("%d-> ",i);
		for(int j=0;j<active_edge_table[i].size();j++)
		{
			double xx = active_edge_table[i][j].x;
			int yy = active_edge_table[i][j].ymax;
			int dx = active_edge_table[i][j].dx;
			int dy = active_edge_table[i][j].dy;
			double antislope = active_edge_table[i][j].antislope;


			printf("(%d %0.2lf %0.2lf)->",yy,xx,antislope);

			if(yy-1 != i)
			{
				Edge_table temp = active_edge_table[i][j];
				temp.x += temp.antislope;
				active_edge_table[i+1].push_back(temp);
			}
		}
		printf("lambda\n");

		}
	}
}




void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(3);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    for(int y=Y_MIN;y<=Y_MAX;y++)
    {
    	for(int i=0;i<active_edge_table[y].size();i++)
    	{
    		int x= active_edge_table[y][i].x;
    		glColor3f(1.0,1.0,1.0);
    		glVertex2i(x,y);
    	}
    }


    for(int i=0;i<n;i++)
    {
    	glColor3f(1.0,1.0,0.0);///Ash
    	BIG_POINT(point[i].x,point[i].y);

    }


	//Polygon Filling Part    
    for(int y=Y_MIN;y<Y_MAX;y++)
    {

    	for(int i=0;i<active_edge_table[y].size()-1;i+=2)
    	{
    		int x0 = active_edge_table[y][i].x;
    		int y0 = y;
    		int x1 = active_edge_table[y][i+1].x;
    		int y1 = y;

    		 printf("%d %d %d %d\n",x0,y0,x1,y1);
    		
    		glBegin(GL_LINES);
    		glColor3f(1.0,0.0,0.0);
    		glVertex2i(x0,y0);
    		glVertex2i(x1,y1);
    		glEnd();

    	}
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

    input();
    make_edge_table();
    make_active_Edge_Table();

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(Wi, He);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Polygon Filling");

	myInit();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
