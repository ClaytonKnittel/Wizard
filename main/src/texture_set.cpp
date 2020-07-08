
#include <texture_set.h>


TextureSet::TextureSet(const std::string & img_file, int tile_w, int tile_h) :
        img_file(img_file), tile_w(tile_w), tile_h(tile_h) {

    int res = texture_init(&this->tex, img_file.c_str());

    if (res != 0) {
        fprintf(stderr, "Unable to load texture %s\n", img_file.c_str());
    }
}


TextureSet::~TextureSet() {
    texture_destroy(&this->tex);
}


const std::string & TextureSet::get_img_file() const {
    return img_file;
}

const texture_t * TextureSet::get_tex() const {
    return &tex;
}


void TextureSet::get_bounds(int tex_idx, float & llx, float & lly, float & urx,
            float & ury) const {

    float w = static_cast<float>(this->tile_w);
    float h = static_cast<float>(this->tile_h);

    int tex_x = tex_idx % tile_w;
    int tex_y = tex_idx / tile_w;

    llx = tex_x / w;
    lly = tex_y / h;
    urx = (tex_x + 1) / w;
    ury = (tex_y + 1) / h;
}

