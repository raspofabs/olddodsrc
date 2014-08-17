#ifndef _glhack_h_
#define _glhack_h_
#ifdef _WIN32
#include <stddef.h>
//#include <GL/gl.h> //@@ took this out otherwise compile errors
#define GLchar char
typedef unsigned int	GLenum;
typedef unsigned char	GLboolean;
typedef unsigned int	GLbitfield;
typedef void		GLvoid;
typedef signed char	GLbyte;		/* 1-byte signed */
typedef short		GLshort;	/* 2-byte signed */
typedef int		GLint;		/* 4-byte signed */
typedef unsigned char	GLubyte;	/* 1-byte unsigned */
typedef unsigned short	GLushort;	/* 2-byte unsigned */
typedef unsigned int	GLuint;		/* 4-byte unsigned */
typedef int		GLsizei;	/* 4-byte signed */
typedef float		GLfloat;	/* single precision float */
typedef float		GLclampf;	/* single precision float in [0,1] */
typedef double		GLdouble;	/* double precision float */
typedef double		GLclampd;	/* double precision float in [0,1] */
typedef ptrdiff_t GLsizeiptr;
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_BGRA                           0x80E1
#define GL_TEXTURE0				0x84C0
#define GL_CLAMP_TO_EDGE			0x812F
#define GL_GENERATE_MIPMAP                0x8191
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4

void glUseProgram( GLuint program );
void glShaderSource( GLuint shader, GLsizei count, const GLchar ** string, const GLint * length );
void glCompileShader( GLuint shader );
void glGetShaderiv( GLuint shader, GLenum pname, GLint *params);
void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glAttachShader (GLuint program, GLuint shader);
void glLinkProgram (GLuint program);
void glGetProgramiv (GLuint program, GLenum pname, GLint *params);
void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLuint glCreateProgram (void);
GLuint glCreateShader (GLenum type);
void glValidateProgram (GLuint program);
GLint glGetAttribLocation (GLuint program, const GLchar *name);
void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name);
void glActiveTexture( GLenum texture );
void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glDisableVertexAttribArray (GLuint index);
void glEnableVertexAttribArray (GLuint index);
void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
void glBindBuffer (GLenum target, GLuint buffer);
void glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void glGenBuffers (GLsizei n, GLuint *buffers);

GLint glGetUniformLocation( GLuint program, const GLchar * name );
void glUniform3fv( GLuint program, GLint count, const GLfloat *v );
void glUniform4fv( GLuint program, GLint count, const GLfloat *v );

void glUniform1f( GLint location, GLfloat v0 );
void glUniform2f( GLint location, GLfloat v0, GLfloat v1 );
void glUniform3f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2 );
void glUniform4f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 );

void glUniform1i( GLint location, GLint v0 ); 
void glUniform2i( GLint location, GLint v0, GLint v1 );
void glUniform3i( GLint location, GLint v0, GLint v1, GLint v2 );
void glUniform4i( GLint location, GLint v0, GLint v1, GLint v2, GLint v3 );

void glUniform1ui( GLint location, GLuint v0 ); 
void glUniform2ui( GLint location, GLuint v0, GLuint v1 );
void glUniform3ui( GLint location, GLuint v0, GLuint v1, GLuint v2 );
void glUniform4ui( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 );

// frame buffer stuff
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RGB32F                         0x8815
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
void glGenFramebuffers (GLsizei n, GLuint *framebuffers);
void glBindFramebuffer (GLenum target, GLuint framebuffer);
void glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glDrawBuffers (GLsizei n, const GLenum *bufs);
GLenum glCheckFramebufferStatus (GLenum target);


#endif // _WIN32
#endif // _glhack_h_
