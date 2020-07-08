

#include <cmath>
#include <sstream>
#include <unordered_map>
#include <random>

#include <math/random.h>

#include <bin_stream.h>
#include <board.h>
#include <util.h>


#define N_TEXS 4

void Board::make_generic() {

#define W 5
#define H 5

    texs.emplace_back("main/img/test.bmp", 2, 2);
    //texs.emplace_back("main/img/gsquare3.bmp", 1, 1);

    for (int i = 0; i < W * H; i++) {
        tiles.emplace_back(
                texs[0],
                (i / W + i % W) % N_TEXS,
                i % W,
                i / W
                );
    }
}

Board::Board(const std::string & file) : Entity(0, 0, .08f), rbuf(8000) {

    make_generic();
    //load(file);

    gl_load_program(&prog, "main/res/tile.vs", "main/res/tile.fs");

    gl_use_program(&prog);

    // bind each texture in fragment shader to the proper texture unit
    int loc = gl_uniform_location(&prog, "texs");
    for (int i = 0; i < 16; i++) {
        glUniform1i(loc + i, i);
    }
}


Board::~Board() {
    gl_unload_program(&prog);
}



int Board::save(const std::string & loc) {
    bofstream f;

    f.open(loc, std::ios::binary | std::fstream::trunc);

    if (!f.is_open()) {
        fprintf(stderr, "Unable to open file %s for writing\n", loc.c_str());
        return -1;
    }

    bstringstream texs;
    bstringstream dat;
    std::unordered_map<const TextureSet *, int> tex_idxs;

    for (const Tile & t : tiles) {
        int tex_idx;
        auto it = tex_idxs.find(t.texset);

        if (it == tex_idxs.end()) {
            tex_idx = tex_idxs.size();
            tex_idxs[t.texset] = tex_idx;

            texs << t.texset->get_img_file();
            texs << t.texset->get_tile_w() << t.texset->get_tile_h();
        }
        else {
            tex_idx = it->second;
        }

        dat << t.x << t.y << tex_idx << t.tex_idx;
    }

    uint32_t n_tiles = tiles.size();
    uint32_t n_texs = tex_idxs.size();
    f << n_tiles << n_texs;
    // write without encoding length of strings
    f.write(texs.str());
    f.write(dat.str());

    return 0;
}


int Board::load(const std::string & loc) {
    bifstream f;

    f.open(loc, std::ios::binary);

    uint32_t n_tiles;
    uint32_t n_texs;

    f >> n_tiles >> n_texs;

    texs.clear();
    tiles.clear();

    for (int i = 0; i < n_texs; i++) {
        std::string name;
        int tile_w, tile_h;
        f >> name >> tile_w >> tile_h;

        texs.emplace_back(std::move(name), tile_w, tile_h);
    }

    for (int i = 0; i < n_tiles; i++) {
        tiles.resize(i + 1);
        Tile & t = tiles[i];

        int tex_idx, tex_idx2;
        f >> t.x >> t.y >> tex_idx >> tex_idx2;
        t.texset = &texs[tex_idx];
        t.tex_idx = tex_idx2;

        t.gen_vertices();
    }

    set_preview(0, 0, 0);

    return 0;
}


void Board::get_coords(double world_x, double world_y, int &x, int &y) const {

    x = floor((world_x - this->x) / this->scale);
    y = floor((world_y - this->y) / this->scale); 

}


void Board::add_tile(int x, int y, int tex_idx) {
    for (auto it = tiles.begin(); it != tiles.end();) {
        const Tile &t = *it;
        if (t.x == x && t.y == y) {
            it = tiles.erase(it);
        }
        else {
            it++;
        }
    }
    tiles.emplace_back(texs[tex_idx], 0, x, y);
}


void Board::set_preview(int x, int y, int tex_idx) {
    if (preview.x != x || preview.y != y || preview.texset != &texs[tex_idx]) {

        preview.x = x;
        preview.y = y;
        preview.texset = &texs[tex_idx];
        preview.tex_idx = 0;
        preview.gen_vertices();
    }
}


void Board::render(const Screen & screen) {
    gl_use_program(&prog);

    // upload camera and view transformation matrices
    screen.apply(&prog);

    // call superclass render callback
    upload_pos(&prog);

    GLuint prev = gl_uniform_location(&prog, "preview");
    glUniform1i(prev, false);

    // upload all tiles to renderer
    for (Tile & t : tiles) {
        t.insert_all(rbuf);
    }

    // flush renderer to screen
    rbuf.flush();

    glUniform1i(prev, true);
    preview.insert_all(rbuf);
    rbuf.flush();

}

