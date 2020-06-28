#pragma once

#include <string>
#include <vector>

#include <gl/drawable.h>
#include <gl/gl.h>
#include <gl/shader.h>
#include <gl/texture.h>

#include <entity.h>
#include <tile.h>
#include <render_buffer.h>
#include <screen.h>

class Board : public Entity {
private:

    RenderBuffer rbuf;

    program prog;
    //drawable prot;
    texture_t * texs;
    // name of each of the textures
    std::string * tex_files;

    std::vector<Tile> tiles;

public:

    Board();

    ~Board();

    /*
     * save board configuration to a file
     */
    int save(const char * loc);

    /*
     * load board configuration from file
     */
    int load(const char * loc);

    virtual void render(const Screen & cam);
};


