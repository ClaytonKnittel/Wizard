#pragma once

#include <gl/texture.h>

#include <object.h>

// 2 triangles per face, 5 sides (bottom never visible)
#define TILE_N_PRIMITIVES 2


/*
 * for tiles on the board, are always a fixed size (1x1x1) and must lie on
 * integer-aligned coordinates
 */
class Tile : public Object {
    friend class Board;
private:

    // which texture will be mapped to the tile
    texture_t * tex;
    int x, y;

    Triangle faces[TILE_N_PRIMITIVES];

    void gen_vertices();

public:

    Tile();
    Tile(texture * tex, int x, int y);
    virtual ~Tile();

    virtual void insert_all(Renderer &);
};




