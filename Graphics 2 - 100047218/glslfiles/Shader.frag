#version 150

in  vec2 ex_TexCoord; //texture coord arriving from the vertex
in  vec3 ex_Normal;  //normal arriving from the vertex

out vec4 out_Color;   //colour for the pixel
in vec3 ex_LightDir;  //light direction arriving from the vertex

in vec3 ex_PositionEye;

uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

uniform sampler2D DiffuseMap;

void main(void)
{
	//out_Color = texture(DiffuseMap, ex_TexCoord); //show texture values

	//out_Color = vec4(ex_Normal,1.0); //Show normals

	//out_Color = vec4(ex_TexCoord,0.0,1.0); //show texture coords

	//Calculate lighting
	vec3 n, L;
	vec4 color;
	float NdotL;
	
	n = normalize(ex_Normal);
	L = normalize(ex_LightDir);

	vec3 v = normalize(-ex_PositionEye);
	vec3 r = normalize(-reflect(L, n));
	
	float RdotV = max(0.0, dot(r, v));

	NdotL = max(dot(n, L),0.0);

	color = light_ambient * material_ambient;
	
	if(NdotL > 0.0) 
	{
		color += (light_ambient * material_diffuse * NdotL);
	}

	color += material_specular * light_specular * pow(RdotV, material_shininess);

	//out_Color = color;  //show just lighting

    out_Color = color * texture(DiffuseMap, ex_TexCoord); //show texture and lighting
}