#version 330 core


uniform sampler2D tex;

in vec2 frag_tex_coords;

out vec4 color;


void main() {
    color = texture(tex, frag_tex_coords);
    if (color.a == 0.f) {
        discard;
    }
}

