#version 330 core


in float frag_darkness;

out vec4 color;


void main() {
    float d = .5 + .5 * round(frag_darkness);
    color = vec4(0.f, 0.f, 0.f, d);
}

