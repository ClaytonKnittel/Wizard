
#include <cstdio>

#include <gl/texture.h>

#include <render_buffer.h>


RenderBuffer::RenderBuffer(program * p, size_t capacity) : prog(p), idx(0) {

    // drawing triangles
    this->capacity = capacity - (capacity % 3);

    gl_load_dynamic_textured(&d, NULL, this->capacity);

    pts = new Vertex[capacity];

    if (pts == nullptr) {
        fprintf(stderr, "Unable to allocate %lu bytes\n",
                capacity * sizeof(Vertex));
    }
}


RenderBuffer::~RenderBuffer() {
    gl_unload_static_monochrome_drawable(&d);
}


RenderBuffer & RenderBuffer::operator<<(const Vertex & v) {
    if (idx == capacity) {
        flush();
    }

    pts[idx] = v;
    idx++;

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

    //gl_use_program(prog);
    gl_draw_sub(&d, idx);

    idx = 0;
}

