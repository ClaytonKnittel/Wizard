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

    void make_generic();

    // called to delete current textures
    void unload_texs();

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

    virtual void render(const Screen & cam);
};


