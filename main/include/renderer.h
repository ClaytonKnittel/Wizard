#pragma once

#include <primitives.h>

class Renderer {
public:

    /*
     * push triangle primitive onto render object
     */
    virtual Renderer & operator<<(const Triangle & t) = 0;

};

