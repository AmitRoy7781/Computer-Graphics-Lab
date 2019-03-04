#include <GL/glut.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

const int Wi = 640, He = 480, maxi = 11111;
int xmax = 319, xmin = -320, ymax = 239, ymin = -240;
int TOP = 8, BOTTOM = 4, RIGHT = 2, LEFT = 1;
int YMIN = INT_MAX, YMAX = INT_MIN;
int num_points;

map<pair<int, int>, int> mp, mp1;

struct Point {
	int selfx;
	int y;
	int minx;
	int miny;
	int dx;
	int dy;
	int othery;
};

std::vector<Point> edge_table[maxi];
std::vector<pair<int, int> > points;
std::vector<int> v[maxi];

void Init(void) {
	glClearColor(0.0, 0.0 , 0.0 , 1.0);
}



void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-5, 74, -5, 54);
	//gluOrtho2D(-Wi / 2, Wi / 2 - 1, -He / 2, He / 2 - 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reverseEightWaySymmetry(int &x, int &y, int zone) {
	if (zone == 1) {
		swap(x, y);
	}
	else if (zone == 2) {
		swap(x, y);
		x *= -1;
	}
	else if (zone == 3) {
		x *= -1;
	}
	else if (zone == 4) {
		x *= -1;
		y *= -1;
	}
	else if (zone == 5) {
		swap(x, y);
		x *= -1;
		y *= -1;
	}
	else if (zone == 6) {
		swap(x, y);
		y *= -1;
	}
	else if (zone == 7) {
		y *= -1;
	}

	return;
}

void eightWaySymmetry(int &x, int &y, int zone) {
	if (zone == 1) {
		swap(x, y);
	}
	else if (zone == 2) {
		x *= -1;
		swap(x, y);
	}
	else if (zone == 3) {
		x *= -1;
	}
	else if (zone == 4) {
		x *= -1;
		y *= -1;
	}
	else if (zone == 5) {
		x *= -1;
		y *= -1;
		swap(x, y);
	}
	else if (zone == 6) {
		y *= -1;
		swap(x, y);
	}
	else if (zone == 7) {
		y *= -1;
	}

	return;
}

int getZone(int x1, int y1, int x0, int y0) {
	int dy = y0 - y1;
	int dx = x0 - x1;

	if (dx >= 0 && dy >= 0) {
		if (dx >= dy) {
			return 0;
		} else {
			return 1;
		}
	} else if (dx < 0 && dy < 0) {
		if (abs(dx) >= abs(dy)) {
			return 4;
		} else {
			return 5;
		}
	} else if (dx < 0 && dy >= 0) {
		if (abs(dx) >= dy) {
			return 3;
		} else {
			return 2;
		}
	} else {
		if (abs(dx) >= abs(dy)) {
			return 7;
		} else {
			return 6;
		}
	}

}

void take_input() {
	freopen("in.txt", "r", stdin);

	scanf("%d", &num_points);
	for (int i = 0; i < num_points; i++) {
		int xx, yy;
		scanf("%d %d", &xx, &yy);
		mp1[make_pair(xx, yy)] = -1;
		YMIN = min(YMIN, yy);
		YMAX = max(YMAX, yy);
		points.push_back(make_pair(xx, yy));
	}

}



bool sort_edge_table1(const Point &a, const Point &b) {
	if (a.selfx < b.selfx) return true;
	if (a.selfx == b.selfx) return true;
	else return false;
}


void construct_edge_table() {

	for (int i = 0; i < num_points; i++) {
		int j = (i + 1) % num_points;
		pair<int, int> a = make_pair(points[i].first, points[i].second);
		pair<int, int> b = make_pair(points[j].first, points[j].second);
		if (a.second == b.second) continue;
		Point temp;
		double tempx;
		if (a.second < b.second) {
			tempx = a.first;
			mp[make_pair(a.first, a.second)]++;
			mp[make_pair(b.first, b.second - 1)]++;

			for (int j = a.second; j < b.second; j++) {
				temp.selfx = floor(tempx);
				temp.othery = j;
				temp.dx = a.first - b.first;
				temp.dy = a.second - b.second;
				edge_table[j].push_back(temp);

				tempx += ((double)temp.dx / (double)temp.dy);

			}

		}
		else {
			mp[make_pair(b.first, b.second)]++;
			mp[make_pair(a.first, a.second-1)]++;

			tempx = b.first;

			for (int j = b.second; j < a.second; j++) {
				temp.selfx = floor(tempx);
				temp.othery = j;
				temp.dx = a.first - b.first;
				temp.dy = a.second - b.second;
				edge_table[j].push_back(temp);

				tempx += ((double)temp.dx / (double)temp.dy);
			}

		}
	}

	for (int i = YMIN; i <= YMAX; i++) {
		sort(edge_table[i].begin(), edge_table[i].end(), sort_edge_table1);
	}

	for (int i = 0; i < YMAX; i++) {
		printf("%d: ", i);
		if (edge_table[i].size() == 0) printf("lambda\n");
		for (int j = 0; j < edge_table[i].size(); j++) {
			double m = ((double)edge_table[i][j].dx / (double)edge_table[i][j].dy);
			printf("%d %d %.3f -> ", edge_table[i][j].othery, edge_table[i][j].selfx, m);

		}
		if (edge_table[i].size() > 0) printf("lambda\n");
	}
}


void drawGrid() {
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
	glVertex2i(-320, 0);
	glVertex2i(319, 0);
	glVertex2i(0, 239);
	glVertex2i(0, -240);
	glEnd();
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	drawGrid();

	glPointSize(4);

	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);

	

	for (int i = 0; i < YMAX; i++) {
		for (int j = 0; j < edge_table[i].size(); j++) {
			double m = ((double)edge_table[i][j].dx / (double)edge_table[i][j].dy);
			int xx = edge_table[i][j].selfx;
			int yy = i;
			//printf("%d %d %.3f -> ", edge_table[i][j].othery, edge_table[i][j].selfx, m);
			if (mp[make_pair(edge_table[i][j].selfx, i)] == 1) {
				glEnd();
				// glPointSize(4);
				glColor3f(1.0, 1.0, 1.0);

				glBegin(GL_POINTS);
				glVertex2i(xx, yy);
				glEnd();
				// glPointSize(1);
				glBegin(GL_POINTS);
			}
			else if (mp[make_pair(edge_table[i][j].selfx, i)] == 2) {
				glEnd();
				// glPointSize(4);
				glColor3f(1.0, 1.0, 0.0);

				glBegin(GL_POINTS);
				glVertex2i(xx, yy);
				glEnd();
				// glPointSize(1);
				glBegin(GL_POINTS);
			} else {
				glEnd();
				// glPointSize(3);
				glColor3f(0.5,0.5,0.5);///Ash 

				glBegin(GL_POINTS);
				glVertex2i(xx, yy);
				glEnd();
				// glPointSize(1);
				glBegin(GL_POINTS);
			}
		}
	}

	for (int i = 0; i < points.size(); i++) {
		int xx = points[i].first;
		int yy = points[i].second;
		if (mp[make_pair(xx, yy)] <= 0) {
			glEnd();
			// glPointSize(4);
			glColor3f(1.0, 0.0, 0.0);

			glBegin(GL_POINTS);
			glVertex2i(xx, yy);
			glEnd();
			// glPointSize(1);
			glBegin(GL_POINTS);
		}
	}

	glEnd();
	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);

	take_input();
	construct_edge_table();

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(Wi, He);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Polygon Clip");

	Init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
