#version 410
in vec3 color;
in vec2 texUV;
in vec3 world_pos;
in vec3 world_normal;

uniform vec3 light_pos;
uniform vec3 eye_pos;

out vec4 frag_color;

uniform sampler2D myTextureSampler;

const float lightIntensity = 10.0;

//Lecture 10
const int bands = 5;
const float scaleFactor = 1.0/bands;


void main() {
	
	vec3 L = normalize(light_pos - world_pos);
	vec3 V = normalize(eye_pos - world_pos);

	float dist = length(light_pos - world_pos);

	float diffuse = max(0, dot(L, world_normal));
	vec3 diffuseOut = (diffuse * color) / (dist*dist);
	
	diffuseOut = diffuseOut*lightIntensity;

		//Lecture 10
	diffuseOut = floor(diffuseOut * bands) * scaleFactor;
	//                    (0.9     *   5) = 4.5 = floor = 4 * 1/5
	
	// Outline effect
	float edge = (dot(V, world_normal) < 0.1) ? 0.0 : 1.0;

	vec3 result = diffuseOut * edge;

	//frag_color = texture(myTextureSampler, texUV)* vec4(result, 1.0);

	frag_color = vec4(result, 1.0);
}
// Above is toon shading from lecture 10 - below is normal frag_shader - not really used
//#version 410

//layout(location = 1) in vec3 inColor;

//out vec4 frag_color;

//void main() { 
	
	//frag_color = vec4(inColor, 1.0);
//}
