#pragma once

#include <string>

#include <gl/texture.h>


class TextureSet {
private:

    texture_t tex;
    std::string img_file;

    int tile_w, tile_h;


public:

    TextureSet(const std::string & img_file, int tile_w, int tile_h);
    ~TextureSet();

    const std::string & get_img_file() const;

    const texture_t * get_tex() const;

    /*
     * given a texture index, returns the lower and upper bounds of the texture
     * in the texture map
     */
    void get_bounds(int tex_idx, float & llx, float & lly, float & urx,
            float & ury) const;

};


