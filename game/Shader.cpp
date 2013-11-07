#include "graphics.h"
#include "util.h"
#include "Shader.h"

#include <string.h>

GLShader DefaultShaderProgram;
GLShader Shader_SingleLight;
GLShader Shader_Prelit;

bool ReadAndCompileShader(GLuint &prog, const char *filename, GLuint &shader, GLuint shaderType ) {
	char pShaderText[8192];
	memset( pShaderText, 0, sizeof( pShaderText ) );
	//printf("Load shader %s\n", filename);
	FILE *fp = fopen(filename, "r" );
	if( fp ) {
		size_t bytesRead = fread(pShaderText,1,sizeof(pShaderText),fp);
		fclose(fp);
		if( sizeof(pShaderText) == bytesRead ) {
			printf( "Unable to load %s (>%i bytes long)\n", filename, (int)sizeof(pShaderText) );
			return false;
		}
	} else {
		printf( "Can't find shader file %s\n", filename );
		return false;
	}
	const GLchar* p[1] = { pShaderText };
	GLint Lengths[1] = { (int)strlen(pShaderText) };
	glShaderSource(shader, 1, p, Lengths);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader %s, type %d: '%s'\n", filename, shaderType, InfoLog);
		return false;
	}
	glAttachShader(prog, shader);
	return true;
}

bool LinkShaders(GLuint &prog) {
	glLinkProgram(prog);

	GLint Success;
	glGetProgramiv(prog, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(prog, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}
	return true;
}

void BindAttributes(GLuint program);
void FindUniforms(GLShader &shader);
void LoadShader(GLShader &shader, bool bailOnError = true ) {
	shader.fragTime = get_mtime( shader.fshad, shader.fragTime );
	shader.vertTime = get_mtime( shader.vshad, shader.vertTime );

	GLuint program = glCreateProgram();
	BindAttributes(program);
	GLuint VertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if( !ReadAndCompileShader(program, shader.vshad, VertShader, GL_VERTEX_SHADER ) ) {
		if( bailOnError ) { exit(1); } else { return; }
	}
	if( !ReadAndCompileShader(program, shader.fshad, FragmentShader, GL_FRAGMENT_SHADER ) ) {
		if( bailOnError ) { exit(1); } else { return; }
	}
	if( !LinkShaders(program) ) {
		if( bailOnError ) { exit(1); } else { return; }
	}
	shader.m_program = program;
	FindUniforms(shader);
	glValidateProgram(shader.m_program);
	glUseProgram(shader.m_program);
	printf("Attribs: v(%d) n(%d) c(%d) u(%d)\n", glGetAttribLocation(shader.m_program, "in_Vertex"),glGetAttribLocation(shader.m_program, "in_Normal"),glGetAttribLocation(shader.m_program, "in_Color"),glGetAttribLocation(shader.m_program, "in_MultiTexCoord0"));
}
void ReloadShader(GLShader &shader) {
	bool reload = false;
	if( shader.fragTime < get_mtime( shader.fshad, shader.fragTime ) ) {
		reload = true;
	}
	if( shader.vertTime < get_mtime( shader.vshad, shader.vertTime ) ) {
		reload = true;
	}

	if( reload ) {
		printf("Reload %s/%s\n", shader.vshad, shader.fshad );
		LoadShader(shader, false);
	}
}


GLuint GetUniformLocation(GLShader& shader, const char* pName)
{
	GLuint n = glGetUniformLocation(shader.m_program, pName);
	if( n == (GLuint)-1 ) {
		printf("GetUniLoc %d(%s,%s) %s - %d\n", shader.m_program, shader.vshad, shader.fshad, pName, n);
		//Exit(1);
	}
	return n;
}
void FindUniforms(GLShader &shader) {
	shader.timeLocation = GetUniformLocation(shader, "gTime");
	shader.textureLocation = GetUniformLocation(shader, "gSampler");

	shader.projLocation = GetUniformLocation(shader, "in_Pmat");
	shader.viewLocation = GetUniformLocation(shader, "in_Vmat");
	shader.modelLocation = GetUniformLocation(shader, "in_Mmat");
	//shader.worldSpaceLightPos = GetUniformLocation(shader, "worldSpaceLightPos");
	//shader.objectSpaceLightDirDiffuse = GetUniformLocation(shader, "ObjectSpaceLightDir");
	//shader.lightColour = GetUniformLocation(shader, "LightColour");
	//shader.ambientColour = GetUniformLocation(shader, "AmbientColour");
}
void BindAttributes(GLuint program) {
	glBindAttribLocation(program, ATTR_VERTEX, "in_Vertex");
	glBindAttribLocation(program, ATTR_NORMAL, "in_Normal");
	glBindAttribLocation(program, ATTR_UV, "in_MultiTexCoord0");
	glBindAttribLocation(program, ATTR_COLOR, "in_Color");
	//printf( "BIND - VNUC = %i %i %i %i\n", ATTR_VERTEX, ATTR_NORMAL, ATTR_UV, ATTR_COLOR );
}

#include <map>
#include <string>
typedef std::map<std::string,GLShader*> ShaderMap;
ShaderMap customShaders;

void AddShader( const char *name, const char *vshad, const char *fshad ) {
	GLShader *shader = new GLShader;
	strcpy( shader->vshad, vshad );
	strcpy( shader->fshad, fshad );
	LoadShader(*shader);
	customShaders[name] = shader;
}
void ActivateShader( const char *name ) {
	ShaderMap::iterator shaderEntry = customShaders.find( name );
	if( shaderEntry != customShaders.end() ) {
		shaderEntry->second->Use();
	} else {
		eprintf("Cannot activate shader %s\n", name );
	}
}

void InitShaders() {
	strcpy( DefaultShaderProgram.vshad, "data/Default.vert" );
	strcpy( DefaultShaderProgram.fshad, "data/Default.frag" );
	LoadShader(DefaultShaderProgram);

	strcpy( Shader_Prelit.vshad, "data/PreLit.vert" );
	strcpy( Shader_Prelit.fshad, "data/PreLit.frag" );
	LoadShader(Shader_Prelit, false);
	AddShader( "prelit", "data/PreLit.vert", "data/PreLit.frag" );
}

void UpdateShaders() {
	ReloadShader(DefaultShaderProgram);
	for( ShaderMap::iterator i = customShaders.begin(); i != customShaders.end(); ++i ) {
		ReloadShader(*(i->second));
	}
}

