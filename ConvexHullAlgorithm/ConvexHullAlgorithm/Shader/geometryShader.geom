#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;


void main() {
	vec3 vector1 = gl_in[2].gl_Position.xyz -gl_in[0].gl_Position.xyz;
	vec3 vector2 = gl_in[1].gl_Position.xyz -gl_in[0].gl_Position.xyz;
	vec3 n = cross(vector1, vector2);

	Normal = n;
	FragPos = gl_in[0].gl_Position.xyz;
	gl_Position = projection * view * gl_in[0].gl_Position;
	EmitVertex();
	Normal = n;
	FragPos = gl_in[1].gl_Position.xyz;
	gl_Position = projection * view * gl_in[1].gl_Position;
	EmitVertex();
	Normal = n;
	FragPos = gl_in[2].gl_Position.xyz;
	gl_Position = projection * view * gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}