#version 440

#include "../fragments/vs_common.glsl"

uniform sampler2D myTextureSampler;

void main() {
	// Pass our UV coords to the fragment shader
	outUV = inUV;
	outColor = inColor;

	outWorldPos = (u_Model * vec4(inPosition, 1.0)).xyz;
		// Normals
	outNormal = mat3(u_NormalMatrix) * inNormal;

		//Lecture 10b
	vec3 vert = inPosition;
	vert.y = texture(myTextureSampler, outUV).r;
	
	// sin animation
	vert.y = sin(vert.x * 5.0 + u_Time) * 0.2;
    outWorldPos = (u_Model * vec4(vert, 1.0)).xyz;

	gl_Position = u_ModelViewProjection * vec4(outWorldPos, 1.0);
	//gl_Position = MVP * vec4(vertex_pos, 1.0);
}

//#version 440

// Include our common vertex shader attributes and uniforms
//#include "../fragments/vs_common.glsl"

//void main() {

	//gl_Position = u_ModelViewProjection * vec4(inPosition, 1.0);

	// Lecture 5
	// Pass vertex pos in world space to frag shader
	//outWorldPos = (u_Model * vec4(inPosition, 1.0)).xyz;

	// Normals
	//outNormal = mat3(u_NormalMatrix) * inNormal;

	// Pass our UV coords to the fragment shader
	//outUV = inUV;

	///////////
	//outColor = inColor;

//}	