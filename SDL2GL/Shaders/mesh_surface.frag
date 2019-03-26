#version 330 core

in vec3 outNormal;
//in vec3 outTangent;
in vec3 outColor;
in vec2 outUV;
//in vec3 outLightPos;
//in vec3 position;

uniform sampler2D texture0;
uniform sampler2D texture_diffuse;
uniform vec3 cameraPosition;
uniform vec3 ambientColor;

const float alpha = 0.3;
//const vec3 diffuseColor=vec3(0.5,0.5,0.5);
//const vec3 specularColor=vec3(0.0,0.0,0.0);

out vec4 color;

void main()
{
		vec3 normal = normalize(outNormal);
		vec3 lightPosition = cameraPosition;
		//vec3 surf2light=normalize(lightPosition-position);
		//vec3 surf2camera=normalize(-position);
		//vec3 reflection=-reflect(surf2camera,normal);
		//float diffuseContribution=max(0.0,dot(normal,surf2light));
		//float specularContribution=pow(max(0.0,dot(reflection,surf2light)),4.0);
		//color = vec4(ambientColor*outColor+diffuseContribution *diffuseColor*outColor+ specularContribution*specularColor,1.0);
		color = vec4(ambientColor,0.5);
}