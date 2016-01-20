// Fragment Shader 
#version 150

in  vec4 ex_Color;  //colour arriving from the vertex
out vec4 out_Color; //colour for the pixel

in vec2 ex_TexCoord;  //texture coordinate coming from the vertex shader

uniform sampler2D DiffuseMap;  //sampler 2d for the texture map

void main(void)
{
	//Set the fragment's colour to be the colour in the texture map
	out_Color = ex_Color*texture(DiffuseMap, ex_TexCoord);
}