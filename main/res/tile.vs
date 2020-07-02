#version 330 core

// transforms the vertices to the proper screen coordinates
uniform mat3 view;

// transformation applied by the camera to shift to the view space
uniform mat3 cam;

// transforms the model to its proper position in the world
uniform mat3 model;


layout (location = 0) in vec2 position;

layout (location = 1) in int in_tex_idx;

layout (location = 2) in vec2 tex_coords;

flat out int tex_idx;

out vec2 frag_tex_coords;


void main() {

    vec3 pos = view * cam * model * vec3(position, 1.f);
    gl_Position.xyw = pos;
    gl_Position.z = 0.f;

    frag_tex_coords = tex_coords;

    tex_idx = in_tex_idx;
}

