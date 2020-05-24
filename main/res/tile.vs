#version 330 core

// transformation matrix to be applied to entire grid. When it's the identity,
// the board is drawn to exactly fit the unit square
uniform mat3 trans;


layout (location = 0) in vec2 position;

layout (location = 1) in vec2 tex_coords;

out vec2 frag_tex_coords;


void main() {

    vec3 pos = trans * vec3(position, 1.f);
    gl_Position.xyw = pos;
    gl_Position.z = 0.f;

    frag_tex_coords = tex_coords;
}

