varying vec4 Color;
varying vec3 Pos;
varying vec2 Tex;
uniform sampler2D gSampler;

varying vec3 diffuse;

void main() {
	vec4 textureColor = texture2D(gSampler, Tex);
	gl_FragColor = textureColor * vec4(diffuse,1.0);
	//gl_FragColor = vec4(diffuse,1.0);
	//gl_FragColor = textureColor;

	//gl_FragColor.xy = mod(Tex*8.0,1.0);

	//gl_FragColor = vec4(vec3(pow(1-abs(Normal.z),3)),1.0);
	//gl_FragColor = vec4(diffuse,1.0);
	//gl_FragColor.xyz = vec3(1.0);
}
