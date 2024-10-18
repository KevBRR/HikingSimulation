#version 330 core

out vec4 FragColor;

in float Height;

void main() {
    float h = (Height)/250.0f; // 32.0f Shift and scale to grayscale value
    FragColor = vec4(h, h, h, 1.0);
}