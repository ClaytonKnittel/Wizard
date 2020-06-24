#pragma once

#include <map>
#include <vector>

#include <gl/drawable.h>
#include <gl/shader.h>



struct Vertex {
    // x and y are transformed screen coordinates, z is used only for depth
    // testing
    float x, y, z;

    // texture coordinates
    float tx, ty;
};


struct Triangle {
    Vertex vertices[3];
    texture * tex;
};

// forward declaration
struct __int_vertex;


class RenderBuffer {
private:

    program * prog;
    drawable d;

    size_t capacity;
    size_t idx;
    __int_vertex * pts;

    /*
     * map from textures to indices in texture array to be sent to GPU
     */
    std::map<texture *, int> tex_idxs;


public:

    RenderBuffer(program * p, size_t capacity);
    ~RenderBuffer();

    /*
     * pushes a vertex onto the buffer
     */
    RenderBuffer & operator<<(const Triangle &);

    /*
     * flushes all contents of the render buffer
     */
    void flush();
};


