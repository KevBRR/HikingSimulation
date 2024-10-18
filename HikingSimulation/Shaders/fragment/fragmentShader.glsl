#version 330 core

out vec4 frag_color;

in float Height;

void main() {
    float h = (Height + 16)/32.0f; // Shift and scale to grayscale value
    frag_color = vec4(1.0, 0.0, 1.0, 1.0);
}