#pragma once

#include <vector>

#include <gl/drawable.h>
#include <gl/gl.h>
#include <gl/shader.h>
#include <gl/texture.h>


struct Tile {
    int id;
    int x, y;
};


class Board {
private:

    program prog;
    drawable prot;
    texture_t tex;

    std::vector<Tile> tiles;

public:

    Board();

    ~Board();


    void render();
};


