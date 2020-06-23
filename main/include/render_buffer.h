#pragma once

#include <vector>

#include <gl/drawable.h>
#include <gl/shader.h>



struct Vertex {
    // x and y are transformed screen coordinates, z is used only for depth
    // testing
    float x, y, z;

    // texture index of texture to be applied to this vertex (all 3 vertices
    // must have the same value for this)
    int texture_idx;

    // texture coordinates
    float tx, ty;
};


class RenderBuffer {
private:

    program * prog;
    drawable d;

    size_t capacity;
    size_t idx;
    Vertex * pts;


public:

    RenderBuffer(program * p, size_t capacity);
    ~RenderBuffer();

    /*
     * pushes a vertex onto the buffer
     */
    RenderBuffer & operator<<(const Vertex &);

    /*
     * flushes all contents of the render buffer
     */
    void flush();
};


