#pragma once

struct Vertex {
    // x and y are transformed screen coordinates, z is used only for depth
    // testing
    float x, y;

    // texture coordinates
    float tx, ty;
};


struct Triangle {
    Vertex vertices[3];
    texture * tex;
};


