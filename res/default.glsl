#vertex
#version 330 core

layout(location = 0) in vec3 position;
uniform mat4 transform;

void main() {

	gl_Position = transform * vec4(position, 1.0f);
}

#fragment
#version 330 core

out vec3 color;
uniform vec3 mesh_color;

void main() {

	color = mesh_color;
}

