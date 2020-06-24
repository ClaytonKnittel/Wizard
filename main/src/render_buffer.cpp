
#include <cstdio>

#include <gl/texture.h>

#include <render_buffer.h>



struct __int_vertex {
    // x and y are transformed screen coordinates, z is used only for depth
    // testing
    float x, y, z;

    // texture index of texture to be applied to this vertex (all 3 vertices
    // must have the same value for this)
    int texture_idx;

    // texture coordinates
    float tx, ty;
};


RenderBuffer::RenderBuffer(program * p, size_t capacity) : prog(p), idx(0) {

    // drawing triangles
    this->capacity = capacity - (capacity % 3);

    gl_load_dynamic_textured(&d, NULL, this->capacity);

    pts = new __int_vertex[capacity];

    if (pts == nullptr) {
        fprintf(stderr, "Unable to allocate %lu bytes\n",
                capacity * sizeof(Vertex));
    }
}


RenderBuffer::~RenderBuffer() {
    gl_unload_static_monochrome_drawable(&d);
}


RenderBuffer & RenderBuffer::operator<<(const Triangle & t) {
    if (idx == capacity) {
        flush();
    }

    int tex_idx;

    auto it = tex_idxs.find(t.tex);

    if (it == tex_idxs.end()) {
        tex_idx = tex_idxs.size();
        tex_idxs[t.tex] = tex_idx;
    }
    else {
        tex_idx = it->second;
    }

    __int_vertex vs[3];

    __builtin_memcpy(&vs[0].x,  &t.vertices[0].x,  3 * sizeof(float));
    vs[0].texture_idx = tex_idx;
    __builtin_memcpy(&vs[0].tx, &t.vertices[0].tx, 2 * sizeof(float));
    __builtin_memcpy(&vs[1].x,  &t.vertices[1].x,  3 * sizeof(float));
    vs[1].texture_idx = tex_idx;
    __builtin_memcpy(&vs[1].tx, &t.vertices[1].tx, 2 * sizeof(float));
    __builtin_memcpy(&vs[2].x,  &t.vertices[2].x,  3 * sizeof(float));
    vs[2].texture_idx = tex_idx;
    __builtin_memcpy(&vs[2].tx, &t.vertices[2].tx, 2 * sizeof(float));

    pts[idx]     = vs[0];
    pts[idx + 1] = vs[1];
    pts[idx + 2] = vs[2];
    idx += 3;

    return *this;
}

void RenderBuffer::flush() {
    gl_update_dynamic_textured(&d, (float*) pts, idx);

    for (int i = 0; i < 0; i++) {
        printf("(%f, %f, %f) %d (%f, %f)\n",
                pts[i].x, pts[i].y, pts[i].z,
                pts[i].texture_idx,
                pts[i].tx, pts[i].ty);
    }

    for (int i = 0; i < 16; i++) {
        int loc = gl_uniform_location(prog, "texs") + i;
        glUniform1i(loc, i);
    }

    for (auto it = tex_idxs.begin(); it != tex_idxs.end(); it++) {
        texture_use_idx(it->first, it->second);
    }

    //gl_use_program(prog);
    gl_draw_sub(&d, idx);

    idx = 0;
}

