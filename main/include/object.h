#pragma once

#include <gl/texture.h>

#include <renderer.h>
#include <primitives.h>


/*
 * interface for all renderable objects, to be used for objects which contain
 * a well defined set of vertices which are fairly static throughout the
 * object's life (but are free to change, just not optimized for that)
 */
class Object {
public:

    // visitor for inserting all primitives of this object to a render buffer
    // via insert operator
    virtual void insert_all(Renderer &) = 0;

};

