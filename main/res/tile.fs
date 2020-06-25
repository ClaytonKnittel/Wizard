#version 330 core


uniform sampler2D texs[16];

flat in int tex_idx;
in vec2 frag_tex_coords;

out vec4 color;


void main() {
    color = texture(texs[tex_idx], frag_tex_coords);
    if (color.a == 0.f) {
        discard;
    }
}

