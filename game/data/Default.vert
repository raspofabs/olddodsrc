#version 120
attribute vec4 in_Vertex;
attribute vec3 in_Normal;
attribute vec4 in_Color;
attribute vec2 in_MultiTexCoord0;

uniform mat4 in_Pmat;
uniform mat4 in_Vmat;
uniform mat4 in_Mmat;

uniform float gTime;

varying vec4 Color;
varying vec3 Normal;
varying vec3 Pos;
varying vec2 Tex;

void main() {
	Pos = in_Vertex.xyz;
	Tex = vec2(in_MultiTexCoord0);
	gl_Position = in_Pmat * in_Vmat * in_Mmat * in_Vertex;
	Color = in_Color;
	Normal = ( in_Mmat * vec4(in_Normal,0.0) ).xyz;
}
