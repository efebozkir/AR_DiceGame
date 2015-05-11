
#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
//#include <GL/gl.h>

#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(100 * x)
#endif

#include <math.h>

#include <iomanip>
#include <iostream>
#include <stdarg.h> 




#include "ScoreBoardLogic.h"
/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

ScoreBoardLogic *	scoreBoardLogic=new ScoreBoardLogic();


GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
static GLuint texture[6];  
GLuint buttonTexture;

int angleX, angleY, angleZ=0;  
float spin;



//  Draws a string at the specified coordinates.
//-------------------------------------------------------------------------
void printw (float x, float y, float z, char* format, ...)
{
    va_list args;   //  Variable argument list
    int len;        // String length
    int i;          //  Iterator
    char * text;    // Text
 
    //  Initialize a variable argument list
    va_start(args, format);
 
    //  Return the number of characters in the string referenced the list of arguments.
    // _vscprintf doesn't count terminating '\0' (that's why +1)
    len = _vscprintf(format, args) + 1;
 
    //  Allocate memory for a string of the specified size
    text = (char*)malloc(len * sizeof(char));
 
    //  Write formatted output using a pointer to the list of arguments
    vsprintf_s(text, len, format, args);
 
    //  End using variable argument list
    va_end(args);
 
    //  Specify the raster position for pixel operations.
	glColor3f(0.5, 0.5, 0.5);
    glRasterPos3f (x, y, z);
 
    //  Draw the characters one by one
    for (i = 0; text[i] != '\0'; i++)
    glutBitmapCharacter(font_style, text[i]);
 
    //  Free the allocated memory for the string
    free(text);
}

void drawScoreBoard(float x1, float y1, float x2, float y2)
{
	
	//glDisable(GL_CULL_FACE); 
	
	//glRectf(x1,y1, x2, y2); 
	glPushMatrix();
	glTranslatef(0.0, 1.5, 0.0);
   
	glColor3f(0.0f,0.0f,1.0f); //blue color
	

	
	glColor4f(1.0,1.0,1.0, 0.0); // Red color 
	printw (x1, y1+0.7, 1, "%s    %s", "Player1", "Player2");
	


	printw (x1, y1+1.5, 1, "Score1: %d    Score2: %d", scoreBoardLogic->getPlayer1Score(), scoreBoardLogic->getPlayer2Score());
	
	printw (x1, y1+2.5, 1, "Round: %d",scoreBoardLogic->getRound());

	printw(x1, y1+3.5, 1, "Total Point1: %d, Total Point2: %d", scoreBoardLogic->player1->getTotalPoint(), scoreBoardLogic->player2->getTotalPoint());
	
	 
	glPopMatrix();
	///glEnable(GL_CULL_FACE); 
	
}


void drawSphere(double r, int lats, int longs) {
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r *  cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}


void solidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	glBegin(GL_LINE_LOOP);
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);

	gluCylinder(quadric, base, 0, height, slices, stacks);

	gluDeleteQuadric(quadric);
	glEnd();
}

void drawCube(){

	glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  float x0 = -1.0, y0 = -1, x1 = 1, y1 = 1, z0 = 1;
  float face[6][4][3] =  { {{x0, y0, z0}, {x1, y0, z0}, {x1, y1, z0}, {x0, y1, z0}},  //front
    {{x0, y1, -z0}, {x1, y1, -z0}, {x1, y0, -z0}, {x0, y0, -z0}},    //back
    {{x1, y0, z0}, {x1, y0, -z0}, {x1, y1, -z0}, {x1, y1, z0}},    //right 
    {{x0, y0, z0}, {x0, y1, z0}, {x0, y1, -z0}, {x0, y0, -z0}},    //left 
    {{x0, y1, z0}, {x1, y1, z0}, {x1, y1, -z0}, {x0, y1, -z0}},    //top 
    {{x0, y0, z0}, {x0, y0, -z0}, {x1, y0, -z0}, {x1, y0, z0}}    //bottom 
  };





  
  glEnable( GL_CULL_FACE );
  glCullFace ( GL_BACK );
  
  glPushMatrix(); 
  glTranslatef(0, 0.0, 0.0 );
  glRotatef(0, 1.0, 1.0, 0.0);
  glRotatef(0, 1.0, 0.0, 0.0);   
  glRotatef(0, 0.0, 1.0, 0.0);     
  glRotatef( 0, 0.0, 0.0, 1.0);
  


  for ( int i = 0; i < 6; ++i ) 
  {      //draw cube 
	glBindTexture(GL_TEXTURE_2D, buttonTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3fv ( face[i][0] );   
	glTexCoord2f(1.0, 0.0); glVertex3fv ( face[i][1] );  
	glTexCoord2f(1.0, 1.0); glVertex3fv ( face[i][2] );
	glTexCoord2f(0.0, 1.0); glVertex3fv ( face[i][3] );
	glEnd();
  }
  glPopMatrix();
	  


  
  glFlush();
  glDisable(GL_TEXTURE_2D);
}

void thiswillDrawDices(){
glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  float x0 = -1.0, y0 = -1, x1 = 1, y1 = 1, z0 = 1;
  float face[6][4][3] =  { {{x0, y0, z0}, {x1, y0, z0}, {x1, y1, z0}, {x0, y1, z0}},  //front
    {{x0, y1, -z0}, {x1, y1, -z0}, {x1, y0, -z0}, {x0, y0, -z0}},    //back
    {{x1, y0, z0}, {x1, y0, -z0}, {x1, y1, -z0}, {x1, y1, z0}},    //right 
    {{x0, y0, z0}, {x0, y1, z0}, {x0, y1, -z0}, {x0, y0, -z0}},    //left 
    {{x0, y1, z0}, {x1, y1, z0}, {x1, y1, -z0}, {x0, y1, -z0}},    //top 
    {{x0, y0, z0}, {x0, y0, -z0}, {x1, y0, -z0}, {x1, y0, z0}}    //bottom 
  };
  glEnable( GL_CULL_FACE );
  glCullFace ( GL_BACK );
  

  glPushMatrix();

  
 
  
  

  
  //rotate along z-axis
 
  for ( int i = 0; i < 6; ++i ) {      //draw cube with texture images
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3fv ( face[i][0] );   
    glTexCoord2f(1.0, 0.0); glVertex3fv ( face[i][1] );  
    glTexCoord2f(1.0, 1.0); glVertex3fv ( face[i][2] );
    glTexCoord2f(0.0, 1.0); glVertex3fv ( face[i][3] );
    glEnd();
  }

  glPopMatrix();
  glFlush();
  //glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
