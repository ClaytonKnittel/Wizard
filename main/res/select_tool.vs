#version 330 core

// transforms the vertices to the proper screen coordinates
uniform mat3 view;

// transformation applied by the camera to shift to the view space
uniform mat3 cam;

// transforms the model to its proper position in the world
uniform mat3 model;


layout (location = 0) in vec2 position;

layout (location = 1) in float darkness;

out float frag_darkness;


void main() {

    vec3 pos = view * cam * model * vec3(position, 1.f);
    gl_Position.xyw = pos;
    gl_Position.z = 0.f;

    frag_darkness = darkness;
}

