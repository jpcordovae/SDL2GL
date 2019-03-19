#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 color;
layout (location = 4) in vec2 UV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

//out vec3 outNormal;
//out vec3 outTangent;
//out vec3 outColor;
out vec2 outUV;
//out vec3 position;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	//position=vec3(modelViewMatrix*vec4(vertex,1.0));
	//outNormal=normalMatrix*normal;
	//outTangent=normalMatrix*outTangent;
	//outColor=color;
	outUV=UV;
}