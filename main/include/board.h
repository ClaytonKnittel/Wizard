#pragma once

#include <string>
#include <vector>

#include <gl/drawable.h>
#include <gl/gl.h>
#include <gl/shader.h>

#include <entity.h>
#include <tile.h>
#include <render_buffer.h>
#include <screen.h>
#include <texture_set.h>

class Board : public Entity {
private:

    RenderBuffer rbuf;

    program prog;
    // list of pairs of textures and the names of each of the textures
    std::vector<TextureSet> texs;

    std::vector<Tile> tiles;

    Tile preview;

    void make_generic();

public:

    Board(const std::string & file);

    ~Board();

    /*
     * save board configuration to a file
     */
    int save(const std::string & loc);

    /*
     * load board configuration from file
     */
    int load(const std::string & loc);


    void get_coords(double world_x, double world_y, int &x, int &y) const;

    /*
     * adds given tile to the map
     */
    void add_tile(int x, int y, int tex_idx);

    void set_preview(int x, int y, int tex_idx);

    virtual void render(const Screen & cam);
};


