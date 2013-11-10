#include <assert.h>
#include <unistd.h>
#include "graphics.h"
#include <stdio.h>
#include "geom.h"
#include <limits>
#include "BadMesh.h"
#include "FontRender.h"
#include "Shader.h"
#include <map>
#include <string>
#include "util.h"

#define SIMPLE_PROFILING 0

bool shouldExit = false;
int closeWindowCallback( ) {
	shouldExit = true;
	return GL_TRUE;
}

int screen_width = 0, screen_height = 0;
int win_width = 800;
int win_height = 500;
int centre_x = win_width / 2;
int centre_y = win_height / 2;

float gTime;
bool gEscapeIsQuit = true;
float g_fGameTime = 0.0f;
int g_nUpdateTimeHead = 0;
bool fingerReleased = false;
double raiseReleaseTime;


void NotSetMode( int menumode ) {
	Log( 3, "Setting to menu[%i]\n", menumode );
	if( menumode == 1 ) {
		glfwEnable( GLFW_MOUSE_CURSOR );
	} else {
		glfwDisable( GLFW_MOUSE_CURSOR );
	}
}

//GLuint listIndex = 10000;
//GLuint vbos[2];

int gFrameNumber = 0;

void MakeGLTexture( TextureAsset *a ) {
	if( a && a->im ) {
		if( int error = glGetError() ) {
			Log( 3, "before MakeGLTexture (%i,%x)\n", error, error );
		}
		//Log( 3, "MakeGLTexture:\ntextureID = %i => ", a->glTextureID );
		glGenTextures( 1, &a->glTextureID );
		if( int error = glGetError() ) {
			Log( 3, "glGenTextures (%i,%x)\n", error, error );
			Log( 3, "glGenTextures : (%i)\n", a->glTextureID );
		}
		//Log( 3, "%i (%i)\n", a->glTextureID, glGetError() );
		glBindTexture( GL_TEXTURE_2D, a->glTextureID );
		//glUniform1i(textureLocation, a->glTextureID);
		if( int error = glGetError() ) {
			Log( 3, "glBindTexture (%i,%x)\n", error, error );
		}
		int nGLFmt = GL_RGBA;
		glShadeModel( GL_SMOOTH );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		nGLFmt = 4;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//NEAREST);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexImage2D( GL_TEXTURE_2D, 0, nGLFmt,
				a->im->w, a->im->h,
				0, GL_RGBA,
				GL_UNSIGNED_BYTE, a->im->p);
		if( int error = glGetError() ) {
			Log( 3, "glTexImage2D (%i,%x)\n", error, error );
			Log( 3, "glTexImage2D : (%i) (%i,%i : %x)\n",
					a->glTextureID,
					a->im->w, a->im->h,
					(unsigned int)((char*)a->im->p-(char*)0) );
		}
	} else {
		if( a ) {
			Log( 3, "cannot make much with a null image\n" );
		} else {
			Log( 3, "cannot make much with a null TextureAsset\n" );
		}
	}
}

extern GLuint g_nSmallWhiteTexture;

void update( double ) {
}

Vec2 gControlVec;

void controlCB( int symbol, int state ) {
	if( symbol == GLFW_KEY_SPACE ) {
		if( state == GLFW_PRESS ) {
			gControlVec.y = 1000.0f;
		} else if( state == GLFW_RELEASE ) {
			gControlVec.y = 0.0f;
		}
	}
	if( symbol == GLFW_KEY_LEFT ) {
		if( state == GLFW_PRESS ) {
			gControlVec.x = -3000.0f;
		} else if( state == GLFW_RELEASE ) {
			gControlVec.x = 0.0f;
		}
	}
	if( symbol == GLFW_KEY_RIGHT ) {
		if( state == GLFW_PRESS ) {
			gControlVec.x = 3000.0f;
		} else if( state == GLFW_RELEASE ) {
			gControlVec.x = 0.0f;
		}
	}
	if( symbol == GLFW_KEY_TAB ) {
	}
}

void MainInit()
{
	//RunTests();

	InitShaders();
	FontRenderInit();
	extern void GameInit();
	GameInit();
	PostInitGraphics();

}

int initialmousex;
int initialmousey;
Vec2 mouseScreenPos;

void MousePositionUpdate()
{
	int mousex, mousey;
	glfwGetMousePos( &mousex, &mousey );
	initialmousex = mousex;
	initialmousey = mousey;

	const float xrel = mousex/float(win_width);
	const float yrel = mousey/float(win_height);
	mouseScreenPos = Vec2( xrel, yrel );
}

F32 c_fBaseScale = 1.0f;

void MainUpdate(float fUpdates) {
	static bool bPressing1 = false;
	if ( glfwGetKey( '1' ) == GLFW_PRESS ) {
		if( !bPressing1 ) {
			extern bool gWireframeMode;
			printf( "changing wireframe mode from %i ", gWireframeMode );
			gWireframeMode = !gWireframeMode;
			printf( " to %i\n", gWireframeMode );
			fflush(stdout);
		}
		bPressing1 = true;
	} else {
		bPressing1 = false;
	}

	UpdateShaders();
	MousePositionUpdate();
	extern void GameUpdate();
	GameUpdate();

	g_fGameTime += fUpdates;

	update(fUpdates);
}

const char* c_szBuildDate = __DATE__;
const char* c_szBuildTime = __TIME__;
//static char s_szVersion[] = "0.0";

void MainShutdown()
{
	extern void GameShutdown();
	GameShutdown();

	glfwEnable( GLFW_MOUSE_CURSOR );

	if (!( glfwGetKey( GLFW_KEY_LSHIFT ) == GLFW_PRESS || glfwGetKey( GLFW_KEY_RSHIFT ) == GLFW_PRESS )) {
	}
}

float g_fScreenW, g_fScreenH;
int main( int argc, char *argv[] ) {
	for( int i = 0; i < argc; ++ i ) {
		Log( 3, "Arg %i: [%s]\n", i, argv[i] );
	}

	SetupCrashdump();

	int result = glfwInit();
	if( result != 1 ) {
		Log( 3, "Init failed: glfwInit() => %i\n", result );
		exit(1);
	}

	//InitPhysics( glfwGetTime() );

	// bug on mac where it can't get the desktop mode, so only get the right size for non-macs
#ifndef __MACH__
	GLFWvidmode return_struct;
	glfwGetDesktopMode( &return_struct );
	screen_height = return_struct.Height;
	screen_width = return_struct.Width;

	win_width = screen_width * 3 / 5;
	win_height = screen_height * 3 / 5;
#endif
	Log( 3, "Screen:Win %i,%i : %i,%i\n", screen_width, screen_height, win_width, win_height );

	int bitsRGBA[] = { 8,8,8, 8 };
	int depthbits = 32;
	int stencilbits = 0;
	int mode = GLFW_WINDOW;

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	result = glfwOpenWindow( win_width, win_height, bitsRGBA[0], bitsRGBA[1], bitsRGBA[2], bitsRGBA[3], depthbits, stencilbits, mode );
	if( result != 1 ) {
		Log( 3, "glfwopenwindow() => %i\n", result );
		exit(1);
	}
	glfwSetWindowTitle("game");

	Log( 3, "GLSL Version %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	//glfwSetWindowPos(0,0);
	//glfwSetWindowPos(800,400);

	MainInit();

	glfwGetMousePos( &initialmousex, &initialmousey );
	glfwSetKeyCallback( controlCB );
	//glfwSetMouseButtonCallback( mouseButtonCB );
	//glfwSetMouseWheelCallback( mouseWheelCB );
	glfwSwapInterval(1);

	double bootTime = glfwGetTime();
	double startTime = bootTime;
	double lastFrameTime = startTime;
	int frame = 0;
	glfwSetWindowCloseCallback( closeWindowCallback );

	while( !shouldExit ) {
		//UpdatePhysics( glfwGetTime() );

		if (glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS) {
			if( gEscapeIsQuit )
				shouldExit = true;
		}
		if(glfwGetKey( 'A' ) == GLFW_PRESS ) {
		}
		if(glfwGetKey( 'D' ) == GLFW_PRESS ) {
		}
		if(glfwGetKey( 'W' ) == GLFW_PRESS ) {
		}
		if(glfwGetKey( 'S' ) == GLFW_PRESS ) {
		}

		int width, height;
		glfwGetWindowSize(&width, &height);
		win_width = width; win_height = height;
		glViewport(0,0, win_width, win_height);
		g_fScreenW = win_width; g_fScreenH = win_height;


		game_on_draw(g_fGameTime);

		//double postRender = glfwGetTime();

		double diff = lastFrameTime - startTime;

		MainUpdate(diff);

		//double postUpdate = glfwGetTime();

		glfwSwapBuffers();

		startTime = lastFrameTime;
		lastFrameTime = glfwGetTime();
		int nextFrame = (int)((lastFrameTime-bootTime) * 30.0f);
		int nWaits = 0;
		while( nextFrame == frame ) {
			nWaits ++;
			usleep( 100 );
			lastFrameTime = glfwGetTime();
			nextFrame = (int)((lastFrameTime-bootTime) * 30.0f);
		}
		frame = nextFrame;
	}

	MainShutdown();

	return 0;
}

void SetGameTitle( const char *newName ) {
	glfwSetWindowTitle(newName);
}

