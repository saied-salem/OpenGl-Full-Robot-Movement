#include <stdlib.h>
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <math.h>
#define speed 0.1
#define PI 3.14159265358979323846
#define INCREASE 0
#define DECREASE 1
#define RIGHT 0
#define LEFT 1
#define X 0
#define Y 1
#define Z 2

int moving, startx, starty;
int fingerBase = 0, fingerUp = 0;

int shoulder[2][3] = {{0,0,0},{0,0,0}};
int fumer[2][3] = {{0,0,0},{0,0,0}};
int elbow[2] = {0, 0};
int tibia[2] = {0, 0};
double eye[] = { 0, 0, 20 };
double center[] = { 0, 0, 1 };
double up[] = { 0, 1, 0 };


GLfloat angle = 0.0;    /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */

/*
 * Input: flangControl The angle of rotation, in degrees.
 * Description : Drawing a flang (scaled box at the tip of the base finger)
*/

void init(void) {
    //glClearColor(0.0, 0.0, 0.0, 0.0); glShadeModel(GL_FLAT);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(65.0, (GLfloat)1024 / (GLfloat)869, 1.0, 60.0);
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];

}

void Left()
{
	// implement camera rotation arround vertical window screen axis to the left
	// used by mouse and left arrow
	rotatePoint(up, PI / 8, eye);
}

void Right()
{
	// implement camera rotation arround vertical window screen axis to the right
	// used by mouse and right arrow
	rotatePoint(up, -PI / 8, eye);
}

void Up_cam()
{
	// implement camera rotation arround horizontal window screen axis +ve
	// used by up arrow
	double horizontal[3];
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	crossProduct(up, direction, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 8, eye);
	rotatePoint(horizontal, PI / 8, up);
	normalize(up);
}

void Down_cam()
{
	// implement camera rotation arround horizontal window screen axis 
	// used by down arrow
	double horizontal[3];
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	crossProduct(up, direction, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 8, eye);
	rotatePoint(horizontal, -PI / 8, up);
	normalize(up);
}

void moveForward()
{
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	//center[0] += direction[0] * speed;
	//center[1] += direction[1] * speed;
	//center[2] += direction[2] * speed;
}

void moveBack()
{
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	eye[0] -= direction[0] * speed;
	eye[1] -= direction[1] * speed;
	eye[2] -= direction[2] * speed;
	//center[0] -= direction[0] * speed;
	//center[1] -= direction[1] * speed;
	//center[2] -= direction[2] * speed;
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	//--------------------------------
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	//----Body----

	glTranslatef(0.0, 0.0, 0.0);

	glPushMatrix();
	glScalef(2.0, 4.0, 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	//----Head----
	glPushMatrix();
	glTranslatef(0, 3.0, 0.0);
	glTranslatef(0.0, 0.0, 0.0);

	glScalef(0.25, 0.25, 0.25);
	glutWireSphere(3.0, 20, 20); // double radius , slices , stacks
	glPopMatrix();

	//----Right Arm----

	glPushMatrix();
	//----Right shoulder----
	glTranslatef(-1.2, 1.7, 0.0);
	glRotatef((GLfloat)shoulder[RIGHT][X], -1.0, 0.0, 0.0);
	glRotatef((GLfloat)shoulder[RIGHT][Y], 0.0, 1.0, 0.0);
	glRotatef((GLfloat)shoulder[RIGHT][Z], 0.0, 0.0, 1.0);
	glTranslatef(-0.1, -0.5, 0.0);

	glPushMatrix();
	glScalef(0.5, 1.5, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	//----Right elbow----
	glPushMatrix();
	glTranslatef(0.0, -0.75, 0.0);
	glRotatef((GLfloat)elbow[RIGHT], 0.0, 0.0, 1.0);
	glTranslatef(0.0, -0.75, 0.0);
	glPushMatrix();
		glScalef(0.5, 1.5, 1.0);
		glutWireCube(1.0);
	glPopMatrix();


	// ----Right 4 fingers----
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
			glTranslatef(-0.2, -0.8, 0.45 - 0.3 * i);
			glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
			glTranslatef(0, -0.1, 0);

			glPushMatrix();
				glScalef(0.1, 0.3, 0.1);
				glutWireCube(1);
			glPopMatrix();

			glTranslatef(0, -0.2, 0);
			glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
			glTranslatef(0, -0.1, 0);

			glPushMatrix();
				glScalef(0.1, 0.3, 0.1);
				glutWireCube(1);
			glPopMatrix();

		glPopMatrix();
	}
	// ----Right Thumb----
		glPushMatrix();

			glTranslatef(0.2, -0.8, 0.0);
			glRotatef((GLfloat)fingerBase, 0.0, 0.0, -1.0);
			glTranslatef(0, -0.1, 0);

			glPushMatrix();
				glScalef(0.1, 0.3, 0.1);
				glutWireCube(1);
			glPopMatrix();

			glTranslatef(0, -0.2, 0);
			glRotatef((GLfloat)fingerUp, 0.0, 0.0, -1.0);
			glTranslatef(0, -0.1, 0);

			glPushMatrix();
				glScalef(0.1, 0.3, 0.1);
				glutWireCube(1);
			glPopMatrix();

		glPopMatrix();

		glPopMatrix();

	glPopMatrix();

	//----Right Leg----
	glPushMatrix();

		glTranslatef(-0.72, -2.0, 0.0);
		glRotatef((GLfloat)fumer[RIGHT][X], -1.0, 0.0, 0.0);
		glRotatef((GLfloat)fumer[RIGHT][Y], 0.0, 1.0, 0.0);
		glRotatef((GLfloat)fumer[RIGHT][Z], 0.0, 0.0, 1.0);
		glTranslatef(0.0, -1.0, 0.0);


		glPushMatrix();
			glScalef(0.6, 2.0, 1.0);
			glutWireCube(1.0);
		glPopMatrix();

		//----Right Tibia----

		glPushMatrix();
			glTranslatef(0.0, -1.0, 0.0);
			glRotatef((GLfloat)tibia[RIGHT], 1.0, 0.0, 0.0);
			glTranslatef(0.0, -1.0, 0.0);

			glPushMatrix();
				glScalef(0.6, 2.0, 1.0);
				glutWireCube(1.0);
			glPopMatrix();
			// ----Right Foot----
			glPushMatrix();
				glTranslatef(-0.23, -1.6, 0.0);
				glTranslatef(0.2, 0.3, 0);

				glPushMatrix();
					glScalef(0.6, 0.5, 2.0);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	//----Left Arm----
	glPushMatrix();
		//----Left shoulder----
		glTranslatef(1.2, 1.7, 0.0);
		glRotatef((GLfloat)shoulder[LEFT][X], -1.0, 0.0, 0.0);
		glRotatef((GLfloat)shoulder[LEFT][Y], 0.0, -1.0, 0.0);
		glRotatef((GLfloat)shoulder[LEFT][Z], 0.0, 0.0, -1.0);
	glTranslatef(0.1, -0.5, 0.0);

	glPushMatrix();
		glScalef(0.5, 1.5, 1.0);
		glutWireCube(1.0);
	glPopMatrix();
	//----Left elbow----
	glPushMatrix();
		glTranslatef(0.0, -0.75, 0.0);
		glRotatef((GLfloat)elbow[LEFT], 0.0, 0.0, -1.0);
		glTranslatef(0.0, -0.75, 0.0);
		glPushMatrix();
		glScalef(0.5, 1.5, -1.0);
		glutWireCube(1.0);
	glPopMatrix();


	// ----Left 4 fingers----
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(0.2, -0.8, 0.45 - 0.3 * i);
		glRotatef((GLfloat)fingerBase, 0.0, 0.0, -1.0);
		glTranslatef(0, -0.1, 0);

		glPushMatrix();
		glScalef(0.1, 0.3, 0.1);
		glutWireCube(1);
		glPopMatrix();

		glTranslatef(0, -0.2, 0);
		glRotatef((GLfloat)fingerUp, 0.0, 0.0, -1.0);
		glTranslatef(0, -0.1, 0);

		glPushMatrix();
		glScalef(0.1, 0.3, 0.1);
		glutWireCube(1);
		glPopMatrix();

		glPopMatrix();
	}
	// ----Left Thumb----
	glPushMatrix();

	glTranslatef(-0.2, -0.8, 0.0);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0, -0.1, 0);

	glPushMatrix();
	glScalef(0.1, 0.3, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0, -0.2, 0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0, -0.1, 0);

	glPushMatrix();
	glScalef(0.1, 0.3, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//----Left Leg----	

	glPushMatrix();

	glTranslatef(0.72, -2.0, 0.0);
	glRotatef((GLfloat)fumer[LEFT][X], -1.0, 0.0, 0.0);
	glRotatef((GLfloat)fumer[LEFT][Y], 0.0, -1.0, 0.0);
	glRotatef((GLfloat)fumer[LEFT][Z], 0, 0.0, -1.0);
	glTranslatef(0.0, -1.0, 0.0);


	glPushMatrix();
	glScalef(0.6, 2.0, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	//----Left Tibia----
	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef((GLfloat)tibia[LEFT], 1.0, 0.0, 0.0);
	glTranslatef(0.0, -1.0, 0.0);

	glPushMatrix();
	glScalef(0.6, 2.0, 1.0);
	glutWireCube(1.0);
	glPopMatrix();


	// ----Left Foot----
	glPushMatrix();

	glTranslatef(0, -1.6, 0.0);
	glTranslatef(0.0, 0.3, 0);

	glPushMatrix();
	glScalef(0.6, 0.5, 2.0);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//---------------------------------
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void control(int& control_var, int type, int low_limit, int upper_limit, int step){
	std::cout << control_var << std::endl;
	if(type == INCREASE ){
		if (control_var >= low_limit && control_var < upper_limit) {
			control_var +=  step;
		}
	}
	else{
		if (control_var > low_limit){
 			control_var -=  step;
		} 
	}
	glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT: Left(); break;
		case GLUT_KEY_RIGHT: Right(); break;
		case GLUT_KEY_UP:    Up_cam(); break;
		case GLUT_KEY_DOWN:  Down_cam(); break;
	}

	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
		case 'a': control(shoulder[LEFT][X], INCREASE, 0, 360, 5); break;
		case 'A': control(shoulder[LEFT][X], DECREASE, 0, 360, 5); break;
		case 'q': control(shoulder[LEFT][Y], INCREASE, 0, 90, 5); break;
		case 'Q': control(shoulder[LEFT][Y], DECREASE, 0, 90, 5); break;
		case 'z': control(shoulder[LEFT][Z], INCREASE, -90, 45, 5); break;
		case 'Z': control(shoulder[LEFT][Z], DECREASE, -90, 45, 5); break;

		case ';': control(shoulder[RIGHT][X], INCREASE, 0, 360, 5); break;
		case ':': control(shoulder[RIGHT][X], DECREASE, 0, 360, 5); break;
		case 'p': control(shoulder[RIGHT][Y], INCREASE, 0, 90, 5); break;
		case 'P': control(shoulder[RIGHT][Y], DECREASE, 0, 90, 5); break;
		case '.': control(shoulder[RIGHT][Z], INCREASE, -90, 45, 5); break;
		case '>': control(shoulder[RIGHT][Z] ,DECREASE, -90, 45, 5); break;

		case 's': control(elbow[LEFT], INCREASE, 0, 90, 5); break;
		case 'S': control(elbow[LEFT], DECREASE, 0, 90, 5); break;

		case 'l': control(elbow[RIGHT], INCREASE, 0, 90, 5); break;
		case 'L': control(elbow[RIGHT], DECREASE, 0, 90, 5); break;

		case 'd': control(fumer[LEFT][X], INCREASE, -125, 125, 5); break;
		case 'D': control(fumer[LEFT][X], DECREASE, -125, 125, 5); break;
		case 'e': control(fumer[LEFT][Y], INCREASE, -45, 45, 5); break;
		case 'E': control(fumer[LEFT][Y], DECREASE, -45, 45, 5); break;
		case 'c': control(fumer[LEFT][Z], INCREASE, -90, 45, 5); break;
		case 'C': control(fumer[LEFT][Z], DECREASE, -90, 45, 5); break;

		case 'k': control(fumer[RIGHT][X], INCREASE, -125, 125, 5); break;
		case 'K': control(fumer[RIGHT][X], DECREASE, -125, 125, 5); break;
		case 'i': control(fumer[RIGHT][Y], INCREASE, -45, 45, 5); break;
		case 'I': control(fumer[RIGHT][Y], DECREASE, -45, -45, 5); break;
		case 'm': control(fumer[RIGHT][Z], INCREASE, -90, 45, 5); break;
		case 'M': control(fumer[RIGHT][Z] ,DECREASE, -90, 45, 5); break;

		case 'f': control(tibia[LEFT], INCREASE, 0, 90, 5); break;
		case 'F': control(tibia[LEFT], DECREASE, 0, 90, 5); break;

		case 'j': control(tibia[RIGHT], INCREASE, 0, 90, 5); break;
		case 'J': control(tibia[RIGHT], DECREASE, 0, 90, 5); break;

		case 't':
			moveForward();
			glutPostRedisplay();
			break;
		case 'y':
			moveBack();
			glutPostRedisplay();
			break;
		case 'g':
			Up_cam();
			glutPostRedisplay();
			break;
		case 'h':
			Down_cam();
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

static void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}


static void motion(int x, int y) {
	    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }

}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
