#version 330 core

// transforms the model to its proper position on the screen
uniform mat3 model;

// transformation matrix to be applied to each individual tile to move it into
// its proper relative location
//uniform mat3 constructor;


layout (location = 0) in vec3 position;

layout (location = 1) in int in_tex_idx;

layout (location = 2) in vec2 tex_coords;

flat out int tex_idx;

out vec2 frag_tex_coords;


void main() {

    vec3 pos = model * /*constructor */ vec3(position.xy, 1.f);
    gl_Position.xyw = pos;
    gl_Position.z = position.z;

    frag_tex_coords = tex_coords;

    tex_idx = in_tex_idx;
}

