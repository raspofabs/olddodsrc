#ifndef _SHADER_H_
#define _SHADER_H_

#define ATTR_VERTEX 0
#define ATTR_NORMAL 1
#define ATTR_UV 2
#define ATTR_COLOR 3

class GLShader
{
public:
	GLuint m_program;
	GLuint projLocation;
	GLuint viewLocation;
	GLuint modelLocation;
	GLuint worldSpaceLightPos;
	//GLuint objectSpaceLightDirDiffuse;
	GLuint lightColour;
	GLuint ambientColour;
	GLuint textureLocation;
	GLuint timeLocation;
	time_t vertTime, fragTime;
	char vshad[128], fshad[128];
	
	static GLShader* m_pCurrent;
	void Use(bool force = false)
	{
		if (!force && m_pCurrent == this) return;
		glUseProgram(m_program);
		m_pCurrent = this;
	}
	static GLShader* Current() { return m_pCurrent; }
};

# define ShaderUniformExists(_s, _n) _s._n != 0xFFFFFFFF
# define UniformExists(_n) GLShader::Current()->_n != 0xFFFFFFFF
# define SetUniform1i(_n, _v) glUniform1i(GLShader::Current()->_n, _v)
# define SetUniform1f(_n, _v) glUniform1f(GLShader::Current()->_n, _v)
# define SetUniform2f(_n, _v,_v2) glUniform2f(GLShader::Current()->_n, _v,_v2)
# define SetUniform3f(_n, _v,_v2,_v3) glUniform3f(GLShader::Current()->_n, _v,_v2,_v3)
# define SetUniform4f(_n, _v,_v2,_v3,_v4) glUniform4f(GLShader::Current()->_n, _v,_v2,_v3,_v4)
# define SetUniform2fv(_n, _c, _v) glUniform2fv(GLShader::Current()->_n, _c, _v)
# define SetUniform3fv(_n, _c, _v) glUniform3fv(GLShader::Current()->_n, _c, _v)
# define SetUniform4fv(_n, _c, _v) glUniform4fv(GLShader::Current()->_n, _c, _v)
# define SetUniformMat(_n, _v) glUniformMatrix4fv(GLShader::Current()->_n, 1, false, _v)

extern GLShader DefaultShaderProgram;
void InitShaders();
void UpdateShaders();
void AddShader( const char *name, const char *vshad, const char *fshad );
void ActivateShader( const char *name );

#endif
