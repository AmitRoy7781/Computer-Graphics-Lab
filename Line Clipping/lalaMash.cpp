#include <GL/glut.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

const int Wi = 640, He = 480;
int xmax = 319, xmin = -320, ymax = 239, ymin = -240;
int TOP = 8, BOTTOM = 4, RIGHT = 2, LEFT = 1;
int initx0, initx1, inity0, inity1;

void Init(void) {
	glClearColor(0.0, 0.0 , 0.0 , 1.0);
}

double doubleRand() {
	return double(rand()) / (double(RAND_MAX) + 1.0);
}

int intRand() {
	return rand();
}


void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-Wi / 2, Wi / 2 - 1, -He / 2, He / 2 - 1);
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

int make_code(int x, int y) {
	int code = 0;

	if (y > 100) code += TOP;
	else if (y < -100) code += BOTTOM;
	if (x > 140) code += RIGHT;
	else if (x < -140) code += LEFT;
	return code;
}

void drawLine(int x0, int y0, int x1, int y1) {
	int zone = getZone(x0, y0, x1, y1);
	eightWaySymmetry(x0, y0, zone);
	eightWaySymmetry(x1, y1, zone);
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = 2 * dy - dx;
	int de = 2 * dy;
	int dne = 2 * (dy - dx);
	int x = x0;
	int y = y0;

	reverseEightWaySymmetry(x, y, zone);
	glVertex2i(x, y);
	eightWaySymmetry(x, y, zone);

	while (x < x1) {
		if (d < 0) {
			x++;
			d += de;
		}
		else {
			x++;
			y++;
			d += dne;
		}
		reverseEightWaySymmetry(x, y, zone);
		glVertex2i(x, y);
		eightWaySymmetry(x, y, zone);
	}
}

void drawGrid() {
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);

	int border = 120;
	glVertex2i(-140, ymin);
	glVertex2i(-140, ymax);
	glVertex2i(140, ymin);
	glVertex2i(140, ymax);

	glVertex2i(xmin, 100);
	glVertex2i(xmax, 100);
	glVertex2i(xmin, -100);
	glVertex2i(xmax, -100);
	glEnd();
}

pair<int, int> find_t(int x0, int y0, int x1, int y1, float t) {
	int retx = x0 + t * (x1 - x0);
	int rety = y0 + t * (y1 - y0);
	return make_pair(retx, rety);
}

void cyrusBeakClipLine(int x0, int y0, int x1, int y1) {
	int xxmax = 140, xxmin = -140, yymax = 100, yymin = -100;
	float t, temax = 0.0, tlmin = 1.0;


	t = -((float)(y0 - yymax) / (y1 - y0));
	if (y1 >= y0) {
		if (t < tlmin) {
			tlmin = t;
		}
	}
	else {
		if (t > temax) {
			temax = t;
		}
	}

	t = -((float)(y0 - yymin) / (y1 - y0));
	if (y0 >= y1) {
		if (t < tlmin) {
			tlmin = t;
		}
	}
	else {
		if (t > temax) {
			temax = t;
		}
	}

	t = -((float)(x0 - xxmax) / (x1 - x0));
	if (x1 >= x0) {
		if (t < tlmin) {
			tlmin = t;
		}
	}
	else {
		if (t > temax) {
			temax = t;
		}
	}

	t = -((float)(x0 - xxmin) / (x1 - x0));
	if (x0 >= x1) {
		if (t < tlmin) {
			tlmin = t;
		}
	}
	else {
		if (t > temax) {
			temax = t;
		}
	}

	printf("tlmin: %.2f, temax: %.2f\n", tlmin, temax);

	if (temax >= 0.0 && tlmin <= 1.0 && tlmin >= temax) {
		printf("Accepted\n");
		pair<int, int> p0 = find_t(x0, y0, x1, y1, tlmin);
		pair<int, int> p1 = find_t(x0, y0, x1, y1, temax);

		glEnd();
		glPointSize(4);
		glColor3f(1.0, 1.0, 1.0);

		glBegin(GL_POINTS);
		glVertex2i(p0.first, p0.second);
		glVertex2i(p1.first, p1.second);
		glEnd();
		glPointSize(1);
		glBegin(GL_POINTS);

		glColor3f(1.0, 1.0, 0.0);
		drawLine(p0.first, p0.second, p1.first, p1.second);

		glColor3f(0.5, 0.5, 0.5);
		p0 = find_t(x0, y0, x1, y1, tlmin);
		p1 = find_t(x0, y0, x1, y1, 1.0);
		//drawLine(p0.first, p0.second, p1.first, p1.second);

		p0 = find_t(x0, y0, x1, y1, 0);
		p1 = find_t(x0, y0, x1, y1, temax);
		//drawLine(p0.first, p0.second, p1.first, p1.second);
	}
	else {
		printf("Rejected\n");
		glColor3f(1.0, 0.0, 0.0);
		drawLine(x0, y0, x1, y1);
	}
}

void display(void ) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	drawGrid();

	int cnt = 10;


	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);

	while (cnt--) {
		int x0 = intRand(), y0 = intRand(), x1 = intRand(), y1 = intRand();
		initx0 = x0;
		initx1 = x1;
		inity0 = y0;
		inity1 = y1;
		if (x0 == x1) x1 += 1;
		if (y0 == y1) y1 += 1;
		glColor3f(0.5, 0.5, 0.5);
		drawLine(x0, y0, x1, y1);
		cyrusBeakClipLine(x0, y0, x1, y1);
	}

	glEnd();
	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(Wi, He);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Cyrus Beak Line Clip");

	Init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}