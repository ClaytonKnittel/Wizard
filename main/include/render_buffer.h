#pragma once

#include <unordered_map>
#include <vector>

#include <gl/drawable.h>
#include <gl/shader.h>
#include <gl/texture.h>

#include <object.h>
#include <renderer.h>


// forward declaration
struct __int_vertex;


class RenderBuffer : public Renderer {
private:

    drawable d;

    size_t capacity;
    size_t idx;
    __int_vertex * pts;

    // number of texture units available on this hardware
    int n_texture_units;

    /*
     * map from textures to indices in texture array to be sent to GPU
     */
    std::unordered_map<const texture_t *, int> tex_idxs;


public:

    RenderBuffer(size_t capacity);
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


