#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
//#include <GL/glew.h>
#include <iostream>
#include <glut.h>
//#include "ScoreBoardLogic.h"



#include "DrawPrimitives.h"

#include <iomanip>


#include <opencv/cv.h>
#include <opencv/highgui.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "PoseEstimation.h"
#include "MarkerTracker.h"




using namespace std;




cv::VideoCapture cap;

float myHeight;
float myWidth;

int marker1=0x005a;
int marker2=0x1c44;
int zar1=0x0b44;
int zar2=0x1228;
int scoreboard=0x0272;

bool player1Round = false;
bool player2Round= false;

vector<Marker> previousMarkers;

float resultTransposedMatrixButton1[16];
float resultTransposedMatrixButton2[16];
float resultTransposedMatrixDice1[16];
float resultTransposedMatrixDice2[16];
float resultTransposedMatrixScoreBoard[16];


//camera settings
const int camera_width  = 640; 
const int camera_height = 480;
const int virtual_camera_angle = 60;
unsigned char bkgnd[camera_width*camera_height*3];

int angleR=0;






void initVideoStream( cv::VideoCapture &cap ) {
	if( cap.isOpened() )
		cap.release();

	cap.open(0); // open the default camera
	if ( cap.isOpened()==false ) {
		std::cout << "No webcam found, using a video file" << std::endl;
		cap.open("MarkerMovie.mpg");
		if ( cap.isOpened()==false ) {
			std::cout << "No video file found. Exiting."      << std::endl;
			exit(0);
		}
	}

}



  
GLuint LoadTextureRAW( const char * filename)
{
    GLuint texture;
  

 
    unsigned char* data;
    FILE * file;
  
    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
  
    // allocate buffer
  float width = 512;
  float height = 512;
    data = (unsigned char*)malloc( width * height * 3 );
  
    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  glGenTextures(1, &texture);
  glBindTexture( GL_TEXTURE_2D, texture);
  
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_RGB , GL_UNSIGNED_BYTE, data);    // build our texture mipmaps
  
    // allocate a texture name
  
    // free buffer
    free( data );  
return texture;
}



/* program & OpenGL initialization */
void initGL(int argc, char *argv[])
{
	// initialize the GL library
	
	texture[0] = LoadTextureRAW ("image (1).raw");
  
  texture[1] = LoadTextureRAW ("image (2).raw");
  
  texture[2] = LoadTextureRAW ("image (3).raw");
  
  texture[3] = LoadTextureRAW ("image (4).raw");

  texture[4] = LoadTextureRAW ("image (5).raw");
  texture[5] = LoadTextureRAW ("image (6).raw");

  buttonTexture=LoadTextureRAW("button_texture.raw");


// Added in Exercise 8 - End *****************************************************************
    // pixel storage/packing stuff
    glPixelStorei( GL_PACK_ALIGNMENT,   1 ); // for glReadPixels?
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); // for glTexImage2D?
    glPixelZoom( 1.0, -1.0 );
// Added in Exercise 8 - End *****************************************************************

    // enable and set colors
    glEnable( GL_COLOR_MATERIAL );
    glClearColor( 0, 0, 0, 1.0 );

	// enable and set depth parameters
	glEnable( GL_DEPTH_TEST );
	glClearDepth( 1.0 );

	// light parameters
	GLfloat light_pos[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_amb[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_dif[] = { 0.7, 0.7, 0.7, 1.0 };

	// enable lighting
	glLightfv( GL_LIGHT0, GL_POSITION, light_pos );
	glLightfv( GL_LIGHT0, GL_AMBIENT,  light_amb );
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_dif );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );


}


void display( GLFWwindow* window, const cv::Mat &img_bgr, vector<Marker>& markers ) 
{
	/////?????
	bool isbutton1 = false;
	bool isbutton2 = false;
	bool isdice1 = false;
	bool isdice2 = false;
	bool isscoreBoard = false;


	float resultTransposedMatrix[16];
	float resultmatrix_005a[16];
	float resultmatrix_1c44[16];
	float resultmatrix_0272[16];
	float resultmatrix_0b44[16];//zar1
	float resultmatrix_1228[16];//zar2

	
	for(int i=0; i<markers.size(); i++)
	{
		const int code=markers[i].code;

		if(code==marker1)
		{
			isbutton1 = true;
			for(int j=0; j<16; j++)
			{
				resultmatrix_005a[j]=markers[i].resultMatrix[j];
			
			}
		}
		else if(code==marker2) //button2
		{
						isbutton2 = true;
			for(int j=0; j<16; j++)
				{
					resultmatrix_1c44[j]=markers[i].resultMatrix[j];
			
				}
		}

		else if(code==scoreboard) //scoreboard
		{
						isscoreBoard = true;

			for(int j=0; j<16; j++)
				{
					resultmatrix_0272[j]=markers[i].resultMatrix[j];
			
				}
		}

		else if(code==zar1)
		{
						isdice1 = true;

			for(int j=0; j<16; j++)
				{
					resultmatrix_0b44[j]=markers[i].resultMatrix[j];
			
				}
		
		}
		else if(code==zar2)
		{
						isdice2 = true;

			for(int j=0; j<16; j++)
				{
					resultmatrix_1228[j]=markers[i].resultMatrix[j];
			
				}
		
		}
	}



	if(previousMarkers.size()==4&& markers.size()==3 )
	{
			
		for(int i=0;i<markers.size();i++)
		{
		
			if(markers[i].code == marker1)
			{
				if(!player2Round)
				{
					scoreBoardLogic->player2->rollDice();
				}
				player2Round= true;
			}
			else if(markers[i].code == marker2)
			{
				if(!player1Round)
				{
					scoreBoardLogic->player1->rollDice();


					
					

				}
					player1Round = true;
			}
		}
		if(player1Round && player2Round)
		{
			cout<<"update"<<endl;
						scoreBoardLogic->updateScoreBoard();
						player1Round = false;
						player2Round = false;
		}
		

	}
	if(markers.size()!=0)
	{
		previousMarkers = markers;
	}


// Added in Exercise 8 - Start *****************************************************************
	memcpy( bkgnd, img_bgr.data, sizeof(bkgnd) );
// Added in Exercise 8 - End *****************************************************************

	int width0, height0;
	glfwGetFramebufferSize(window, &width0, &height0);
//	reshape(window, width, height);

    // clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
// Added in Exercise 8 - Start *****************************************************************
    // draw background image
    
	
	
	
	
	glDisable( GL_DEPTH_TEST );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0.0, camera_width, 0.0, camera_height );

    glRasterPos2i( 0, camera_height-1 );
    glDrawPixels( camera_width, camera_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bkgnd );

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);

	 

	
// Added in Exercise 8 - End *****************************************************************


	
	
    // move to marker-position
    glMatrixMode( GL_MODELVIEW );
    
	

	if(isbutton1)
	{
	for(int k=0; k<4; ++k)
	{
		for(int t=0; t<4; ++t)
		{
			resultTransposedMatrixButton1[k*4+t]=resultmatrix_005a[t*4+k];

		
		}
	
	}
	glLoadMatrixf( resultTransposedMatrixButton1 );
	//glLoadTransposeMatrixf( resultMatrix );
    //glRotatef( -90, 1, 0, 0 );
	glScalef(0.02, 0.02, 0.02);
	//drawCube(myHeight, myWidth);
	//glTranslatef(-1.0, 0.0, 0.0);
	drawCube();
	//cout<<"Marker Height:"<<myHeight<<" Marker Width:"<<myWidth<<endl;
	}
	if(isbutton2)
	{
	for(int k=0; k<4; ++k)
	{
		for(int t=0; t<4; ++t)
		{
			resultTransposedMatrixButton2[k*4+t]=resultmatrix_1c44[t*4+k];
		}
	
	}

	glLoadMatrixf( resultTransposedMatrixButton2 );
	//glLoadTransposeMatrixf( resultMatrix );
    //glRotatef( -90, 1, 0, 0 );
	glScalef(0.02, 0.02, 0.02);

	//drawCube(myHeight, myWidth);
	//glTranslatef(-1.0, 0.0, 0.0);
	drawCube();
	}
	
	
	if(isdice1)
	{
	for(int k=0; k<4; ++k)
	{
		for(int t=0; t<4; ++t)
		{
			resultTransposedMatrixDice1[k*4+t]=resultmatrix_0b44[t*4+k];
		}
	}
	glLoadMatrixf( resultTransposedMatrixDice1 );
	//glLoadTransposeMatrixf( resultMatrix );
    //glRotatef( -90, 1, 0, 0 );
		
	//drawCube(myHeight, myWidth);
	//glTranslatef(-1.0, 0.0, 0.0);
	
	glScalef(0.02, 0.02, 0.02);
	if(scoreBoardLogic->player1->getScore() == 1 )
	{
		//glRotatef(90, 1.0, 0.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);

	}
	else if(scoreBoardLogic->player1->getScore() ==2)
	{
		glRotatef(0, 1.0, 0.0, 0.0);
	}
		else if(scoreBoardLogic->player1->getScore() ==3)
	{
		//glRotatef(180, 0.0, 1.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
	
	}
		else if(scoreBoardLogic->player1->getScore() ==4)
	{
		//glRotatef(90, -1.0, 0.0, 0.0);
		glRotatef(90, 0.0, -1.0, 0.0);
	}
		else if(scoreBoardLogic->player1->getScore() ==5)
	{
		//glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(90, -1.0, 0.0, 0.0);
	}
	else if(scoreBoardLogic->player1->getScore() ==6)
	{
		//glRotatef(90, 0.0, -1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
	}

	thiswillDrawDices();
	//cout<<"Marker Height:"<<myHeight<<" Marker Width:"<<myWidth<<endl;
	}

	if(isdice2)
	{
	for(int k=0; k<4; ++k)
	{
		for(int t=0; t<4; ++t)
		{
			resultTransposedMatrixDice2[k*4+t]=resultmatrix_1228[t*4+k];
		}
	
	}
	glLoadMatrixf( resultTransposedMatrixDice2 );
	//glLoadTransposeMatrixf( resultMatrix );
    //glRotatef( -90, 1, 0, 0 );
		
	//drawCube(myHeight, myWidth);
	//glTranslatef(-1.0, 0.0, 0.0);
	glScalef(0.02, 0.02, 0.02);
	if(scoreBoardLogic->player2->getScore() == 1 )
	{	
		//glRotatef(90, 1.0, 0.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);
	}
	else if(scoreBoardLogic->player2->getScore()==2)
	{
		//glRotatef(0, 1.0, 0.0, 0.0);
		glRotatef(0, 1.0, 0.0, 0.0);
	}
		else if(scoreBoardLogic->player2->getScore() ==3)
	{
		//glRotatef(180, 0.0, 1.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
	}
		else if(scoreBoardLogic->player2->getScore()==4)
	{
		//glRotatef(90, -1.0, 0.0, 0.0);
		glRotatef(90, 0.0, -1.0, 0.0);
	}
		else if(scoreBoardLogic->player2->getScore() ==5)
	{
		//glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(90, -1.0, 0.0, 0.0);
	}
	else if(scoreBoardLogic->player2->getScore() ==6)
	{
		//glRotatef(90, 0.0, -1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
	}

	thiswillDrawDices();
	}
	
	if(isscoreBoard)
	{
	for(int k=0; k<4; ++k)
	{
		for(int t=0; t<4; ++t)
		{
			resultTransposedMatrixScoreBoard[k*4+t]=resultmatrix_0272[t*4+k];
		}
	
	}


	glLoadMatrixf( resultTransposedMatrixScoreBoard );
	glScalef(0.01, 0.01, 0.01);




	drawScoreBoard(0.00,0.00, 0.50, 0.50);

	}

	glLoadIdentity();
	
	
}


void reshape( GLFWwindow* window, int width, int height ) {

	// set a whole-window viewport
	glViewport( 0, 0, (GLsizei)width, (GLsizei)height );

	// create a perspective projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Note: Just setting the Perspective is an easy hack. In fact, the camera should be calibrated.
	// With such a calibration we would get the projection matrix. This matrix could then be loaded 
	// to GL_PROJECTION.
	// If you are using another camera (which you'll do in most cases), you'll have to adjust the FOV
	// value. How? Fiddle around: Move Marker to edge of display and check if you have to increase or 
	// decrease.
	gluPerspective( virtual_camera_angle, ((GLfloat)width/(GLfloat)height), 0.01, 100 );

	// invalidate display
	//glutPostRedisplay();
}


int main(int argc, char* argv[]) 
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	// initialize the window system
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(camera_width, camera_height, "Exercise 8 - Combine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	// Set callback functions for GLFW
	glfwSetFramebufferSizeCallback(window, reshape);

	glfwMakeContextCurrent(window);
	glfwSwapInterval( 1 );
	
	int window_width, window_height;
	glfwGetFramebufferSize(window, &window_width, &window_height);
	reshape(window, window_width, window_height);

	glViewport(0, 0, window_width, window_height);

	// initialize the GL library
	initGL(argc, argv);

    // setup OpenCV
	cv::Mat img_bgr;
	initVideoStream(cap);
	const double kMarkerSize = 0.048; // [m]
	MarkerTracker markerTracker(kMarkerSize);
	
	float resultMatrix[16];
	/* Loop until the user closes the window */
	vector<Marker> markers;
	while (!glfwWindowShouldClose(window))
	{
		markers.resize(0);

		/* Capture here */
		cap >> img_bgr;
		
		if(img_bgr.empty()){
			std::cout << "Could not query frame. Trying to reinitialize." << std::endl;
			initVideoStream(cap);
			cv::waitKey(1000); /// Wait for one sec.
			continue;
		}

		/* Track a marker */
		markerTracker.findMarker( img_bgr, markers);
		
		
		//myHeight=markerTracker.markerHeight;
		//myWidth=markerTracker.markerWidth;
		/* Render here */
			display(window,      img_bgr, markers);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

  
    return 0;
}


