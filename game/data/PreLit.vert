#version 120
attribute vec4 in_Vertex;
attribute vec4 in_Color;
attribute vec2 in_MultiTexCoord0;

uniform mat4 in_Pmat;
uniform mat4 in_Vmat;
uniform mat4 in_Mmat;

uniform float gTime;

varying vec4 Color;
varying vec3 Pos;
varying vec2 Tex;
varying vec3 diffuse; 

void main() {
	Pos = in_Vertex.xyz;
	Tex = vec2(in_MultiTexCoord0);
	vec4 vert = in_Vertex;
	gl_Position = in_Pmat * in_Vmat * in_Mmat * vert;
	Color = in_Color;

	diffuse = Color.xyz;
}
