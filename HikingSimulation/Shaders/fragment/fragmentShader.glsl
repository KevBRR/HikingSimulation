#version 330 core

out vec4 FragColor;

in float Height;

uniform sampler2D lookup;

void main() {
    float h = (Height)/255.0f; // 32.0f Shift and scale to grayscale value
    //FragColor = vec4(h, h, h, 1.0);
    FragColor = texture2D(lookup, vec2(Height/275.0f, 2.0));
}