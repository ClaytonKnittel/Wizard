#pragma once

#include <vector>

#include <gl/drawable.h>
#include <gl/gl.h>
#include <gl/shader.h>
#include <gl/texture.h>

#include <entity.h>
#include <tile.h>
#include <render_buffer.h>

class Board : public Entity {
private:

    RenderBuffer rbuf;

    program prog;
    //drawable prot;
    texture_t * texs;

    std::vector<Tile> tiles;

public:

    Board();

    ~Board();


    virtual void render();
};


