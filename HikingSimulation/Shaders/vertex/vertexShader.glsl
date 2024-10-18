#version 330 core

layout(location = 0) in vec3 in_position;

out float Height;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    Height = in_position.y;
    Position = (view * model * vec4(in_position, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(in_position, 1.0f);
}
