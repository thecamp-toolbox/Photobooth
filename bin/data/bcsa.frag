precision highp float; // this will make the default precision high

//we passed this in from our vert shader
varying vec2        texcoord0;

//These are variables we set in our ofApp using the ofShader API

//our texture reference
//passed in by
//shader.setUniformTexture("tex0", sourceImage.getTextureReference(), sourceImage.getTextureReference().texData.textureID);
uniform sampler2D   image;

//width and height that we are working with
//passed in by
//shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
uniform vec2        resolution;

const vec3 LumCoeff = vec3 (0.2125, 0.7154, 0.0721);

uniform vec3 avgluma;  
uniform float saturation;  
uniform float contrast;  
uniform float brightness;  
uniform float alpha;  

void main (void)  
{  
	vec3 texColor  	= texture2d(image, texcoord0).rgb;
	vec3 intensity 	= vec3 (dot(texColor, LumCoeff));
	vec3 color     	= mix(intensity, texColor, saturation);
	color          	= mix(avgluma, color, contrast);  
	color			*= brightness;  
	gl_FragColor   	= vec4 (color, alpha);  
}  
