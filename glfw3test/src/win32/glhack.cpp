#include <GLFW/glfw3.h>
#ifdef _WIN32
#include "glhack.h"

//#include <GL/gl.h>     //@@ took this out otherwise compile errors
//#include <GL/glext.h>  //@@ took this out otherwise compile errors
void *wglGetProcAddress( const char *funcName ) { return glfwGetProcAddress( funcName ); }
//#include "wingdi.h"


GLint glGetUniformLocation( GLuint program, const GLchar * name ) {
	typedef GLint (__stdcall*glGetUniformLocationFunction)( GLuint program, const GLchar * name );
	static glGetUniformLocationFunction f;
	if(!f) f= (glGetUniformLocationFunction)wglGetProcAddress("glGetUniformLocation");
	return f( program, name );
}
void glUseProgram( GLuint program ) {
	typedef void (__stdcall*glUseProgramFunc)( GLuint program );
	static glUseProgramFunc f;
	if(!f) f= (glUseProgramFunc)wglGetProcAddress("glUseProgram");
	return f( program );
}
void glShaderSource( GLuint shader, GLsizei count, const GLchar ** string, const GLint * length ) {
	typedef void (__stdcall*glShaderSourceFunc)( GLuint shader, GLsizei count, const GLchar ** string, const GLint * length );
	static glShaderSourceFunc f;
	if(!f) f=(glShaderSourceFunc)wglGetProcAddress("glShaderSource");
	return f( shader, count, string, length );
}
void glCompileShader( GLuint shader ) {
	typedef void (__stdcall*glCompileShaderFunc)( GLuint shader );
	static glCompileShaderFunc f;
	if(!f) f=(glCompileShaderFunc)wglGetProcAddress("glCompileShader");
	return f( shader );
}
void glGetShaderiv( GLuint shader, GLenum pname, GLint *params) {
	typedef void (__stdcall*glGetShaderivFunc)( GLuint shader, GLenum pname, GLint *params);
	static glGetShaderivFunc f;
	if(!f) f = (glGetShaderivFunc)wglGetProcAddress("glGetShaderiv");
	return f( shader, pname, params);
}
void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
	typedef void (__stdcall*glGetShaderInfoLogFunc) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	static glGetShaderInfoLogFunc f;
	if(!f) f = (glGetShaderInfoLogFunc)wglGetProcAddress("glGetShaderInfoLog");
	return f(shader, bufSize, length, infoLog);
}
void glAttachShader (GLuint program, GLuint shader) {
	typedef void (__stdcall*glAttachShaderFunc) (GLuint program, GLuint shader);
	static glAttachShaderFunc f;
	if(!f) f= (glAttachShaderFunc)wglGetProcAddress("glAttachShader");
	return f(program, shader);
}
void glLinkProgram (GLuint program) {
	typedef void (__stdcall*glLinkProgramFunc) (GLuint program);
	static glLinkProgramFunc f;
	if(!f) f= (glLinkProgramFunc)wglGetProcAddress("glLinkProgram");
	return f(program);
}
void glGetProgramiv (GLuint program, GLenum pname, GLint *params) {
	typedef void (__stdcall*glGetProgramivFunc) (GLuint program, GLenum pname, GLint *params);
	static glGetProgramivFunc f;
	if(!f) f= (glGetProgramivFunc)wglGetProcAddress("glGetProgramiv");
	return f(program, pname, params);
}
void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
	typedef void (__stdcall*glGetProgramInfoLogFunc) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	static glGetProgramInfoLogFunc f;
	if(!f) f= (glGetProgramInfoLogFunc)wglGetProcAddress("glGetProgramInfoLog");
	return f(program, bufSize, length, infoLog);
}
GLuint glCreateProgram (void) {
	typedef GLuint (__stdcall*glCreateProgramFunc) (void);
	static glCreateProgramFunc f;
	if(!f) f= (glCreateProgramFunc)wglGetProcAddress("glCreateProgram");
	return f();
}
GLuint glCreateShader (GLenum type) {
	typedef GLuint (__stdcall*glCreateShaderFunc) (GLenum type);
	static glCreateShaderFunc f;
	if(!f) f = (glCreateShaderFunc)wglGetProcAddress("glCreateShader");
	return f(type);
}
void glValidateProgram (GLuint program) {
	typedef void (__stdcall*glValidateProgramFunc) (GLuint program);
	static glValidateProgramFunc f;
	if(!f) f = (glValidateProgramFunc)wglGetProcAddress("glValidateProgram");
	return f(program);
}
GLint glGetAttribLocation (GLuint program, const GLchar *name) {
	typedef GLint (__stdcall*glGetAttribLocationFunc) (GLuint program, const GLchar *name);
	static glGetAttribLocationFunc f;
	if(!f) f= (glGetAttribLocationFunc)wglGetProcAddress("glGetAttribLocation");
	return f(program, name);
}
void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name) {
	typedef void (__stdcall*glBindAttribLocationFunc) (GLuint program, GLuint index, const GLchar *name);
	static glBindAttribLocationFunc f;
	if(!f) f= (glBindAttribLocationFunc)wglGetProcAddress("glBindAttribLocation");
	return f(program, index, name);
}
void glActiveTexture( GLenum texture ) {
	typedef void (__stdcall*glActiveTextureFunc)( GLenum texture );
	static glActiveTextureFunc f;
	if(!f) f = (glActiveTextureFunc)wglGetProcAddress("glActiveTexture");
	return f( texture );
}
void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	typedef void (__stdcall*glUniformMatrix4fvFunc) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	static glUniformMatrix4fvFunc f;
	if(!f) f= (glUniformMatrix4fvFunc)wglGetProcAddress("glUniformMatrix4fv");
	return f(location, count, transpose, value);
}
void glDisableVertexAttribArray (GLuint index) {
	typedef void (__stdcall*glDisableVertexAttribArrayFunc) (GLuint index);
	static glDisableVertexAttribArrayFunc f;
	if(!f) f= (glDisableVertexAttribArrayFunc)wglGetProcAddress("glDisableVertexAttribArray");
	return f(index);
}
void glEnableVertexAttribArray (GLuint index) {
	typedef void (__stdcall*glEnableVertexAttribArrayFunc) (GLuint index);
	static glEnableVertexAttribArrayFunc f;
	if(!f) f= (glEnableVertexAttribArrayFunc)wglGetProcAddress("glEnableVertexAttribArray");
	return f(index);
}
void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
	typedef void (__stdcall*glVertexAttribPointerFunc) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	static glVertexAttribPointerFunc f;
	if(!f) f= (glVertexAttribPointerFunc)wglGetProcAddress("glVertexAttribPointer");
	return f(index, size, type, normalized, stride, pointer);
}
void glBindBuffer (GLenum target, GLuint buffer) {
	typedef void (__stdcall*glBindBufferFunc) (GLenum target, GLuint buffer);
	static glBindBufferFunc f;
	if(!f) f= (glBindBufferFunc)wglGetProcAddress("glBindBuffer");
	return f(target, buffer);
}
void glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
	typedef void (__stdcall*glBufferDataFunc) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	static glBufferDataFunc f;
	if(!f) f= (glBufferDataFunc)wglGetProcAddress("glBufferData");
	return f(target, size, data, usage);
}
void glGenBuffers (GLsizei n, GLuint *buffers) {
	typedef void (__stdcall*glGenBuffersFunc) (GLsizei n, GLuint *buffers);
	static glGenBuffersFunc f;
	if(!f) f= (glGenBuffersFunc)wglGetProcAddress("glGenBuffers");
	return f(n, buffers);
}
void glUniform4fv( GLuint program, GLint count, const GLfloat *v ) {
	typedef void (__stdcall*glUniform4fvFunc)( GLuint program, GLint count, const GLfloat *v );
	static glUniform4fvFunc f;
	if(!f) f= (glUniform4fvFunc)wglGetProcAddress("glUniform4fv");
	return f( program, count, v );
}

void glUniform3fv( GLuint program, GLint count, const GLfloat *v ) {
	typedef void (__stdcall*glUniform3fvFunc)( GLuint program, GLint count, const GLfloat *v );
	static glUniform3fvFunc f;
	if(!f) f= (glUniform3fvFunc)wglGetProcAddress("glUniform3fv");
	return f( program, count, v );
}
void glUniform1f( GLint location, GLfloat v0 ) {
	typedef void (__stdcall*glUniform1fFunc)( GLint location, GLfloat v0 );
	static glUniform1fFunc f;
	if(!f) f= (glUniform1fFunc)wglGetProcAddress("glUniform1f");
	return f( location, v0 );
}
void glUniform2f( GLint location, GLfloat v0, GLfloat v1 ) {
	typedef void (__stdcall*glUniform2fFunc)( GLint location, GLfloat v0, GLfloat v1 );
	static glUniform2fFunc f;
	if(!f) f= (glUniform2fFunc)wglGetProcAddress("glUniform2f");
	return f( location, v0, v1 );
}
void glUniform3f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2 ) {
	typedef void (__stdcall*glUniform3fFunc)( GLint location, GLfloat v0, GLfloat v1, GLfloat v2 );
	static glUniform3fFunc f;
	if(!f) f= (glUniform3fFunc)wglGetProcAddress("glUniform3f");
	return f( location, v0, v1, v2 );
}
void glUniform4f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 ) {
	typedef void (__stdcall*glUniform4fFunc)( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 );
	static glUniform4fFunc f;
	if(!f) f = (glUniform4fFunc)wglGetProcAddress("glUniform4f");
	return f( location, v0, v1, v2, v3 );
}
void glUniform1i( GLint location, GLint v0 ) {
	typedef void (__stdcall*glUniform1iFunc)( GLint location, GLint v0 );;
	static glUniform1iFunc f;
	if(!f) f= (glUniform1iFunc)wglGetProcAddress("glUniform1i");
	return f( location, v0 );
}
void glUniform2i( GLint location, GLint v0, GLint v1 ) {
	typedef void (__stdcall*glUniform2iFunc)( GLint location, GLint v0, GLint v1 );
	static glUniform2iFunc f;
	if(!f) f= (glUniform2iFunc)wglGetProcAddress("glUniform2i");
	return f( location, v0, v1 );
}
void glUniform3i( GLint location, GLint v0, GLint v1, GLint v2 ) {
	typedef void (__stdcall*glUniform3iFunc)( GLint location, GLint v0, GLint v1, GLint v2 );
	static glUniform3iFunc f;
	if(!f) f= (glUniform3iFunc)wglGetProcAddress("glUniform3i");
	return f( location, v0, v1, v2 );
}
void glUniform4i( GLint location, GLint v0, GLint v1, GLint v2, GLint v3 ) {
	typedef void (__stdcall*glUniform4iFunc)( GLint location, GLint v0, GLint v1, GLint v2, GLint v3 );
	static glUniform4iFunc f;
	if(!f) f= (glUniform4iFunc)wglGetProcAddress("glUniform4i");
	return f( location, v0, v1, v2, v3 );
}
void glUniform1ui( GLint location, GLuint v0 ) {
	typedef void (__stdcall*glUniform1uiFunc)( GLint location, GLuint v0 );;
	static glUniform1uiFunc f;
	if(!f) f= (glUniform1uiFunc)wglGetProcAddress("glUniform1ui");
	return f( location, v0 );
}
void glUniform2ui( GLint location, GLuint v0, GLuint v1 ) {
	typedef void (__stdcall*glUniform2uiFunc)( GLint location, GLuint v0, GLuint v1 );
	static glUniform2uiFunc f;
	if(!f) f= (glUniform2uiFunc)wglGetProcAddress("glUniform2ui");
	return f( location, v0, v1 );
}
void glUniform3ui( GLint location, GLuint v0, GLuint v1, GLuint v2 ) {
	typedef void (__stdcall*glUniform3uiFunc)( GLint location, GLuint v0, GLuint v1, GLuint v2 );
	static glUniform3uiFunc f;
	if(!f) f= (glUniform3uiFunc)wglGetProcAddress("glUniform3ui");
	return f( location, v0, v1, v2 );
}
void glUniform4ui( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 ) {
	typedef void (__stdcall*glUniform4uiFunc)( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 );
	static glUniform4uiFunc f;
	if(!f) f= (glUniform4uiFunc)wglGetProcAddress("glUniform4ui");
	return f( location, v0, v1, v2, v3 );
}

// frame buffer stuff
void glGenFramebuffers (GLsizei n, GLuint *framebuffers) {
	typedef void (__stdcall*glGenFramebuffers) (GLsizei n, GLuint *framebuffers);
	static glGenFramebuffers f;
	if(!f) f = (glGenFramebuffers)wglGetProcAddress("glGenFramebuffers");
	return f(n,framebuffers);
}
void glBindFramebuffer (GLenum target, GLuint framebuffer) {
	typedef void (__stdcall*glBindFramebuffer) (GLenum target, GLuint framebuffer);
	static glBindFramebuffer f;
	if(!f) f = (glBindFramebuffer)wglGetProcAddress("glBindFramebuffer");
	return f(target,framebuffer);
}
void glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
	typedef void (__stdcall*glFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	static glFramebufferTexture2D f;
	if(!f) f = (glFramebufferTexture2D)wglGetProcAddress("glFramebufferTexture2D");
	return f(target,attachment,textarget,texture,level);
}
void glDrawBuffers (GLsizei n, const GLenum *bufs) {
	typedef void (__stdcall*glDrawBuffers) (GLsizei n, const GLenum *bufs);
	static glDrawBuffers f;
	if(!f) f = (glDrawBuffers)wglGetProcAddress("glDrawBuffers");
	return f(n,bufs);
}
GLenum glCheckFramebufferStatus (GLenum target) {
	typedef GLenum (__stdcall*glCheckFramebufferStatus) (GLenum target);
	static glCheckFramebufferStatus f;
	if(!f) f = (glCheckFramebufferStatus)wglGetProcAddress("glCheckFramebufferStatus");
	return f(target);
}
#endif //_WIN32