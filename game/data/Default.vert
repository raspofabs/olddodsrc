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
<<<<<<< HEAD:game/data/Default.vert
	gl_Position = in_Pmat * in_Vmat * in_Mmat * in_Vertex;
	gl_Position.x += gTime * 0.0001;
=======
	vec4 v = in_Vertex;
	//v.z *= 0.01;
	//v.x += 2.0;
	gl_Position = in_ProjectionMatrix * in_ModelViewMatrix * v;
	//gl_Position.x += gTime * 0.0001;
	gl_Position.x += 0.3;
	gl_Position.z *= 0.001;
>>>>>>> 77635c053be1f399bb43007f00fcaa5c322db896:game/data/vert.glsl
	Color = in_Color;
	Normal = ( in_Mmat * vec4(in_Normal,0.0) ).xyz;
}
