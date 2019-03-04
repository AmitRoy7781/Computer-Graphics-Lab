#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <GL/glut.h>
//#include <bits/stdc++.h>


#define MAX 150
#define INF 1000000000


/*****************************************************
			(-y,x)			   |      (y,x)
							   |
	        (-x,y)             |      (x,y)
	       ----------------------------------------
	        (-x,-y)            |       (x,-y)
							   |
			(-y,-x)			   |       (y,-x)



 */



int n; //number of verices
int Y_MIN,Y_MAX;
int pointer_edge_table=0; //koto tuke porjonto nawa hoise
int counter = 0 ; // temp_edge_table e koita element ache 
int x_st,y_st,x_en,y_en; 
int tot_line; 

struct Point{
	int x,y;
};
Point point[MAX+1];


struct Edge_table{
	int ymin;
	int ymax;
	int x;
	int dx;
	int dy;
	int sign;
	int new_x;
	int new_y;
};

Edge_table edge_table[MAX+1],temp_edge_table[MAX+1];

void init2D(float r, float g, float b)
{
	glClearColor(r,g,b,0.0);
	glMatrixMode (GL_PROJECTION);
	gluOrtho2D (-250,250,-250,250);
}

//function to find zone
int find_zone(){
	int dx=x_en-x_st;
	int dy=y_en-y_st;
	if(dx>=0 && dy >=0){
		if(abs(dx)>=abs(dy)) return 0;
		return 1;
	}
	if(dx<=0 && dy>=0){
		if(abs(dy)>=abs(dx)) return 2;
		return 3;
	}
	if(dx<=0 && dy <= 0){
		if(abs(dx)>=abs(dy)) return 4;
		return 5;
	}
	if(dx>=0 && dy<=0){
		if(abs(dy)>=abs(dx)) return 6;
		return 7;
	}
	return 0;
}


int return_x(int x,int y,int pr_zone,int tr_zone,int choice){
		//converting into 0 ghor
		int tx,ty;
		if(pr_zone==0) {
			tx=x;
			ty=y;
		}
		if(pr_zone==1) {
			tx=y;
			ty=x;
		}
		if(pr_zone==2){
			tx=-y;
			ty=x;
		}
		if(pr_zone==3){
			tx=-x;
			ty=y;
		}
		if(pr_zone==4) {
			tx=-x;
			ty=-y;
		}
		if(pr_zone==5) {
			tx=-y;
			ty=-x;
		}
		if(pr_zone==6) {
			tx=y;
			ty=-x;
		}
		if(pr_zone==7) {
			tx=x;
			ty=-y;
		}
			x=tx;
			y=ty;
			if(tr_zone==0) {
				 if(choice==0) return x;
				 else return y;
			}
			if(tr_zone==1) {
				 if(choice==0) return y;
				 else return x;
			}
			if(tr_zone==2) {
				 if(choice==0) return -y;
				 else return x;
			}
			if(tr_zone==3) {
				 if(choice==0) return -x;
				 else return y;
			}
			if(tr_zone==4) {
				 if(choice==0) return -x;
				 else return -y;
			}
			if(tr_zone==5) {
				 if(choice==0) return -y;
				 else return -x;
			}
			if(tr_zone==6) {
				 if(choice==0) return y;
				 else return -x;
			}
			if(tr_zone==7) {
				 if(choice==0) return x;
				 else return -y;
			}
			return -1;
}

void line_draw(){

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);

	int zone = find_zone();
	/*if(zone==0) glColor3f(1.0, 0.0, 0.0);
	if(zone==1) glColor3f(0.0, 0.5, 0.7);
	if(zone==2) glColor3f(0.2, 0.6, 0.0);
	if(zone==3) glColor3f(0.3, 0.4, 0.5);
	if(zone==4) glColor3f(0.5, 0.0, 0.0);
	if(zone==5) glColor3f(1.0, 1.0, 0.0);
	if(zone==6) glColor3f(0.4, 0.5, 0.0);
	if(zone==7) glColor3f(0.9, 0.0, 0.4);*/

	int x1=return_x(x_st,y_st,zone,0,0);
	int y1=return_x(x_st,y_st,zone,0,1);
	int x2=return_x(x_en,y_en,zone,0,0);
	int y2=return_x(x_en,y_en,zone,0,1);
	if(x1>x2){
		int temp=x1;
		x1=x2;
		x2=temp;

		temp=y1;
		y1=y2;
		y2=temp;
	}
	//cout<<"x1 = " <<x1<<" "<<y1<<endl;
	//cout<<"x2 = " <<x2<<" "<<y2<<endl;
	int x = x1;
	int y = y1;

	int dx=x2-x1;
	int dy=y2-y1;
	int d = 2*dy - dx;
	int d_n= 2*dy;
	int d_ne=2*(dy-dx);

	printf("zone %d\n",zone);
	int tempx=return_x(x,y,0,zone,0);
	int tempy=return_x(x,y,0,zone,1);

	glVertex2i(tempx,tempy);
	//printf("x = %d y = %d\n",tempx,tempy);
	while(x<x2){
		if(d<0){
			x++;
			d=d+d_n; //error bare
		}
		else{
			x++;
			y++;
			d=d+d_ne; //error kome
		}
		//cout<<"x = " <<x<<" "<<y<<endl;
		tempx=return_x(x,y,0,zone,0);
		tempy=return_x(x,y,0,zone,1);
		glVertex2i(tempx,tempy);
		//printf("x = %d y = %d\n",tempx,tempy);
	}
	//printf("ber hoi\n");
    tempx=x_en;
    tempy=y_en;
	glVertex2i(tempx,tempy);
	glEnd();
    glFlush();
	return;
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

int min(int a,int b){
	if(b<a) return b;
	else return a;
}

int max(int a, int b){
	if(a>b) return a;
	else return b;
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


//line draw function 


//take each edge save ymin,ymax,(ymin,x) er x,dx,dy,sign,new x
void make_edge_table(){
	int guni=0;
	for(int i=0;i<n;i++){
		Point a=point[i];
		Point b=point[(i+1)%n];
		if(a.y<b.y){
			printf("%d %d\n",a.y,b.y);
			edge_table[guni].ymin = a.y;
			edge_table[guni].ymax = b.y;
			edge_table[guni].x = a.x;
			edge_table[guni].dx = abs(b.x-a.x);
			edge_table[guni].dy = abs(b.y - a.y);
			edge_table[guni].new_x = a.x;
			edge_table[guni].new_y = a.y;
			
			if(((b.x-a.x)*(b.y-a.y))>=0){
				edge_table[guni].sign=1;
			}
			else{
				edge_table[guni].sign=-1;
			}
			guni++;
			
		}
		else if(a.y>b.y){
			printf("%d %d\n",a.y,b.y);
			edge_table[guni].ymin = b.y;
			edge_table[guni].ymax = a.y;
			edge_table[guni].x = b.x;
			edge_table[guni].dx = abs(b.x-a.x);
			edge_table[guni].dy = abs(b.y - a.y);
			edge_table[guni].new_x = b.x;
			edge_table[guni].new_y = b.y;
			if(((b.x-a.x)*(b.y-a.y))>=0){
				edge_table[guni].sign=1;
			}
			else{
				edge_table[guni].sign=-1;
		    }
		    guni++;
		}
		/*else {
			//x axix parallel line
			edge_table[i].ymin = b.y;
			edge_table[i].ymax = a.y;
			edge_table[i].x = min(a.x,b.x);
			edge_table[i].dx = abs(b.x - a.x);
			edge_table[i].dy = abs(b.y - a.y);
			edge_table[i].sign = 1;
			edge_table[i].new_x = 0; 
		}*/		
	}
	n = guni;
	std::sort(edge_table,edge_table+n,sort_edge_table);
	for(int i=0;i<n;i++){
		printf("ymin = %d ymax = %d x = %d dx = %d dy = %d sign = %d new_x = %d new_y = %d\n",edge_table[i].ymin,edge_table[i].ymax,edge_table[i].x,edge_table[i].dx,edge_table[i].dy,edge_table[i].sign,edge_table[i].new_x,edge_table[i].new_y);
	}
}

//return those edeges where ymin == desired ymin
int return_edge(int ymin){
	//start from pointer
	for(int i=pointer_edge_table;i<n;i++){
		if(edge_table[i].ymin == ymin){
			counter++;
			temp_edge_table[counter]=edge_table[i];
			pointer_edge_table++;
		}
		else {
			break;
		}
	}
	return counter; 
}

void active_list(){
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	int ymin=Y_MIN;
	int ymax=Y_MAX;
	pointer_edge_table=0;
	counter = 0;
	tot_line=0;
	for(int i=ymin;i<ymax;i++){
		//edge niye asi jeigular ymin i er shoman
		counter = return_edge(i); 
		// x er basis e sort kori 
		std::sort(temp_edge_table+1,temp_edge_table+counter,sort_edge_table); 
		//now draw line 
		printf("counter %d\n",counter);
		printf("taken\n");
		for(int j=1;j<=counter;j++){
			printf("ymin = %d ymax = %d x=%d sign = %d\n dy = %d dx = %d new_x = %d new_y = %d\n",temp_edge_table[j].ymin,temp_edge_table[j].ymax,temp_edge_table[j].x,temp_edge_table[j].sign,temp_edge_table[j].dy,temp_edge_table[j].dx,temp_edge_table[j].new_x,temp_edge_table[j].new_y);
		}
		for(int j=1;j<=counter;j+=2){
			//draw line between j and j+1
			//make set of lines 
			x_st = temp_edge_table[j].x;
			y_st = temp_edge_table[j].ymin;
			x_en = temp_edge_table[j+1].x;
			y_en = temp_edge_table[j+1].ymin;
			tot_line++;
			printf("x_st = %d y_st = %d x_en = %d y_en = %d\n",x_st,y_st,x_en,y_en);
			line_draw(); 
		}
		//now change the edges 
		int second_counter = 0;
		//printf("notun hishab\n");
		for(int j=1;j<=counter;j++){
			temp_edge_table[j].ymin++;
			if(temp_edge_table[j].ymin != temp_edge_table[j].ymax){
				//eta shesh hoi nai 
				second_counter++;
				temp_edge_table[second_counter].ymin=temp_edge_table[j].ymin;
				temp_edge_table[second_counter].ymax=temp_edge_table[j].ymax;
				temp_edge_table[second_counter].dx=temp_edge_table[j].dx;
				temp_edge_table[second_counter].dy=temp_edge_table[j].dy;
				temp_edge_table[second_counter].sign=temp_edge_table[j].sign;
				temp_edge_table[second_counter].new_x=temp_edge_table[j].new_x;
				temp_edge_table[second_counter].new_y=temp_edge_table[j].new_y;
				if(temp_edge_table[second_counter].dy != 0){ //x will change 
						double lob = (double)temp_edge_table[j].dx*1.0;
						double hor = (double)temp_edge_table[j].dy*1.0;
						double frac = (double)temp_edge_table[j].new_x+1.0*(temp_edge_table[j].sign)*(lob/hor)*(temp_edge_table[j].ymin-temp_edge_table[j].new_y);
						int jog = round(frac);
						//printf("frac = %lf jog = %d\n",frac,jog);
						temp_edge_table[second_counter].x = jog;
				}
				else{
					temp_edge_table[second_counter].x=temp_edge_table[j].x;
				}
			}
		}
		counter = second_counter; 
	}
	//printf("tot line %d\n",tot_line);
	return;
}

void polygonFillingAlgorithm(){
	Y_MIN = INF;
	Y_MAX = -INF; 
	input();
	make_edge_table();
	active_list(); 	
	
}

int main(int argc,char **argv){
	
	polygonFillingAlgorithm();
	
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (500,500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("points and lines");
	init2D(0.0,0.0,0.0);
	glutDisplayFunc(active_list);
	glutMainLoop();
	
	
	
	return 0;
}
