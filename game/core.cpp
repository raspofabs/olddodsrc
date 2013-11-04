#include <assert.h>
#include <unistd.h>
#include "graphics.h"
#include <stdio.h>
#include "geom.h"
#include <limits>
#include "assetloader.h"
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

bool gEscapeIsQuit = true;
float g_fGameTime = 0.0f;
int g_nUpdateTimeHead = 0;
bool fingerReleased = false;
double raiseReleaseTime;


typedef std::string Key;
struct TextureAsset {
	Image *im;
	GLuint glTextureID;
};

typedef std::map<Key, TextureAsset*> TextureDic;
typedef std::map<int, TextureAsset*> MaterialToTexture;
TextureDic gTextures;
std::map<std::string,int> reverselookup;
std::map<std::string, TextureAsset> icons;

bool TextureExists(const char* name)
{
	return icons.count(name);
}

Image get_sub_icon( Image asset, int x, int y, int w = 16, int h = 16 ) {
	Image icon;
	icon.w = w;
	icon.h = h;
	icon.p = new C32[ w * h ];
	//Log( 3, "making asset at %i,%i\n",x,y);
	for( int j = 0; j < h; ++j ) {
		C32 *outrow = icon.p + j*h;
		for( int i = 0; i < w; ++i ) {
			C32 colour = asset.p[(x*w+i)+(y*h+j)*asset.w];
			outrow[i] = colour;
		}
		//Log( 3, " %08x %08x %08x %08x\n", outrow[0], outrow[1], outrow[14], outrow[15] );
	}
	return icon;
}

void AddAsset( const std::string &name, Image *source ) {
	TextureAsset &asset = icons[ name ];
	asset.im = source;
	asset.glTextureID = -1;
	gTextures.insert( TextureDic::value_type(name, &asset ) );
	gTextures[name] = &asset;
	//Log( 3, "AddAsset - %s: %p == %p\n", name.c_str(), &asset, gTextures[name] );
}

void AddSubAsset( const std::string &name, Image source, int x, int y ) {
	TextureAsset &asset = icons[ name ];
	asset.im = new Image;
	Log( 3, "Sub Asset %s\n", name.c_str() );
	*(asset.im) = get_sub_icon( source, x,y, 16, 16 );
	asset.glTextureID = -1;
	gTextures.insert( TextureDic::value_type(name, &asset ) );
	gTextures[name] = &asset;
}

void AddTileAsset( const std::string &name, Image source, int x, int y ) {
	TextureAsset &asset = icons[ name ];
	asset.im = new Image;
	*(asset.im) = get_sub_icon( source, x,y, 64, 64 );
	asset.glTextureID = -1;
	gTextures.insert( TextureDic::value_type(name, &asset ) );
}

void LoadAssets() {
	Image *core;

	Log( 1, "loading images\n" );
	core = LoadImageG("data/core.png");

	AddSubAsset( "pointer", *core, 0,7 );
	AddSubAsset( "white", *core, 2,0 );

	AddAsset( "cursor", LoadImageG("data/cursor.png") );
	AddAsset( "core", core );
}

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
	LoadAssets();
	InitDrawing();
	FontRenderInit();
	extern void GameInit();
	GameInit();

	for( TextureDic::iterator i = gTextures.begin(); i != gTextures.end(); ++i ) {
		TextureAsset *a = i->second;
		if( a ) {
			MakeGLTexture( a );
		} else {
			Log( 3, "Texture asset is null for %s\n", i->first.c_str() );
		}
	}

	void GLMakeSmallWhite();
	GLMakeSmallWhite();
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

void MainUpdate(float fUpdates)
{
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

	if (!( glfwGetKey( GLFW_KEY_LSHIFT ) == GLFW_PRESS || glfwGetKey( GLFW_KEY_RSHIFT ) == GLFW_PRESS ))
	{
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

